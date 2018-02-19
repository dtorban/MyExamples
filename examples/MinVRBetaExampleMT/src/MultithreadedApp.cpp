/*
 * Copyright Regents of the University of Minnesota, 2017.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include "MultithreadedApp.h"

namespace MinVR {

MultithreadedApp::MultithreadedApp(int argc, char** argv) : running(false), frame(0) {
	vrmain = new VRMain();
	vrmain->initialize(argc, argv);
	vrmain->addEventHandler(this);
	vrmain->addRenderHandler(this);
}

MultithreadedApp::~MultithreadedApp() {
	vrmain->shutdown();
	delete vrmain;
}

void MultithreadedApp::run() {
	running = true;
	while (running) {
		vrmain->synchronizeAndProcessEvents();
		update();
		vrmain->renderOnAllDisplays();
		frame++;
	}
}

void MultithreadedApp::onVRRenderGraphicsContext(const MinVR::VRGraphicsState &renderState) {
	int windowId = renderState.getWindowId();
	int sharedContextId = renderState.getSharedContextId();

	// If this is the inital call, initialize context variables
	if (renderState.isInitialRenderCall()) {
		// Need to lock here so that we can add the context objects one by one
		std::unique_lock<std::mutex> lock(mutex);

		// If a shared context Id exists, create one per shared context, otherwise one per window
		AppSharedContext* context = NULL;
		if (sharedContextId < 0) {
			// Create a shared context per window if this window doesn't have a shared context
			context = createSharedContext(renderState);
			normalContexts[windowId] = context;
		}
		else {
			// Use shared context if it already exists, otherwise create one
			std::map<int, AppSharedContext*>::iterator it = sharedContexts.find(sharedContextId);
			if (it == sharedContexts.end()) {
				context = createSharedContext(renderState);
				sharedContexts[sharedContextId] = context;
				sharedContextCount[sharedContextId] = 1;
			}
			else {
				context = it->second;
				sharedContextCount[sharedContextId]++;
			}
		}

		// Create one view per window and use shared context
		views[windowId] = createRenderer(*context, renderState);
		lock.unlock();
	}
	else {
		if (sharedContextId < 0) {
			// Update window shared context if no shared context exists
			normalContexts[windowId]->update(renderState, frame);
		}
		else {
			// Update shared context if it exists
			sharedContexts[sharedContextId]->update(renderState, frame);
		}

		// Update the view for the window
		views[windowId]->update(renderState);
	}

	// Destroy context items if the program is no longer running
	if (!isRunning()) {
		// Need to lock for deleting
		std::unique_lock<std::mutex> lock(mutex);

		// Delete view
		{
			std::map<int, AppRenderer*>::iterator it = views.find(windowId);
			delete it->second;
			views.erase(it);
		}

		// Delete shared contexts
		if (sharedContextId < 0) {
			std::map<int, AppSharedContext*>::iterator it = normalContexts.find(windowId);
			delete it->second;
			normalContexts.erase(it);
		}
		else {
			sharedContextCount[sharedContextId]--;
			if (sharedContextCount[sharedContextId] == 0) {
				std::map<int, AppSharedContext*>::iterator it = sharedContexts.find(sharedContextId);
				delete it->second;
				sharedContexts.erase(it);
			}
		}

		lock.unlock();

		return;
	}
}

/// onVRRenderScene will run draw calls on each viewport inside a context.
void MultithreadedApp::onVRRenderGraphics(const MinVR::VRGraphicsState &renderState) {
	// Only draw if the application is still running.
	if (isRunning()) {
		int windowId = renderState.getWindowId();

		// Render on view
		views[windowId]->render(renderState);
	}
}

}
