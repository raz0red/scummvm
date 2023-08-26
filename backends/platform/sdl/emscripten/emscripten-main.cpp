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
#include <emscripten.h>
#include "backends/platform/sdl/emscripten/emscripten.h"
#include "backends/plugins/sdl/sdl-provider.h"
#include "base/main.h"

int main(int argc, char *argv[]) {

	// Create our OSystem instance
	g_system = new OSystem_Emscripten();
	assert(g_system);

	// Pre initialize the backend
	g_system->init();

#ifdef DYNAMIC_MODULES
	PluginManager::instance().addPluginProvider(new SDLPluginProvider());
#endif

	for(int i = 0; i < argc; i++) {
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
		window.emulator.getApp().exit();
	);

	return res;
}

extern "C" void emSetStretchMode(int mode) {
		printf("### Set stretch mode: %d\n", mode);
		g_system->beginGFXTransaction();
		g_system->setStretchMode(mode);
		g_system->endGFXTransaction();
}

bool emPaused = false;
extern "C" void emPause() {
	emPaused = true;
}

extern "C" void emUnpause() {
	emPaused = false;
}

extern "C" void emQuit() {
	Common::Event eventQ;
	eventQ.type = Common::EVENT_QUIT;
	g_system->getEventManager()->pushEvent(eventQ);
}

// Common::Error Engine::saveGameState(int slot, const Common::String &desc, bool isAutosave) {
extern "C" int saveGame() {
	//Common::Error
	auto err = g_engine->saveGameState(0, "foo", false);
	printf("## Save result: %d\n", err.getCode());
	return err.getCode();
}

extern "C" int loadGame() {
	//Common::Error
	auto err = g_engine->loadGameState(0);
	printf("## Load result: %d\n", err.getCode());
	return err.getCode();
}
#endif


#if 0
window.Module._emPause()
window.Module._saveGame();

// Loop until save shows up... need callback from scumm side
window.Module._emUnpause(); setTimeout(() => { Module._emPause(); }, 1);
just do async wait and loop, etc.
mark save pending
make call
loop and wait one ms. (unpause, wait one ms, pause)
at some point, scummvm calls back to indicate save worked (maybe pass filename as well?)
deal with save file
clear save pending

FS.readdir("/home/web_user/.local/share/scummvm/saves");
(3)['.', '..', 'tentacle.s00']

(done) need to deal w/ tab changing, pause and unpause
need to deal with touch controls
need to override keyboard input (menu, etc. change sizes, etc. etc.)

disable autosave
#endif
