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

class AbstractMVRApp;

struct WindowInfo
{
	int threadId;
	WindowRef window;
	int viewportIndex;

	AbstractCameraRef getCamera() const { return window->getCamera(viewportIndex); }
};

struct AppInfo
{
	AbstractMVRApp* app;
};

class RenderDevice {
public:
	RenderDevice(const WindowInfo& windowInfo, const AppInfo& appInfo);
	virtual ~RenderDevice();

	const WindowInfo& getWindowInfo() const {
		return _windowInfo;
	}

	const AppInfo& getAppInfo() const {
		return _appInfo;
	}

private:
	WindowInfo _windowInfo;
	AppInfo _appInfo;
};

} /* namespace MinVR */

#endif /* RENDERDEVICE_H_ */
