/*
 * Copyright 2010-2016 OpenXcom Developers.
 *
 * This file is part of OpenXcom.
 *
 * OpenXcom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenXcom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenXcom.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "FundingState.h"
#include <sstream>
#include "../Engine/Game.h"
#include "../Mod/Mod.h"
#include "../Engine/LocalizedText.h"
#include "../Engine/Unicode.h"
#include "../Interface/TextButton.h"
#include "../Interface/Window.h"
#include "../Interface/Text.h"
#include "../Interface/TextList.h"
#include "../Savegame/Country.h"
#include "../Mod/RuleCountry.h"
#include "../Savegame/SavedGame.h"
#include "../Engine/Options.h"

namespace OpenXcom
{

/**
 * Initializes all the elements in the Funding screen.
 * @param game Pointer to the core game.
 */
FundingState::FundingState()
{
	_screen = false;

	// Create objects
	_window = new Window(this, 320, 200, 0, 0, POPUP_BOTH);
	_btnOk = new TextButton(50, 12, 135, 180);
	_txtTitle = new Text(320, 17, 0, 8);
	_txtCountry = new Text(100, 9, 32, 30);
	_txtFunding = new Text(100, 9, 140, 30);
	_txtChange = new Text(72, 9, 240, 30);
	_lstCountries = new TextList(260, 136, 32, 40);

	// Set palette
	setInterface("fundingWindow");

	add(_window, "window", "fundingWindow");
	add(_btnOk, "button", "fundingWindow");
	add(_txtTitle, "text1", "fundingWindow");
	add(_txtCountry, "text2", "fundingWindow");
	add(_txtFunding, "text2", "fundingWindow");
	add(_txtChange, "text2", "fundingWindow");
	add(_lstCountries, "list", "fundingWindow");

	centerAllSurfaces();

	// Set up objects
	setWindowBackground(_window, "fundingWindow");

	_btnOk->setText(tr("STR_OK"));
	_btnOk->onMouseClick((ActionHandler)&FundingState::btnOkClick);
	_btnOk->onKeyboardPress((ActionHandler)&FundingState::btnOkClick, Options::keyOk);
	_btnOk->onKeyboardPress((ActionHandler)&FundingState::btnOkClick, Options::keyCancel);
	_btnOk->onKeyboardPress((ActionHandler)&FundingState::btnOkClick, Options::keyGeoFunding);

	_txtTitle->setAlign(ALIGN_CENTER);
	_txtTitle->setBig();
	_txtTitle->setText(tr("STR_INTERNATIONAL_RELATIONS"));

	_txtCountry->setText(tr("STR_COUNTRY"));

	_txtFunding->setText(tr("STR_FUNDING"));

	_txtChange->setText(tr("STR_CHANGE"));

	_lstCountries->setColumns(3, 108, 100, 52);
	_lstCountries->setDot(true);

	if (!_game->getMod()->getNewBaseUnlockResearch().empty())
	{
		bool newBasesUnlocked = _game->getSavedGame()->isResearched(_game->getMod()->getNewBaseUnlockResearch(), true);
		if (!newBasesUnlocked)
		{
			_txtCountry->setVisible(false);
			_txtFunding->setVisible(false);
			_txtChange->setVisible(false);
			_lstCountries->setVisible(false);
		}
	}
	for (auto* country : *_game->getSavedGame()->getCountries())
	{
		std::ostringstream ss, ss2;
		ss << Unicode::TOK_COLOR_FLIP << Unicode::formatFunding(country->getFunding().at(country->getFunding().size()-1)) << Unicode::TOK_COLOR_FLIP;
		if (country->getFunding().size() > 1)
		{
			ss2 << Unicode::TOK_COLOR_FLIP;
			int change = country->getFunding().back() - country->getFunding().at(country->getFunding().size()-2);
			if (change > 0)
				ss2 << '+';
			ss2 << Unicode::formatFunding(change);
			ss2 << Unicode::TOK_COLOR_FLIP;
		}
		else
		{
			ss2 << Unicode::formatFunding(0);
		}
		_lstCountries->addRow(3, tr(country->getRules()->getType()).c_str(), ss.str().c_str(), ss2.str().c_str());
	}
	_lstCountries->addRow(2, tr("STR_TOTAL_UC").c_str(), Unicode::formatFunding(_game->getSavedGame()->getCountryFunding()).c_str());
	_lstCountries->setRowColor(_game->getSavedGame()->getCountries()->size(), _txtCountry->getColor());
}

/**
 *
 */
FundingState::~FundingState()
{

}

/**
 * Returns to the previous screen.
 * @param action Pointer to an action.
 */
void FundingState::btnOkClick(Action *)
{
	_game->popState();
}

}
