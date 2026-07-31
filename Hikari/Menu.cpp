//===============================
// Fichier : Menu.cpp
// Description : Implémentation de la classe Menu pour gérer l'interface de jeu (UI and UX)
// Auteur : Aboubacar Sanogo
// Version : 1.0
//===============================

//===============================
// Inclusion des bibliothèques nécessaires
//===============================
#include <iostream>
#include <vector>
#include <string>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "Menu.h"
#include "Functions.h"
#include "MapData.h"



Menu::Menu(const std::string& fontPath, const std::string& title, int titleSize, sf::Color hColor,
	float positionRatioX, float positionRatioY,
	float bWidthRatio, float bHeightRatio, float bSpacingRatio,
	float headerWidthRatio, float headerHeightRatio,
	unsigned int wWidth, unsigned int wHeight)
	: windowWidth(wWidth), windowHeight(wHeight), menuHeader(font), menuHeaderColor(hColor), previousActiveMenu(nullptr)
{
	if (!font.openFromFile(fontPath)) {
		std::cerr << "Erreur lors du chargement de la police: " << fontPath << std::endl;
	}

	menuRatios.titleSizeRatio = (float)titleSize;
	menuRatios.positionRatioX = positionRatioX;
	menuRatios.positionRatioY = positionRatioY;
	menuRatios.bWidthRatio = bWidthRatio;
	menuRatios.bHeightRatio = bHeightRatio;
	menuRatios.bSpacingRatio = bSpacingRatio;
	menuRatios.headerWidthRatio = headerWidthRatio;
	menuRatios.headerHeightRatio = headerHeightRatio;

	menuHeader.setFont(font);
	menuHeader.setString(title);

	int initialHSize = calculateSize((float)titleSize, 960, 576, windowWidth, windowHeight);
	menuHeader.setCharacterSize(initialHSize);
	menuHeader.setFillColor(menuHeaderColor);

	// Valeur pour le positionnement et la taille des boutons du menu
	startX = calculateValue(windowWidth, positionRatioX);
	startY = calculateValue(windowHeight, positionRatioY);
	buttonWidth = calculateValue(windowWidth, bWidthRatio);
	buttonHeight = calculateValue(windowHeight, bHeightRatio);
	spacing = calculateValue(windowHeight, bSpacingRatio);

	// Position de l'étiquette du menu
	float headerX = calculateValue(windowWidth, headerWidthRatio);
	float headerY = calculateValue(windowHeight, headerHeightRatio);
	menuHeader.setPosition({ headerX, headerY});
}



void Menu::setColors(sf::Color bgNormal, sf::Color txtNormal, sf::Color bgHover, sf::Color txtHover) {
	backgroundColor = bgNormal;
	textColor = txtNormal;
	backgroundHover = bgHover;
	textHover = txtHover;
}



void Menu::addButton(const std::string& label, const std::string& action, Menu* subMenu, const float txtSizeRatio) {
	MenuButton button(font, subMenu);
	button.action = action;

	// Garder l'acinne donnée de ratio
	menuRatios.txtSizeRatio = txtSizeRatio;

	float index = (float)menu.size();

	// Configuration de base du bouton
	button.background.setSize({ buttonWidth, buttonHeight });
	button.background.setPosition({ startX, startY + index * (buttonHeight + spacing) });

	button.text.setFont(font);
	button.text.setString(label);

	// Taille de la police du texte à l'intérieur du bouton
	int txtSize = (int)calculateValue(windowWidth, windowHeight, txtSizeRatio);

	button.text.setCharacterSize(txtSize);
	button.text.setPosition({ startX + calculateValue((int)buttonWidth, 7.f), startY + index * (buttonHeight + spacing) + calculateValue((int)buttonHeight, 35.f) });

	menu.push_back(button);

	// Mettre à jour l'apparence du boutton
	updateVisuals();
}



void Menu::updateVisuals() {
	for (size_t i = 0; i < menu.size(); ++i) {
		if (i == selectedIndex) {
			menu[i].background.setFillColor(backgroundHover);
			menu[i].text.setFillColor(textHover);
		}
		else {
			menu[i].background.setFillColor(backgroundColor);
			menu[i].text.setFillColor(textColor);
		}
	}
}



