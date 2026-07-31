//===============================
// Fichier : main.cpp
// Description : Point d'entrée du jeu, création de la fenêtre, boucle principale du jeu
// Auteur : Aboubacar Sanogo
// Version : 1.0
//===============================

//===============================
// Inclusion des bibliothèques nécessaires
//===============================
// Librairies standard
#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
#include <string>

// Librairies SFML
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics.hpp>
#include <optional>
#include <SFML/System/Clock.hpp>

// Librairies du jeu
#include "Player.h"
#include "MapData.h"
#include "MiniMap.h"
#include "HealthBar.h"
#include "Camera.h"
#include "HitBox.h"
#include "Menu.h"
#include "TileMap.h"



// Structure pour stocker les éléments à rendre avec leur position Y
struct RenderItem {
	float y;
	std::function<void()> drawCall;
};



int main()
{
	//===============================
	// Creation de la fenêtre de jeu
	//===============================
    sf::RenderWindow window(sf::VideoMode({ windowWidth, windowHeight }), "Hikari");
	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);
	//window.setVerticalSyncEnabled(true);

	//===============================
	// Initialisation des menus différents
	//===============================
	std::vector<Menu*> allMenus;
	
	// Menu principal
	Menu mainMenu(PressStart2P, "HIKARI", 70);
	// Menu de paramètres (Menu principal)
	Menu mainMenuSettings(PressStart2P, "SETTINGS", 70);
	// Menu de pause
	Menu pauseMenu(PressStart2P, "PAUSE MENU", 40, sf::Color::White, 2.f, 40.f);
	// Menu de paramètres (Menu principal)
	Menu pauseMenuSettings(PressStart2P, "SETTINGS", 40, sf::Color::White, 2.f, 40.f);

	// Couleurs pour les menus
	mainMenu.setColors(sf::Color::Transparent, sf::Color::Black, LessTransparent, sf::Color::White);
	mainMenuSettings.setColors(sf::Color::Transparent, sf::Color::Black, LessTransparent, sf::Color::White);
	pauseMenu.setColors(sf::Color::Transparent, sf::Color::Black, LessTransparent, sf::Color::White);
	pauseMenuSettings.setColors(sf::Color::Transparent, sf::Color::Black, LessTransparent, sf::Color::White);


	//// Vecteurs pour les étiquettes du menu principal
	//std::vector<std::string> mainMenuLabels = {
	//	// Pour le menu principal
	//	"PLAY", "SETTINGS", "CREDITS", "QUIT",
	//	// Pour les paramètres du menu principal
	//	"TOGGLE FULLSCREEN", "BACK"
	//};

	//// Vecteurs pour les étiquettes du menu de pause
	//std::vector<std::string> pauseMenuLabels = {
	//	// Pour le menu de pause
	//	"RESUME", "SETTINGS", "EXIT TO MAIN MENU",
	//	// Pour les paramètres du menu principal
	//	"TOGGLE FULLSCREEN", "BACK"
	//};


	// Ajouter les boutons des menus
	mainMenu.addButton("PLAY", "PLAY");
	mainMenu.addButton("SETTINGS", "", &mainMenuSettings);
	mainMenu.addButton("CREDITS", "CREDITS");
	mainMenu.addButton("QUIT", "QUIT");

	mainMenuSettings.addButton("TOGGLE FULLSCREEN", "FULLSCREEN");
	mainMenuSettings.addButton("BACK", "", &mainMenu);

	pauseMenu.addButton("RESUME", "RESUME");
	pauseMenu.addButton("SETTINGS", "", &pauseMenuSettings);
	pauseMenu.addButton("EXIT TO MAIN MENU", "MAIN MENU");

	pauseMenuSettings.addButton("TOGGLE FULLSCREEN", "FULLSCREEN");
	pauseMenuSettings.addButton("BACK", "", &pauseMenu);

	// Ajouter les menus au vecteur pour faciliter le redimensionnement
	allMenus.push_back(&mainMenu);
	allMenus.push_back(&mainMenuSettings);
	allMenus.push_back(&pauseMenu);
	allMenus.push_back(&pauseMenuSettings);

	// Menu actif présentement
	Menu* activeMenu = &mainMenu;
	
	//===============================
	// Création du sprite et de la texture du joueur
	//===============================
	Player playerCharacter("Assets/adventurerFull.png", 96, 80);
	playerCharacter.initialState("Sid");
	
	//===============================
	// Création du sprite et de la texture du personnage de test
	//===============================
	Player testCharacter("Assets/adventurerFull.png", 96, 80);
	testCharacter.initialState("Test");
	testCharacter.setPlayerPosition(200.f, 200.f);

	// Verrouiller le mouvement du personnage de test pour éviter qu'il ne se déplace pendant les tests
	testCharacter.LockPlayer();

	//===============================
	// Création de la caméra
	//===============================
	Camera playerCamera(windowWidth, windowHeight, MAP_WIDTH, MAP_HEIGHT, TILE_SIZE, 0.7f);

	//=================================
	// Création de la barre de vie du joueur
	//=================================
	HealthBar playerHealthBar("Assets/healthBarContainer.png", 32, 48, 8, "Assets/fireHealthBar.png", 32, 48, 8, 0.1f);

	//===============================
	// Création de la mini-carte
	//===============================
	MiniMap miniMap((float)(MAP_WIDTH),
					(float)(MAP_HEIGHT),
					(float)windowWidth,
					(float)windowHeight, TILE_SIZE);

	//===============================
	// Création de la carte de tuiles
	//===============================
	TileMap tileMap(MAP_WIDTH, MAP_HEIGHT);

	int map[MAX_TILES] = { 0 };
	int map_width = MAP_WIDTH;
	int map_height = MAP_HEIGHT;

	std::vector<int> mapData;

	// Charger les données de la carte à partir du fichier CSV
	loadCSV("Assets/InitialMap.csv", mapData, map_width, map_height);

	// Copier les données de la carte chargée dans le tableau 1D
	loadMap(map, mapData, MAX_TILES);

	// Charger la map avec ces nouvelles dimensions
	if (!tileMap.load("Assets/Tileset_Grass.png", sf::Vector2u(32, 32), map))
	{
		std::cerr << "Failed to load tile map!" << std::endl;
		return -1;
	}

	//===============================
	// Variables pour la boucle principale du jeu
	//===============================
	std::vector<Player> players;

	// Horloge pour gérer le temps écoulé entre les mises à jour
	sf::Clock deltaClock;

	// Variable pour afficher ou non les hitboxes des personnages
	bool showHitboxes = false;

	// Variable pour gérer l'état du jeu (menu de pause ou en cours de jeu)
	GameState currentGameState = GameState::Playing;

	bool isFullscreen = false;

	//===============================
	// Loop principal du jeu
	//===============================
    while (window.isOpen())
    {
		// Delta time pour gérer...
		float dt = deltaClock.restart().asSeconds();

		if (dt > 0.1f) {
			dt = 0.1f;
		}

		//===============================
		// Gestion des événements de la fenêtre
		//===============================
		while (auto event = window.pollEvent()) 
		{
			if (event->is<sf::Event::Closed>())
				window.close();

			switch (currentGameState) {
				// --- MAIN MENU CONTROLS ---
				case GameState::Menu:
				{
					std::string action = activeMenu->handleEvent(*event, window, activeMenu);

					if (action == "PLAY") {
						currentGameState = GameState::Playing;
					}
					else if (action == "FULLSCREEN") {
						activeMenu->toggleFullScreen(isFullscreen, window, allMenus);

						// Update Camera and Minimap dimensions
						playerCamera.updateSize((float)window.getSize().x, (float)window.getSize().y, 604.8f / window.getSize().y);
						miniMap.updateWindowSize((float)window.getSize().x, (float)window.getSize().y);
					}
					else if (action == "CREDITS") {
						std::cout << "Nothing yet" << std::endl;
					}
					else if (action == "QUIT") {
						window.close();
					}
					break;
				}
				// --- PAUSE MENU CONTROLS ---
				case GameState::PauseMenu:
				{
					std::string action = activeMenu->handleEvent(*event, window, activeMenu);

					if (action == "RESUME" || action == "ESCAPE") {
						currentGameState = GameState::Playing;
					}
					else if (action == "FULLSCREEN") {
						activeMenu->toggleFullScreen(isFullscreen, window, allMenus);

						// Update Camera and Minimap dimensions
						playerCamera.updateSize((float)window.getSize().x, (float)window.getSize().y, 604.8f / window.getSize().y);
						miniMap.updateWindowSize((float)window.getSize().x, (float)window.getSize().y);
					}
					else if (action == "MAIN MENU") {
						currentGameState = GameState::Menu;
						activeMenu = &mainMenu; // On s'assure de revenir au Main Menu
					}
					break;
				}
				// --- GAMEPLAY CONTROLS ---
				case GameState::Playing:
				{
					if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
						if (keyPressed->code == sf::Keyboard::Key::Escape) {
							// Arreter le jeu et passer au menu de pause
							currentGameState = GameState::PauseMenu;
							activeMenu = &pauseMenu;

							// Réinitialiser les entrées du joueur pour éviter les mouvements pendant le menu de pause
							playerCharacter.resetInputs();
						}
					}

					if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
						if (keyPressed->code == sf::Keyboard::Key::H &&
							keyPressed->shift &&
							keyPressed->control) {
							showHitboxes = !showHitboxes;
						}
					}

					/*for (int i = 0; i < players.size(); ++i) {
						players[i].handleEvent(*event);
					}*/

					miniMap.handleEvent(*event);

					// Pour éviter que le joueur ne se déplace lorsqu'il est dans le menu de pause
					playerCharacter.handleEvent(*event);
					testCharacter.handleEvent(*event);

					break;
				}
			}
		}

		sf::FloatRect playerHitbox;
		sf::FloatRect testHitbox;

		if (currentGameState == GameState::Playing) {
			playerCharacter.update(dt, window);
			testCharacter.update(dt, window);

			playerHealthBar.update(playerCharacter.getHealth(), playerCharacter.getMaxHealth(), dt);

			// Récupérer les hitboxes des deux personnages pour la détection de collision
			playerHitbox = playerCharacter.getHitbox();
			testHitbox = testCharacter.getHitbox();

			// Détection de collision
			resolveCollision(playerCharacter.getSprite(), playerHitbox, testHitbox);

			// Mettre à jour la position de la caméra pour suivre le joueur
			playerCamera.follow(playerCharacter);
			playerCamera.clamped(playerCharacter);
		}

		// Pour le rendu, on efface la fenêtre avec une couleur noire
        window.clear(sf::Color::Black);

		switch (currentGameState) {
			case GameState::Menu:
			{
				// Dessiner un background sombre pour le menu principal (Temporaire)
				sf::Color bgColor = sf::Color(58, 58, 58);

				activeMenu->drawMenuBackground(bgColor, window);
				activeMenu->draw(window);

				break;
			}
			// Pour le rendu, on vérifie si le jeu est en cours ou en pause pour dessiner les éléments du jeu
			case GameState::Playing:
			case GameState::PauseMenu:
			{
				// Appliquer la vue de la caméra à la fenêtre
				playerCamera.apply(window);

				window.draw(tileMap);

				std::vector<RenderItem> renderQueue;

				renderQueue.push_back({ playerCharacter.getSprite().getPosition().y, [&]()
					{ playerCharacter.draw(window); } });
				renderQueue.push_back({ testCharacter.getSprite().getPosition().y, [&]()
					{ testCharacter.draw(window); } });

				// TODO : Ajouter d'autres éléments à la file d'attente de rendu si nécessaire

				// Trier par la position Y des éléments
				std::sort(renderQueue.begin(), renderQueue.end(), [](const RenderItem& a, const RenderItem& b)
					{
						return a.y < b.y;
					});

				// Dessiner les éléments dans l'ordre trié
				for (const auto& item : renderQueue) {
					item.drawCall();
				}

				// Dessiner les hitboxes si l'option est activée
				if (showHitboxes) {
					drawHitboxOutline(window, playerHitbox);
					drawHitboxOutline(window, testHitbox);
				}

				// Reset view to default for UI rendering)
				window.setView(window.getDefaultView());

				playerHealthBar.draw(window, { 20.f, 20.f });

				miniMap.draw(window, tileMap, playerCharacter, testCharacter);

				applyCameraUI(window);

				if (currentGameState == GameState::PauseMenu) {
					// Draw a dark full-screen overlay to dim the game world
					sf::Color dimColor = sf::Color(0, 0, 0, 150);

					activeMenu->drawMenuBackground(dimColor, window);
					activeMenu->draw(window);
				}

				break;
			}
		}
        window.display();
    }
	return 0;
}
