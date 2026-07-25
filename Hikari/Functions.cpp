//===============================
// Fichier : Functions.cpp
// Description : Implémentation des fonctions qui vont être utiliser dans le jeu.
// Auteur : Aboubacar Sanogo
// Version : 1.0
//===============================

//===============================
// Inclusion des bibliothèques nécessaires
//===============================
#include "Functions.h"

int calculateSize(int baseSize, int baseWindowWidth, int baseWindowHeight, int currentWindowWidth, int currentWindowHeight) {
	float PX = static_cast<float>(baseSize) / baseWindowWidth;	// Pourcentage de la position X de l'en-tête par rapport à la largeur de la fenêtre (960)
	float PY = static_cast<float>(baseSize) / baseWindowHeight;	// Pourcentage de la position Y de l'en-tête par rapport à la hauteur de la fenêtre (576)

	float sizeX = currentWindowWidth * PX;	// Taille de la position X de l'en-tête par rapport à la largeur de la fenêtre (960)
	float sizeY = currentWindowHeight * PY;	// Taille de la position Y de l'en-tête par rapport à la hauteur de la fenêtre (576)

	return int(((sizeX + sizeY) / 2));
}

float calculateSizeX(float baseSize, int baseWindowWidth, int currentWindowWidth) {
	float PX = baseSize / baseWindowWidth;	// Pourcentage de la position X de l'en-tête par rapport à la largeur de la fenêtre (960)

	float sizeX = currentWindowWidth * PX;	// Taille de la position X de l'en-tête par rapport à la largeur de la fenêtre (960)

	return sizeX;
}

float calculateSizeY(float baseSize, int baseWindowHeight, int currentWindowHeight) {
	float PY = baseSize / baseWindowHeight;	// Pourcentage de la position Y de l'en-tête par rapport à la hauteur de la fenêtre (576)

	float sizeY = currentWindowHeight * PY;	// Taille de la position Y de l'en-tête par rapport à la hauteur de la fenêtre (576)

	return sizeY;
}

float calculatePositionX(float positionX, int oldWindowWidth, int currentWindowWidth) {
	float PX = positionX / oldWindowWidth;	// Pourcentage de la position X de l'en-tête par rapport à la largeur de la fenêtre (960)

	float posX = currentWindowWidth * PX;	// Taille de la position X de l'en-tête par rapport à la largeur de la fenêtre (960)

	return posX;
}

float calculatePositionY(float positionY, int oldWindowHeight, int currentWindowHeight) {
	float PY = positionY / oldWindowHeight;	// Pourcentage de la position Y de l'en-tête par rapport à la hauteur de la fenêtre (576)

	float posY = currentWindowHeight * PY;	// Taille de la position Y de l'en-tête par rapport à la hauteur de la fenêtre (576)

	return posY;
}

float calculateValue(int windowWidth, float ratio) {
	return windowWidth * (ratio / 100);
}

float calculateValue(int windowWidth, int windowHeight, float ratio) {
	float a = windowWidth * (ratio / 100);
	float b = windowHeight * (ratio / 100);

	return (a + b) / 2;
}