std::string Menu::handleEvent(const sf::Event& event, const sf::RenderWindow& window, Menu*& currentActiveMenu) {
	previousActiveMenu = currentActiveMenu;
	// Navigation Souris (Survol)
	if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
		sf::Vector2f mousePos = window.mapPixelToCoords(mouseMoved->position);
		for (size_t i = 0; i < menu.size(); ++i) {
			if (menu[i].background.getGlobalBounds().contains(mousePos)) {
				if (selectedIndex != i) {
					selectedIndex = (int)i;
					updateVisuals();
				}
			}
		}
	}

	// Clic Souris
	if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
		if (mousePressed->button == sf::Mouse::Button::Left) {
			sf::Vector2f mousePos = window.mapPixelToCoords(mousePressed->position);
			if (menu[selectedIndex].background.getGlobalBounds().contains(mousePos)) {
				if (menu[selectedIndex].subMenu != nullptr) {
					currentActiveMenu = menu[selectedIndex].subMenu; // Changement de menu !
					return "";
				}
				previousActiveMenu = nullptr;
				return menu[selectedIndex].action; // Retourne l'action ("PLAY", "QUIT", etc.)
			}
		}
	}

	// Navigation Clavier
	if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
		if (keyPressed->code == sf::Keyboard::Key::Up) {
			if (selectedIndex > 0) { 
				selectedIndex--; 
				updateVisuals();
			}
		}
		else if (keyPressed->code == sf::Keyboard::Key::Down) {
			if (selectedIndex < menu.size() - 1) { 
				selectedIndex++; 
				updateVisuals(); 
			}
		}
		else if (keyPressed->code == sf::Keyboard::Key::Enter) {
			if (menu[selectedIndex].subMenu != nullptr) {
				currentActiveMenu = menu[selectedIndex].subMenu; // Changement de menu !
				return "";
			}
			previousActiveMenu = nullptr;
			return menu[selectedIndex].action; // Retourne l'action
		}
		else if (keyPressed->code == sf::Keyboard::Key::Escape) {
			if (previousActiveMenu != nullptr) {
				currentActiveMenu = previousActiveMenu;
				return "";
			}
			return "ESCAPE"; // Pour reculer d'un menu
		}
	}
	return ""; // Aucune action spécifique n'a été déclenchée
}



void Menu::toggleFullScreen(bool& isFullScreen, sf::RenderWindow& window, std::vector<Menu*>& allMenus) {
	isFullScreen = !isFullScreen;
	if (isFullScreen) {
		window.create(sf::VideoMode::getDesktopMode(), "Hikari",
			sf::Style::Default, sf::State::Fullscreen);
	}
	else {
		window.create(sf::VideoMode({ getWindowWidth(), getWindowHeight() }), "Hikari",
			sf::Style::Default, sf::State::Windowed);
	}
	window.setFramerateLimit(60);

	// Retrieve the actual new window dimensions
	windowWidth = window.getSize().x;
	windowHeight = window.getSize().y;

	// Mettre à jour les menus passés en paramètre
	for (Menu* m : allMenus) {
		m->updateLayout(windowWidth, windowHeight);
	}
}



void Menu::updateLayout(unsigned int& newWWidth, unsigned int& newWHeight) {
	windowWidth = newWWidth;
	windowHeight = newWHeight;

	// Recalcule les variables de base
	startX = calculateValue(windowWidth, menuRatios.positionRatioX);
	startY = calculateValue(windowHeight, menuRatios.positionRatioY);
	buttonWidth = calculateValue(windowWidth, menuRatios.bWidthRatio);
	buttonHeight = calculateValue(windowHeight, menuRatios.bHeightRatio);
	spacing = calculateValue(windowHeight, menuRatios.bSpacingRatio);

	// Nouvelle taille du texte de l'en-tête
	int newHeaderTxtSize = calculateSize(menuRatios.titleSizeRatio, 960, 576, (int)windowWidth, (int)windowHeight);
	menuHeader.setCharacterSize(newHeaderTxtSize);

	// Position de l'étiquette du menu
	float headerX = calculateValue(windowWidth, menuRatios.headerWidthRatio);
	float headerY = calculateValue(windowHeight, menuRatios.headerHeightRatio);
	menuHeader.setPosition({ headerX, headerY });

	// Repositionner tous les boutons de ce menu
	for (size_t i = 0; i < menu.size(); ++i) { // "menu" est ton vecteur de MenuButton
		menu[i].background.setSize({ buttonWidth, buttonHeight });
		menu[i].background.setPosition({ startX, startY + i * (buttonHeight + spacing) });

		// Taille de la police du texte à l'intérieur du bouton
		int txtSize = (int)calculateValue(windowWidth, windowHeight, menuRatios.txtSizeRatio);
		
		menu[i].text.setCharacterSize(txtSize);
		menu[i].text.setPosition({ startX + calculateValue((int)buttonWidth, 7.f), startY + i * (buttonHeight + spacing) + calculateValue((int)buttonHeight, 35.f) });
	}
}



void Menu::drawMenuBackground(sf::Color& backgroundColor, sf::RenderWindow& window) {
	// Dessiner un background pour le menu
	sf::RectangleShape menuBackground(sf::Vector2f((float)windowWidth, (float)windowHeight));
	menuBackground.setFillColor(backgroundColor);

	window.draw(menuBackground);
}



void Menu::drawMenuImageBg(std::string& backgroundImage, sf::RenderWindow& window) {
	// Dessiner une image de fond pour le menu
	sf::Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile(backgroundImage)) {
		std::cerr << "Erreur lors du chargement de l'image de fond" << std::endl;
	}
	sf::Sprite backgroundSprite(backgroundTexture);

	window.draw(backgroundSprite);
}



void Menu::draw(sf::RenderWindow& window) {
	window.draw(menuHeader);
	for (const auto& button : menu) {
		window.draw(button.background);
		window.draw(button.text);
	}
}
