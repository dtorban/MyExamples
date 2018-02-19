/*
 * Copyright Regents of the University of Minnesota, 2017.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef APPRENDERER_H_
#define APPRENDERER_H_

#include <mutex>
#include <api/MinVR.h>

namespace MinVR {

class AppRenderer {
public:
	virtual ~AppRenderer() {}

	virtual void update(const MinVR::VRGraphicsState &renderState) {}
	virtual void render(const MinVR::VRGraphicsState &renderState) {}
};

}

#endif /* APPRENDERER_H_ */
