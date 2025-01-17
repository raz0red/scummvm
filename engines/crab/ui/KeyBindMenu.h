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

/*
 * This code is based on the CRAB engine
 *
 * Copyright (c) Arvind Raja Yadav
 *
 * Licensed under MIT
 *
 */

#ifndef CRAB_KEYBINDMENU_H
#define CRAB_KEYBINDMENU_H

#include "crab/ui/ImageData.h"
#include "crab/ui/menu.h"
#include "crab/ui/OptionSelect.h"

namespace Crab {

namespace pyrodactyl {
namespace ui {
class KeyBindMenu {
	// The keyboard controls menu has 2 types of inputs
	enum Controls {
		CON_GAME,
		CON_UI,
		CON_TOTAL
	};

	// Each menu can be in these 2 states
	enum States {
		STATE_NORMAL,
		STATE_KEY
	} _state;

	// This button swaps between sub-sections "Gameplay" and "Interface"
	OptionSelect _selControls;

	// These two buttons are the template buttons for the menu
	Button _prim, _alt;

	// This is the template text info
	TextData _desc;

	// inc tells us what to add to the reference buttons to get multiple buttons
	// Divide is the space between two columns
	Vector2i _inc, _divide;

	// The number of rows and columns
	Vector2i _dim;

	// The menu for the keyboard options in both sub categories
	// all control types have equal entries so we just need to change the text displayed
	ButtonMenu _menu[CON_TOTAL];

	// The selected button in the current menu
	int _choice;

	struct PromptInfo {
		int _col, _colPrev;
		Common::String _text;

		PromptInfo() {
			_col = 0;
			_colPrev = 0;
		}

		void load(rapidxml::xml_node<char> *node) {
			if (nodeValid(node)) {
				loadStr(_text, "text", node);
				loadNum(_col, "color", node);
			}
		}

		void Swap(Caption &c) {
			_colPrev = c._col;
			c._text = _text;
			c._col = _col;
		}
	} _prompt;

	void startAndSize(const int &type, int &start, int &size);
	void initMenu(const int &type);
	void drawDesc(const int &type);

public:
	KeyBindMenu() {
		reset();
		_choice = -1;
	}
	~KeyBindMenu() {}

	void reset() {
		_state = STATE_NORMAL;
	}

	bool disableHotkeys() {
		return _state != STATE_NORMAL;
	}

	void load(rapidxml::xml_node<char> *node);

	void handleEvents(const Common::Event &event);

	void setCaption();

	void draw();
	void setUI();
};
} // End of namespace ui
} // End of namespace pyrodactyl

} // End of namespace Crab

#endif // CRAB_KEYBINDMENU_H
