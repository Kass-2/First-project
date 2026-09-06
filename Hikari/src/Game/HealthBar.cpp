//===============================
// Fichier : HealthBar.cpp
// Description : Implémentation de la classe HealthBar pour gérer la barre de vie du joueur
// Auteur : Aboubacar Sanogo
// Version : 1.0
//===============================

//===============================
// Inclusion des bibliothèques
//===============================
#include "HealthBar.h"
#include <algorithm>
#include <iostream>

HealthBar::HealthBar(const std::string& containerPath, 
    int containerWidth, int containerHeight, int containerFrames,
    const std::string& firePath, int fireWidth, int fireHeight, 
    int fireFrames, float frameTime)
    : containerSprite(containerTexture), fireSprite(fireTexture),
    cW(containerWidth), cH(containerHeight), containerTotalFrames(containerFrames), 
    containerCurrentFrame(0), fW(fireWidth), fH(fireHeight), fireTotalFrames(fireFrames),
    fireCurrentFrame(0), frameTime(frameTime), animationTimer(0.f)
{
    // Load container spritesheet
    if (!containerTexture.loadFromFile(containerPath)) {
        std::cerr << "Error loading health container texture: " <<
            containerPath << std::endl;
    }

    // Load fire spritesheet
    if (!fireTexture.loadFromFile(firePath)) {
        std::cerr << "Error loading health fire texture: " << firePath
            << std::endl;
    }

    // Set initial full texture rect boundaries
    containerSprite.setTextureRect(sf::IntRect({ 0, 0 }, { cW, cH }));
    fireSprite.setTextureRect(sf::IntRect({ 0, 0 }, { fW, fH }));
}

void HealthBar::update(float currentHealth, float maxHealth, float dt) {
	// Permet de gérer l'animation des spritesheets pour le conteneur et le feu
    animationTimer += dt;
    if (animationTimer >= frameTime) {
        animationTimer -= frameTime;
        containerCurrentFrame = (containerCurrentFrame + 1) %
            containerTotalFrames;
        fireCurrentFrame = (fireCurrentFrame + 1) % fireTotalFrames;
    }

	// Mettre à jour le rectangle de texture pour le conteneur en fonction de la frame actuelle
    containerSprite.setTextureRect(sf::IntRect(
        { containerCurrentFrame * cW, 0 },
        { cW, cH }
    ));

	// Calculer le ratio de santé pour déterminer la largeur du feu à afficher
    float healthRatio = 0.f;
    if (maxHealth > 0.f) {
        healthRatio = std::clamp(currentHealth / maxHealth, 0.f, 1.f);
    }

	// Calculer la largeur du feu à afficher en fonction du ratio de santé
    int croppedHeight = static_cast<int>(fH * healthRatio);
    yOffset = static_cast<float>(fH - croppedHeight);
    
    // Mettre à jour le rectangle de texture pour le feu en fonction de la frame actuelle et du ratio de santé
    sf::IntRect fireRect(
        { fireCurrentFrame* fW, fH - croppedHeight },
        { fW, croppedHeight }
    );
    fireSprite.setTextureRect(fireRect);
}

void HealthBar::draw(sf::RenderWindow& window,
    const sf::Vector2f&
    position) {
	// Mettre à jour la position des sprites pour qu'ils soient dessinés à la position spécifiée
    containerSprite.setPosition(position);
    fireSprite.setPosition({position.x, position.y + yOffset});

	// Dessiner le conteneur et le feu sur la fenêtre
    window.draw(containerSprite);
    window.draw(fireSprite);
}