/*
 * Copyright Regents of the University of Minnesota, 2014.  This software is released under the following license: http://opensource.org/licenses/lgpl-3.0.html.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef INPUTDEVICE_H_
#define INPUTDEVICE_H_

#include <memory>
#include "MVRCore/ConfigMap.H"
#include "MVRCore/Event.H"

namespace MinVR {
namespace framework {

typedef std::shared_ptr<class InputDevice> InputDeviceRef;
typedef std::shared_ptr<class InputDeviceDriver> InputDeviceDriverRef;

/*! @brief Base class for InputDevices.
 *  Input Devices should be polled once
 *  per frame and append Events that were generated since the last
 *  call to poll to the array passed into the poll routine.
 */
class InputDevice
{
public:
	InputDevice() {}
  virtual ~InputDevice() {}

  /*! @brief Adds device events to event queue.
	*
	*  Adds any events generated by the input device to the events array.
	*
	*  @remarks This should be implemented by any derived classes.
	*/
  virtual void pollForInput(std::vector<EventRef> &events) = 0;
};

class InputDeviceDriver {
public:
	virtual ~InputDeviceDriver() {}

	virtual InputDeviceRef create(const std::string &type, const std::string& name, ConfigMapRef config) = 0;
};

} /* namespace framework */
} /* namespace MinVR */

#endif /* INPUTDEVICE_H_ */