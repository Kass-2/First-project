//====================================
// Fichier : Camera.cpp
// Description : Implémentation de la classe Camera pour gérer les différentes caméras du jeu
// Auteur : Aboubacar Sanogo
// Version : 1.0
//====================================

#pragma once

//===============================
// Inclusion des bibliothèques
//===============================
# include "GameHeaders.h"
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "Player.h"

//===============================
// Appliquer la vue de la caméra à l'interface utilisateur (UI)
//===============================
void applyCameraUI(sf::RenderWindow& window);

class Camera {
public:
	//===============================
	// Constructeur de la caméra avec ou sans zoom
	//===============================
	Camera(int width, int height, int map_width, int map_height, int tile_size, float zoom = 1.0f);

	//===============================
	// Changer la taille de la caméra
	//===============================
	void updateSize(float width, float height, float zoom);

	//===============================
	// Mise à jour de la caméra (Clamping)
	//===============================
	void clamped(const Player& player);

	//===============================
	// Mettre à jour la position de la caméra pour suivre le joueur
	//===============================
	void follow(const Player& player, float dt);

	/*void follow(const Player& player);*/

	//===============================
	// Appliquer la vue de la caméra à la fenêtre
	//===============================
	void apply(sf::RenderWindow& window);

private:
	//===============================
	// Gestion de la caméra pour suivre le joueur
	//===============================
	sf::View camera;

	int MAP_WIDTH = 0;
	int MAP_HEIGHT = 0;
	int TILE_SIZE = 0;
};