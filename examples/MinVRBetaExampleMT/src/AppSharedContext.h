/*
 * Copyright Regents of the University of Minnesota, 2017.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef APPSHAREDCONTEXT_H_
#define APPSHAREDCONTEXT_H_

#include <mutex>
#include <api/MinVR.h>
#include <iostream>

namespace MinVR {

class AppSharedContext {
public:
	AppSharedContext() : version(0) {}
	virtual ~AppSharedContext() {}

	void update(const MinVR::VRGraphicsState &renderState, int version);

protected:
	virtual void update(const MinVR::VRGraphicsState &renderState) {}

private:
	std::mutex mutex;
	int version;
};

inline void AppSharedContext::update(const MinVR::VRGraphicsState& renderState, int version) {
	// In this application, only one window will update the shared context at a time, hence the locking
	if (version != this->version) {
		std::unique_lock<std::mutex> lock(mutex);
		if (version != this->version) {

			update(renderState);

			this->version = version;
		}
	}
}

}

#endif /* SHAREDCONTEXT_H_ */
