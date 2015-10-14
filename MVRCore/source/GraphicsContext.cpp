/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <MVRCore/GraphicsContext.H>

namespace MinVR {

std::map<int, GraphicsContextHolder> GraphicsContext::contextMap = std::map<int, GraphicsContextHolder>();
THREAD_LOCAL int GraphicsContext::currentThreadId = -1;

void GraphicsContext::setCurrentContext(GraphicsContextHolder context)
{
	if (context.threadId >= 0)
	{
		contextMap[context.threadId] = context;
	}

	currentThreadId = context.threadId;
}

void GraphicsContext::cleanup()
{
	contextMap.clear();
	currentThreadId = -1;
}

GraphicsContextHolder GraphicsContext::getCurrentContext()
{
	if (currentThreadId >= 0)
	{
		return contextMap[currentThreadId];
	}

	GraphicsContextHolder context = { -1, NULL };
	return context;
}

} /* namespace MinVR */
