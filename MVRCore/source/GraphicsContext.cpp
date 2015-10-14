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

ContextObject::~ContextObject() {
}

void ContextObject::initContext()
{
	if (!isInitialized())
	{
		initContextItem();
		_oldVersion.reset(new int(getVersion()));
		_initialized.reset(new bool(true));
	}
}

void ContextObject::updateContext()
{
	if (!isInitialized())
	{
		initContext();
	}

	int version = getVersion();
	if (updateContextItem(*_oldVersion != version))
	{
		*_oldVersion = version;
	}
}

void ContextObject::cleanupContext()
{
	if (GraphicsContext::getCurrentContext().threadId < 0)
	{
		std::vector<GraphicsContextHolder> contexts = _initialized.getContexts();
		for (int f = 0; f < contexts.size(); f++)
		{
			GraphicsContext::currentThreadId = contexts[f].threadId;
			contexts[f].window->makeContextCurrent();
			cleanupContextItem();
			contexts[f].window->releaseContext();
		}

		GraphicsContext::currentThreadId = -1;
	}
}

void ContextObject::updateImediately()
{
	if (GraphicsContext::currentThreadId < 0)
	{
		std::vector<GraphicsContextHolder> contexts = _initialized.getContexts();
		for (int f = 0; f < contexts.size(); f++)
		{
			GraphicsContext::currentThreadId = contexts[f].threadId;
			contexts[f].window->makeContextCurrent();
			updateContext();
			contexts[f].window->releaseContext();
		}

		GraphicsContext::currentThreadId = -1;
	}
}

} /* namespace MinVR */
