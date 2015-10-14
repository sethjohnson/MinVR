/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef GRAPHICSCONTEXT_H_
#define GRAPHICSCONTEXT_H_

#include "MVRCore/AbstractWindow.H"
#include "MVRCore/Thread.h"
#include <map>

namespace MinVR {

struct GraphicsContextHolder
{
	int threadId;
	WindowRef window;
};

class GraphicsContext {
public:
	static void setCurrentContext(GraphicsContextHolder context);
	static void cleanup();
	static GraphicsContextHolder getCurrentContext();

	static std::map<int, GraphicsContextHolder> contextMap;
	static THREAD_LOCAL int currentThreadId;
};

} /* namespace MinVR */

#endif /* GRAPHICSCONTEXT_H_ */
