/* ================================================================================

This file is part of the MinVR Open Source Project, which is developed and 
maintained collaboratively by the University of Minnesota's Interactive 
Visualization Lab and the Brown University Visualization Research Lab.

File: MinVR/MVRCore/include/MVRCore/ConfigVal.H

Original Author(s) of this File: 
	Daniel Keefe, 2004, Brown University (originally VRG3D/VRBase/ConfigVal.H, inspired by Bob Zeleznik's CONFIGval's)
	
Author(s) of Significant Updates/Modifications to the File:
	Bret Jackson, 2013, University of Minnesota (adapted to MinVR using boost)

-----------------------------------------------------------------------------------
Copyright (c) 2008-2015 Regents of the University of Minnesota and Brown University
Copyright (c) 2004-2008 Brown University
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.

* Neither the name of the University of Minnesota, Brown University, nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
================================================================================ */


#ifndef CONFIGVAL_H
#define CONFIGVAL_H

#include <iostream>
#include <sstream>
#include "MVRCore/ConfigMap.H"
#include <log/Logger.h>

namespace MinVR {

/**
ConfigVal's and config files description: 

ConfigVal's are an easy way to access program settings that can be
stored in a configuration file.  They are all key=value pairs where
key and value are both originally std::strings.  But, the value
string can be easily reinterpreted by any class that overrides the
stream >> and << operators.  The ConfigVal() function figures out
what type to try to convert to by the type of the second parameter
to the function.  This parameter also specifies the default value
to return if the key is not found in the ConfigMap::_map Table.

To use a config file, call ConfigMap::readFile() with the filename
of a text file to read.  The format of each line of the file should be:

key value

where key has no spaces in it.  value is everything after the space
until the end of the line, so it can have spaces in it.  Any line
that *starts* with the character # is a comment and is ignored.
A single \ character means cut the line here and continue on the
next line.  Everything after the \ is ignored and the text on the
next line is appended to the text on the current line.  You can 
escape the \ operator with a \\ to get a single \ character.

Additionally, any value X appearing inside the special character
sequence $(X) will be replaced by the value of the environment
variable named X.  If X is not defined, it will be replaced by a
null string.  If X takes the form of the path to a file and you're
running in Windows under cygwin then X will be automatically 
converted from the form /cygdrive/c/blah/blah to the more Windows 
friendly form c:/blah/blah to stay compatible with the Visual C++ 
compiler.

Additionally, if you put a += after the key and the key has already
been defined, then the value is appended to the key's current value.
If you don't have the += and the same key is read in from a file,
then the new value overwrites the old.  For example:

mykey1     value1
mykey1     value2
mykey2     value3
mykey2+=   value4

The value of mykey1 will be "value2".  The value of mykey2 will be 
"value3 value4".
*/


/// This static class holds a ConfigMap that is used for the
/// ConfigVal function.
class ConfigValMap
{
public:
	static ConfigMapRef map;
};



/// This function is used to convert from a string type to something else
template <class T>
static inline bool retypeVal(const std::string &str, T &val) {
	std::istringstream is(str.c_str());
	is >> val;
	if (!is) return false;
	else return true;
}


static inline void notFoundWarning(const std::string &keyString) {
	std::cout << "No ConfigVal mapping found for \"" << keyString << "\"." << std::endl;
}


/// This is the main templated function that is used to access a ConfigVal.  The value
/// is stored internally as a string, the function will attempt to reinterpret the value
/// as the same type as that which is passed in to the defaultVal parameter.  This function
/// is overridden by several variants below for various string-like types, which generally
/// don't require a type conversion, but can confuse a compiler.
template <class KEYTYPE, class VALTYPE>
VALTYPE ConfigVal(KEYTYPE keyString, const VALTYPE &defaultVal, bool warn=true) {
	MinVR::Logger::getInstance().assertMessage(ConfigValMap::map != nullptr , "The global config map is NULL!");
	if (!ConfigValMap::map->containsKey(keyString)) {
		if (warn) {
			notFoundWarning(keyString);
		}
		return defaultVal;
	}
	else {
		std::string valString = ConfigValMap::map->getValue(keyString);
		//std::cout << "valString = " << valString << std::endl;
		VALTYPE val;
		bool ok = retypeVal(valString, val);
		if (!ok) {
			std::string errString = std::string("ERROR: ConfigVal is unable to retype value.\n\tKey:\t'")
            + keyString + std::string("'\n\tValue:\t'") + valString + std::string("'\n");
			std::cout << errString;
		}
		return val;
	}
}


// Override the template above for the case of a quoted string passed in as the 
// default value for the ConfigVal function call.  The quoted string takes on
// different types depending on the compiler (hence the #ifdef's below.)

#if (defined(__SUNPRO_CC) && __SUNPRO_CC >= 0x500) || \
(defined(__GNUC__) && (__GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 95)))
#define QUOTED_STRING const char *
#else
#define QUOTED_STRING char *
#endif

inline std::string ConfigVal(QUOTED_STRING keyString, QUOTED_STRING defaultVal, bool warn=true) {
	MinVR::Logger::getInstance().assertMessage(ConfigValMap::map != nullptr, "The global config map is NULL!");
	if (ConfigValMap::map->containsKey(keyString))
		return replaceEnvVars(ConfigValMap::map->getValue(keyString));
	else {
		if (warn) notFoundWarning(keyString);
		return replaceEnvVars(std::string(defaultVal));
	}
}

inline std::string ConfigVal(QUOTED_STRING keyString, std::string defaultVal, bool warn=true) {
	MinVR::Logger::getInstance().assertMessage(ConfigValMap::map != nullptr , "The global config map is NULL!");
	if (ConfigValMap::map->containsKey(keyString))
		return replaceEnvVars(ConfigValMap::map->getValue(keyString));
	else {
		if (warn) notFoundWarning(keyString);
		return replaceEnvVars(defaultVal);
	}
}

inline std::string ConfigVal(std::string keyString, QUOTED_STRING defaultVal, bool warn=true) {
	MinVR::Logger::getInstance().assertMessage(ConfigValMap::map != nullptr, "The global config map is NULL!");
	if (ConfigValMap::map->containsKey(keyString))
		return replaceEnvVars(ConfigValMap::map->getValue(keyString));
	else {
		if (warn) notFoundWarning(keyString);
		return replaceEnvVars(std::string(defaultVal));
	}
}

inline std::string ConfigVal(std::string keyString, std::string defaultVal, bool warn=true) {
	MinVR::Logger::getInstance().assertMessage(ConfigValMap::map != nullptr, "The global config map is NULL!");
	if (ConfigValMap::map->containsKey(keyString))
		return replaceEnvVars(ConfigValMap::map->getValue(keyString));
	else {
		if (warn) notFoundWarning(keyString);
		return replaceEnvVars(defaultVal);
	}
}


} // end namespace


#endif

