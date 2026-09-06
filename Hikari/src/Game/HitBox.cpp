//===============================
// Fichier : HitBox.cpp
// Description : Implémentation des fonctions pour gérer les collisions entre les entités et les obstacles
// Auteur : Aboubacar Sanogo
// Version : 1.0
//===============================

//===============================
// Inclusion des bibliothèques nécessaires
//===============================
#include "HitBox.h"

void resolveCollision(sf::Sprite& entitySprite, const sf::FloatRect&
    entityHitbox, const sf::FloatRect& obstacleHitbox) {
    // findIntersection returns std::optional<sf::FloatRect> in SFML 3.0
    if (auto overlap = entityHitbox.findIntersection(obstacleHitbox)) {
        // Resolve along the axis of minimum overlap
        if (overlap->size.x < overlap->size.y) {
            // Collision on X axis: push left or right
            if (entityHitbox.position.x < obstacleHitbox.position.x) {
                entitySprite.move({ -overlap->size.x, 0.f });
            }
            else {
                entitySprite.move({ overlap->size.x, 0.f });
            }
        }
        else {
            // Collision on Y axis: push up or down
            if (entityHitbox.position.y < obstacleHitbox.position.y) {
                entitySprite.move({ 0.f, -overlap->size.y });
            }
            else {
                entitySprite.move({ 0.f, overlap->size.y });
            }
        }
    }
}

void drawHitboxOutline(sf::RenderWindow& window, const sf::FloatRect&
    hitbox) {
    sf::RectangleShape shape;
    shape.setPosition(hitbox.position);
    shape.setSize(hitbox.size);
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(sf::Color::Red);
    shape.setOutlineThickness(1.f);
    window.draw(shape);
}