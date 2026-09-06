//===============================
// Fichier : Camera.cpp
// Description : Implémentation de la classe Camera pour gérer les différentes caméras du jeu
// Auteur : Aboubacar Sanogo
// Version : 1.0
//===============================

//===============================
// Inclusion des bibliothèques nécessaires
//===============================
#include <algorithm>

#include <SFML/Graphics/RenderWindow.hpp>

#include "Camera.h"



Camera::Camera(int width, int height, 
	int map_width, int map_height, 
	int tile_size, float zoom) {
	// Créer une vue avec les dimensions de la fenêtre et appliquer un zoom
	sf::View c(sf::FloatRect({ 0.f, 0.f }, { (float)width, (float)height }));
	camera = c;
	camera.zoom(zoom);

	// Initialiser les dimensions de la carte et la taille des tuiles
	MAP_WIDTH = map_width;
	MAP_HEIGHT = map_height;
	TILE_SIZE = tile_size;
}



void Camera::updateSize(float width, float height, float zoom) {
    camera.setSize({ width, height });
    camera.zoom(zoom);
}



void Camera::clamped(const Player& player) {
    sf::Vector2f playerPos = player.getPosition();

    float halfWidth = camera.getSize().x / 2.f;
    float halfHeight = camera.getSize().y / 2.f;

    float mapWidthPixels = float(MAP_WIDTH * TILE_SIZE);
    float mapHeightPixels = float(MAP_HEIGHT * TILE_SIZE);

    float clampedX = playerPos.x;
    float clampedY = playerPos.y;

    // Clamp on X axis only if the map is wider than the camera view
    if (mapWidthPixels > camera.getSize().x) {
        float minX = halfWidth;
        float maxX = mapWidthPixels - halfWidth;
        clampedX = std::clamp(playerPos.x, minX, maxX);
    }
    else {
        // Map is smaller than camera: center the camera on the map
        clampedX = mapWidthPixels / 2.f;
    }

    // Clamp on Y axis only if the map is taller than the camera view
    if (mapHeightPixels > camera.getSize().y) {
        float minY = halfHeight;
        float maxY = mapHeightPixels - halfHeight;
        clampedY = std::clamp(playerPos.y, minY, maxY);
    }
    else {
        // Map is smaller than camera: center the camera on the map
        clampedY = mapHeightPixels / 2.f;
    }

    camera.setCenter({ clampedX, clampedY });
}



void Camera::follow(const Player& player, float dt) {
    sf::Vector2f target = player.getPosition();
    sf::Vector2f current = camera.getCenter();
    sf::Vector2f newCenter = current + (target - current) * 10.f * dt; // 10.f = camera smoothness
    camera.setCenter(newCenter);
}

//void Camera::follow(const Player& player) {
//	camera.setCenter(player.getPosition());
//}



void Camera::apply(sf::RenderWindow& window) {
	window.setView(camera);
}



void applyCameraUI(sf::RenderWindow& window) {
	// Réinitialiser la vue pour les éléments de l'interface utilisateur (UI)
	window.setView(window.getDefaultView());

	// TODO : Présentement, elle applique le UI, mais il faudra que je la reconfigure.
	// Je pourrais même créer une classe pour le UI.
}
