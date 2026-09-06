//====================================
// Fichier : tileMap.cpp
// Description : Implémentation de la classe TileMap pour gérer les cartes de tuiles
// Auteur : Aboubacar Sanogo
// Version : 1.0
//====================================

#pragma once

#include "GameHeaders.h"

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    // Default constructor
    TileMap(const unsigned int mapWidth, const unsigned int mapHeight);

    /**
     * @brief Loads a tileset texture and builds the triangle vertex array
     * @param tilesetPath Path to the texture image file
     * @param tileSize Dimensions of a single tile in pixels (e.g., 32x32)
     * @param tiles Pointer to the raw 1D integer map layout array
     * @param width Width of the map grid in number of tiles
     * @param height Height of the map grid in number of tiles
     * @return true if loading succeeded, false if texture failed to load
     */
    bool load(const std::string& tilesetPath,
        sf::Vector2u tileSize,
        const int* tiles);

	int getWidth() const { return mapWidth; }
	int getHeight() const { return mapHeight; }

private:
    // Overridden virtual function from sf::Drawable to allow custom rendering
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	// Dimensions de la carte en nombre de tuiles (Width)
    const unsigned int mapWidth;
	// Dimensions de la carte en nombre de tuiles (Height)
    const unsigned int mapHeight;

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};