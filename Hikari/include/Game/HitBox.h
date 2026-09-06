//===============================
// Fichier : HitBox.h
// Description : Implémentation des fonctions pour gérer les collisions entre les entités et les obstacles
// Auteur : Aboubacar Sanogo
// Version : 1.0
//===============================

#pragma once

//===============================
// Inclusion des bibliothèques
//===============================
#include "GameHeaders.h"

//===============================
// Déclaration de la fonction pour résoudre les collisions entre une entité et un obstacle
//===============================
void resolveCollision(sf::Sprite& entitySprite, const sf::FloatRect& entityHitbox, const sf::FloatRect& obstacleHitbox);

//===============================
// Déclaration de la fonction pour dessiner le contour de la hitbox d'une entité
//===============================
void drawHitboxOutline(sf::RenderWindow& window, const sf::FloatRect& hitbox);