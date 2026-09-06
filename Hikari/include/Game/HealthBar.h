//===============================
// Fichier : HealthBar.cpp
// Description : Implémentation de la classe HealthBar pour gérer la barre de vie du joueur
// Auteur : Aboubacar Sanogo
// Version : 1.0
//===============================

#pragma once

//===============================
// Inclusion des bibliothèques
//===============================
#include <SFML/Graphics.hpp>
#include <string>

class HealthBar {
public:
	//===============================
	// Constructeur de la classe HealthBar
	//===============================
    HealthBar(const std::string& containerPath, int containerWidth, int
        containerHeight, int containerFrames,
        const std::string& firePath, int fireWidth, int fireHeight,
        int fireFrames,
        float frameTime = 0.1f);

	//===============================
	// Fonction pour mettre à jour l'état de la barre de vie en fonction de la santé actuelle et maximale du joueur
	//===============================
    void update(float currentHealth, float maxHealth, float dt);

	//===============================
	// Fonction pour dessiner la barre de vie à l'écran
	//===============================
    void draw(sf::RenderWindow& window, const sf::Vector2f& position);

private:
	sf::Texture containerTexture;   // Texture pour le conteneur de la barre de vie
	sf::Texture fireTexture;        // Texture pour l'animation du feu de la barre de vie

	sf::Sprite containerSprite;     // Sprite pour le conteneur de la barre de vie
    sf::Sprite fireSprite;          // Sprite pour l'animation du feu de la barre de vie

	int cW;                         // Largeur d'une frame du conteneur
	int cH;                         // Hauteur d'une frame du conteneur
    int containerTotalFrames;       // Nombre total de frames dans l'animation du conteneur
	int containerCurrentFrame;      // Frame actuelle de l'animation du conteneur

	int fW;                         // Largeur d'une frame de l'animation du feu
	int fH;                         // Hauteur d'une frame de l'animation du feu
	int fireTotalFrames;            // Nombre total de frames dans l'animation du feu
	int fireCurrentFrame;           // Frame actuelle de l'animation du feu

	float frameTime;                // Temps entre chaque frame de l'animation
	float animationTimer;           // Timer pour gérer le temps écoulé depuis la dernière mise à jour de l'animation
	float yOffset = 0.f;            // Décalage vertical pour positionner le feu au-dessus du conteneur
};
