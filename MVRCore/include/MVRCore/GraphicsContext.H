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
#include "MVRCore/VersionedItem.H"

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

template<typename T>
class ContextSpecificPtr {
public:
	ContextSpecificPtr();
	ContextSpecificPtr(T* value);
	virtual ~ContextSpecificPtr();
	T* get();
	void reset(T* value);
	T& operator*();
	T* operator->();
	std::vector<GraphicsContextHolder> getContexts();

private:
	std::map<int, T*> _threadMap;
};

class ContextObject : public VersionedItem {
public:
	virtual ~ContextObject();
	void initContext();
	void updateContext();
	void cleanupContext();
	void updateImediately();

protected:
	virtual void initContextItem() = 0;
	virtual bool updateContextItem(bool changed) = 0;
	virtual void cleanupContextItem() = 0;

private:
	bool isInitialized() { return _initialized.get() != NULL && *_initialized; }
	ContextSpecificPtr<bool> _initialized;
	ContextSpecificPtr<int> _oldVersion;
};

//-----------------------------------------------------------

template<typename T>
ContextSpecificPtr<T>::ContextSpecificPtr() {}

template<typename T>
ContextSpecificPtr<T>::ContextSpecificPtr(T* value) { reset(value); }

template<typename T>
ContextSpecificPtr<T>::~ContextSpecificPtr() {
	typedef typename std::map<int, T*>::iterator it_type;
	for(it_type iterator = _threadMap.begin(); iterator != _threadMap.end(); iterator++) {
		delete iterator->second;
	}
}

template<typename T>
T* ContextSpecificPtr<T>::get()
{
	if (GraphicsContext::currentThreadId < 0) { return NULL; }

	typedef typename std::map<int, T*>::iterator it_type;
	it_type it = _threadMap.find(GraphicsContext::currentThreadId);
	if (it != _threadMap.end())
	{
		return it->second;
	}

	return NULL;
}

template<typename T>
void ContextSpecificPtr<T>::reset(T* value)
{
	T* val = get();
	if (val != NULL)
	{
		delete val;
	}

	if (GraphicsContext::currentThreadId >= 0)
	{
		_threadMap[GraphicsContext::currentThreadId] = value;
	}
}

template<typename T>
T& ContextSpecificPtr<T>::operator*() {return *get();}

template<typename T>
T* ContextSpecificPtr<T>::operator->() {return get();}

template<typename T>
std::vector<GraphicsContextHolder> ContextSpecificPtr<T>::getContexts()
{
	std::vector<GraphicsContextHolder> contexts;

	typedef typename std::map<int, T*>::iterator it_type;
	for(it_type iterator = _threadMap.begin(); iterator != _threadMap.end(); iterator++) {
		contexts.push_back(GraphicsContext::contextMap[iterator->first]);
	}

	return contexts;
}

} /* namespace MinVR */

#endif /* GRAPHICSCONTEXT_H_ */
