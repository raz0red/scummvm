/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#define FORBIDDEN_SYMBOL_ALLOW_ALL

#include "common/scummsys.h"
#include "engine.h"

#if defined(__EMSCRIPTEN__)
#include "backends/platform/sdl/emscripten/emscripten.h"
#include "backends/events/emscripten/emscripten-events.h"
#include "backends/plugins/sdl/sdl-provider.h"
#include "base/main.h"
#include <emscripten.h>

bool emPaused = false;
bool emShowScummMenu = false;
bool emFilterMouseEvents = false;

extern EmscriptenEventSource* emEventSource;

int main(int argc, char *argv[]) {

	// Create our OSystem instance
	g_system = new OSystem_Emscripten();
	assert(g_system);

	// Pre initialize the backend
	g_system->init();

#ifdef DYNAMIC_MODULES
	PluginManager::instance().addPluginProvider(new SDLPluginProvider());
#endif

	for (int i = 0; i < argc; i++) {
		printf("%s\n", argv[i]);
	}

	// Invoke the actual ScummVM main entry point:
	int res = scummvm_main(argc, argv);

	// Free OSystem
	g_system->destroy();

	EM_ASM(
		window.emulator.onExit();
	);

	return res;
}

extern "C" void emSetTouchpadMouseMode(int enable) {
	printf("## emSetTouchpadMouseMode: %d\n", enable);
	ConfMan.setBool("touchpad_mouse_mode", enable);
	emEventSource->reset();
}

extern "C" void emSetFilterMouseEvents(int filter) {
	emFilterMouseEvents = filter;
	emEventSource->reset();
}

extern int getSdlFilterModeEvent();

int emFilterMode = -1;
extern "C" void emSetFilterEnabled(int enabled) {
	printf("### Set filter mode: %d\n", enabled);
	emFilterMode = enabled;

	Common::Event eventQ;
	eventQ.type = Common::EVENT_CUSTOM_BACKEND_ACTION_START;
	eventQ.customType = getSdlFilterModeEvent();
	g_system->getEventManager()->pushEvent(eventQ);
}

extern int getSdlStretchModeEvent();

int emStretchMode = -1;
extern "C" void emSetStretchMode(int mode) {
	printf("### Set stretch mode: %d\n", mode);
	emStretchMode = mode;

	Common::Event eventQ;
	eventQ.type = Common::EVENT_CUSTOM_BACKEND_ACTION_START;
	eventQ.customType = getSdlStretchModeEvent();
	g_system->getEventManager()->pushEvent(eventQ);
}

static int pauseCount = 0;

extern "C" void emPause() {
	pauseCount++;
	emPaused = true;
}

extern "C" void emUnpause() {
	pauseCount--;
	if (pauseCount == 0) {
		emPaused = false;
	}
}

extern "C" void emQuit() {
	Common::Event eventQ;
	eventQ.type = Common::EVENT_QUIT;
	g_system->getEventManager()->pushEvent(eventQ);
}

extern "C" int saveGame() {
	auto err = g_engine->saveGameState(0, "foo", false);
	printf("## Save result: %d\n", err.getCode());
	return err.getCode();
}

extern "C" int loadGame() {
	auto err = g_engine->loadGameState(0);
	printf("## Load result: %d\n", err.getCode());
	return err.getCode();
}

extern "C" void emOpenScummMainMenuDialog() {
	emShowScummMenu = true;
}

extern "C" void emSaveScreenshot(){
	g_system->saveScreenshot();
}

extern "C" void emKeyboard() {
	Common::Event eventQ;
	eventQ.type = Common::EVENT_VIRTUAL_KEYBOARD;
	g_system->getEventManager()->pushEvent(eventQ);
}

int emScreenWidth = 0;
int emScreenHeight = 0;

extern "C" void emSetScreenSize(int width, int height){
	printf("## Set size: %d, %d\n", width, height);
	emScreenWidth = width;
	emScreenHeight = height;
}

extern "C" int emPing() {
	return 1;
}

#endif
