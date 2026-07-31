//===============================
// Fichier : Menu.h
// Description : Implémentation de la classe Menu pour gérer l'interface de jeu (UI and UX)
// Auteur : Aboubacar Sanogo
// Version : 1.0
//===============================

#pragma once

//===============================
// Inclusion des bibliothèques
//===============================
#include <string>
#include <vector>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

// Énumération pour les états du jeu
enum class GameState {
	Menu,
	PauseMenu,
	Playing
};

// Déclaration anticipée de la classe Menu
class Menu;

// Structure pour stocker les éléments du bouton
struct MenuButton {
	sf::RectangleShape background;
	sf::Text text;

	// Si ce bouton mène à un autre menu
	Menu* subMenu;

	// Si ce bouton exécute une action (ex: "PLAY", "QUIT", "FULLSCREEN")
	std::string action = "";

	MenuButton(const sf::Font& font, Menu* sub = nullptr) : text(font), subMenu(sub) {}
};

// Structure pour stocker les ratios pour le menu
struct MenuRatios {
	float titleSizeRatio = 0;
	float positionRatioX = 2.f;
	float positionRatioY = 18.f;
	float bWidthRatio = 30.f;
	float bHeightRatio = 7.f;
	float bSpacingRatio = 2.f;
	float headerWidthRatio = 4.f;
	float headerHeightRatio = 5.f;
	float txtSizeRatio = 0;
};

//===============================
// Police principale du jeu
//===============================
const std::string PressStart2P = "Assets/PressStart2P-Regular.ttf";

//===============================
// Constantes de couleurs
//===============================
const sf::Color WHITE_LESS(224, 224, 224);
const sf::Color SUBTLE_WHITE(255, 255, 255, 38); 
const sf::Color SUBTLE_BLACK(0, 0, 0, 38);
const sf::Color WHITE_GRAY(86, 86, 86);
const sf::Color LessTransparent(0, 0, 0, 128);

class Menu {
public:
	//===============================
	// Constructeur pour l'interface
	//===============================
	Menu(const std::string& fontPath, const std::string& title, int titleSize, sf::Color hColor = sf::Color::White,
		float positionRatioX = 2.f, float positionRatioY = 55.f,
		float bWidthRatio = 30.f, float bHeightRatio = 7.f, float bSpacingRatio = 2.f,
		float headerWidthRatio = 4.f, float headerHeightRatio = 5.f,
		unsigned int wWidth = 1440, unsigned int wHeight = 864);

	//==============================
	// Fonction pour initialiser la couleur des boutons (texte et background)
	//==============================
	void setColors(sf::Color bgNormal, sf::Color txtNormal, sf::Color bgHover, sf::Color txtHover);

	//==============================
	// Fonction pour ajouter des boutons
	//==============================
	void addButton(const std::string& label, const std::string& action, Menu* subMenu = nullptr, const float txtSizeRatio = 1.8f);

	//==============================
	// Fonction pour mettre à jour les boutons sélectionner
	//==============================
	void updateVisuals();

	//==============================
	// Fonction pour gérer les boutons sélectionner par la souris ou le clavier
	//==============================
	std::string handleEvent(const sf::Event& event, const sf::RenderWindow& window, Menu*& currentActiveMenu);

	//==============================
	// Fonction pour activer le "plein écran"
	//==============================
	void toggleFullScreen(bool& isFullScreen, sf::RenderWindow& window, std::vector<Menu*>& allMenus);

	//==============================
	// Fonction pour mettre à jour la taille d'un menu
	//==============================
	void updateLayout(unsigned int& newWWidth, unsigned int& newWHeight);

	//==============================
	// Fonction pour dessiner l'arrière plan du menu
	//==============================
	void drawMenuBackground(sf::Color& color, sf::RenderWindow& window);

	//==============================
	// Fonction pour dessiner l'arrière plan du menu avec une image
	//==============================
	void drawMenuImageBg(std::string& backgroundImage, sf::RenderWindow& window);

	//==============================
	// Fonction pour dessiner le menu
	//==============================
	void draw(sf::RenderWindow& window);

private:
	unsigned int windowWidth;			// Largeur de la fenêtre
	unsigned windowHeight;				// Hauteur de la fenêtre

	sf::Font font;						// Police du menu

	sf::Text menuHeader;				// En-tête
	sf::Color menuHeaderColor;			// Couleur de l'en-tête

	sf::Color backgroundColor;			// Couleur arrière du bouton
	sf::Color textColor;				// Couleur du texte du bouton
	sf::Color backgroundHover;			// Couleur arrière du bouton (Hover)
	sf::Color textHover;				// Couleur du texte du bouton (Hover)

	std::vector<MenuButton> menu;		// Vecteur pour les différents bouton du menu
	int selectedIndex = 0;				// Index sélectionner par l'utilisateur

	float startX;						// Position en X du menu
	float startY;						// Position en Y du menu
	float buttonWidth;					// Largeur d'un bouton
	float buttonHeight;					// Hauteur d'un bouton
	float spacing;						// Espace entre chaque bouton

	MenuRatios menuRatios;				// Donncées de ratios pour redimensionner le menu

	Menu* previousActiveMenu;			// Menu précédent pour pouvoir revenir en arrière
};
