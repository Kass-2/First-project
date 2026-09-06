//====================================
// Fichier : tileMap.cpp
// Description : Implémentation de la classe TileMap pour gérer les cartes de tuiles
// Auteur : Aboubacar Sanogo
// Version : 1.0
//====================================

#include "tileMap.h"

TileMap::TileMap(unsigned int mapWidth, unsigned int mapHeight)
    : mapWidth(mapWidth), mapHeight(mapHeight)
{

}

bool TileMap::load(const std::string& tilesetPath, sf::Vector2u tileSize, 
    const int* tiles)
{
    float tileWidth = static_cast<float>(tileSize.x);
    float tileHeight = static_cast<float>(tileSize.y);

    unsigned int width = mapWidth;
	unsigned int height = mapHeight;

    if (!m_tileset.loadFromFile(tilesetPath))
        return false;

    m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    m_vertices.resize(width * height * 6);

    for (unsigned int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            int tileNumber = tiles[x + y * width];

            unsigned int tilesPerRow = m_tileset.getSize().x / tileSize.x;

            int tu = tileNumber % tilesPerRow;
            int tv = tileNumber / tilesPerRow;

            sf::Vertex* triangles = &m_vertices[(x + y * width) * 6];

            sf::Vector2f p0(x * tileWidth, y * tileHeight);
            sf::Vector2f p1((x + 1) * tileWidth, y * tileHeight);
            sf::Vector2f p2((x + 1) * tileWidth, (y + 1) * tileHeight);
            sf::Vector2f p3(x * tileWidth, (y + 1) * tileHeight);

            sf::Vector2f t0(tu * tileWidth, tv * tileHeight);
            sf::Vector2f t1((tu + 1) * tileWidth, tv * tileHeight);
            sf::Vector2f t2((tu + 1) * tileWidth, (tv + 1) * tileHeight);
            sf::Vector2f t3(tu * tileWidth, (tv + 1) * tileHeight);

            // Triangle 1
            triangles[0].position = p0; triangles[0].texCoords = t0;
            triangles[1].position = p1; triangles[1].texCoords = t1;
            triangles[2].position = p2; triangles[2].texCoords = t2;

            // Triangle 2
            triangles[3].position = p0; triangles[3].texCoords = t0;
            triangles[4].position = p2; triangles[4].texCoords = t2;
            triangles[5].position = p3; triangles[5].texCoords = t3;
        }
    }
    return true;
}

void TileMap::draw(sf::RenderTarget& target, 
    sf::RenderStates states) const 
{
    states.transform *= getTransform();
    states.texture = &m_tileset;
    target.draw(m_vertices, states);
}