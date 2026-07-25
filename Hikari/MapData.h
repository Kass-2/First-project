//====================================
// Fichier : mapData.cpp
// Description : Implémentation de la fonction pour charger un fichier CSV
// Auteur : Aboubacar Sanogo
// Version : 1.0
//====================================

#pragma once

//===============================
// Inclusion des bibliothèques
//===============================
#include <string>
#include <vector>

//===============================
// Constantes pour la taille des tuiles et les dimensions de la carte
//===============================
extern unsigned int windowWidth;
extern unsigned int windowHeight;
const int TILE_SIZE = 32;

//const int MAP_WIDTH = windowWidth / TILE_SIZE;   // Nombre de tuiles en largeur
//const int MAP_HEIGHT = windowHeight / TILE_SIZE; // Nombre de tuiles en hauteur
const int MAP_WIDTH = 30;   // Nombre de tuiles en largeur
const int MAP_HEIGHT = 18;	// Nombre de tuiles en hauteur
const int MAX_TILES = MAP_WIDTH * MAP_HEIGHT;     // Nombre total de tuiles dans la carte

//===============================
// Fonction pour charger une carte de tuiles à partir d'un fichier CSV
//================================
bool loadCSV(const std::string& filename, std::vector<int>& map, int& width, int& height);

//===============================
// Fonction pour copier les données de la carte chargée dans un tableau 1D
//================================
void loadMap(int* map, const std::vector<int>& mapData, const int MAX_TILES);


inline unsigned int getWindowWidth() {
	return windowWidth;
}

inline unsigned int getWindowHeight() {
	return windowHeight;
}