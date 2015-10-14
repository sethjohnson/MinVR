/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef RENDERDEVICE_H_
#define RENDERDEVICE_H_

#include "MVRCore/Event.H"
#include "MVRCore/AbstractWindow.H"
#include "MVRCore/AbstractCamera.H"
#include <memory>

namespace MinVR {

struct WindowInfo
{
	int threadId;
	WindowRef window;
	int viewportIndex;

	AbstractCameraRef getCamera() const { return window->getCamera(viewportIndex); }
};

class RenderDevice {
public:
	RenderDevice(const WindowInfo& windowInfo);
	virtual ~RenderDevice();

	const WindowInfo& getWindowInfo() const {
		return _windowInfo;
	}

private:
	WindowInfo _windowInfo;
};

} /* namespace MinVR */

#endif /* RENDERDEVICE_H_ */
