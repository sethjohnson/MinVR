/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VERSIONEDITEM_H_
#define VERSIONEDITEM_H_

namespace MinVR {

class VersionedItem {
public:
	virtual ~VersionedItem() {}

	virtual int getVersion() const { return _version; }
	void incrementVersion() {_version++;}

private:
	int _version = 0;
};

}

#endif /* VERSIONEDITEM_H_ */
