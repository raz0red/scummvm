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
#include "backends/plugins/sdl/sdl-provider.h"
#include "base/main.h"
#include <emscripten.h>

bool emPaused = false;
bool emShowScummMenu = false;

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

	printf("## Starting main.\n");

	// Invoke the actual ScummVM main entry point:
	int res = scummvm_main(argc, argv);

	printf("## After main.\n");

	// Free OSystem
	g_system->destroy();

	printf("## After destroy.\n");

	EM_ASM(
		window.emulator.onExit();
	);

	return res;
}

extern "C" void emSetStretchMode(int mode) {
	printf("### Set stretch mode: %d\n", mode);
	g_system->beginGFXTransaction();
	g_system->setStretchMode(mode);
	g_system->endGFXTransaction();
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

// Common::Error Engine::saveGameState(int slot, const Common::String &desc, bool isAutosave) {
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

#endif
