//===============================
// Fichier : MiniMap.h
// Description : Déclaration de la classe MiniMap pour gérer la mini-carte
// Auteur : Aboubacar Sanogo
// Version : 1.0
//===============================

#pragma once

//===============================
// Inclusion des bibliothèques
//===============================
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "TileMap.h"

class MiniMap {
public:
	//===============================
	// Constructeur de la mini-carte
	//===============================
	MiniMap(float mapWidth, float mapHeight, float windowWidth, float windowHeight, int tileSize);

	//===============================
	// Changer la taille de la minimap
	//===============================
	void updateWindowSize(float windowWidth, float windowHeight);

	//===============================
	// Mise à jour de la mini-carte en fonction de la position du joueur
	//===============================
	void handleEvent(const sf::Event& event);

	//===============================
	// Dessin de la mini-carte à l'écran
	//===============================
	void draw(sf::RenderWindow& window, const TileMap& tileMap,
		const Player& player, const Player& testChar);

private:
	sf::View view;					// Vue pour la mini-carte
	sf::RectangleShape border;		// Bordure de la mini-carte
	sf::RectangleShape background;	// Fond de la mini-carte
	sf::CircleShape playerDot;		// Marqueur du joueur sur la mini-carte
	sf::CircleShape testCharDot;	// Marqueur du personnage de test sur la mini-carte
	bool miniMapVisible = true;	// Indique si la mini-carte est visible
};