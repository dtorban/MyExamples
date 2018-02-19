/*
 * Copyright Regents of the University of Minnesota, 2017.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef MULTITHREADEDAPP_H_
#define MULTITHREADEDAPP_H_

#include <main/VREventHandler.h>
#include <main/VRGraphicsHandler.h>
#include <main/VRMain.h>

#include <map>
#include <mutex>
#include "AppRenderer.h"
#include "AppSharedContext.h"

namespace MinVR {
class MultithreadedApp : public VREventHandler, public VRGraphicsHandler {
public:
	MultithreadedApp(int argc, char** argv);
	virtual ~MultithreadedApp();

	/// Handles and updates Graphics Context
	void onVRRenderGraphicsContext(const MinVR::VRGraphicsState &renderState);
	/// Handles Graphics rendering
	void onVRRenderGraphics(const MinVR::VRGraphicsState &renderState);

	virtual AppSharedContext* createSharedContext(const MinVR::VRGraphicsState &renderState) { return new AppSharedContext(); }
	virtual AppRenderer* createRenderer(AppSharedContext& sharedContext, const MinVR::VRGraphicsState &renderState) = 0;

	virtual void update() {}

	virtual void run();
	bool isRunning() { return running; }
	void shutdown() { running = false; }

protected:
	std::map<int, AppRenderer*> views;
	std::map<int, AppSharedContext*> sharedContexts;
	std::map<int, int> sharedContextCount;
	std::map<int, AppSharedContext*> normalContexts;
	std::mutex mutex;
	int frame;
	VRMain* vrmain;
	bool running;
};

}

#endif /* MULTITHREADEDAPP_H_ */
