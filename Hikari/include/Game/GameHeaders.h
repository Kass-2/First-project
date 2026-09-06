//===============================
// Fichier : GameHeaders.h
// Description : Avoir un point d'entrée unique pour inclure tous les en-têtes de classes du jeu, 
//				 afin de faciliter l'inclusion dans les fichiers source et d'améliorer la lisibilité 
//				 du code.
// Auteur : Aboubacar Sanogo
//===============================

#pragma once

//===============================
// Inclusion des bibliothèques
//===============================
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "TileMap.h"
#include "MapData.h"
#include "InputManager.h"
