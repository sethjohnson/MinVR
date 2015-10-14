/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef GRAPHICSOBJECT_H_
#define GRAPHICSOBJECT_H_

#include "MVRCore/GraphicsContext.H"
#include "MVRCore/RenderDevice.H"

namespace MinVR {

class GraphicsObject : public ContextObject {
public:
	virtual ~GraphicsObject() {}

	virtual void draw(RenderDevice& renderDevice) = 0;
};

} /* namespace MinVR */

#endif /* GRAPHICSOBJECT_H_ */
