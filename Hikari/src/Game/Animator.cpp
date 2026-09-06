//===============================
// Fichier : Animator.cpp
// Description : Implémentation de la classe Animator pour gérer les animations du joueur
// Auteur : Aboubacar Sanogo
// Version : 2.0
//===============================

// TODO : Ajouter la gestion des animations pour les ennemis et les objets du jeu

//===============================
// Inclusion des bibliothèques
//===============================
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>

#include "C:\MyProjects\Hikari\Hikari\include\Game\Animator.h"



Animator::Animator(sf::Sprite& s, int frameWidth, int frameHeight)
	: sprite(s), fL(frameWidth), fH(frameHeight)
{
	//rectSource = sprite.getTextureRect();
	rectSource = sf::IntRect({ 0,0 }, { fL,fH });
}



void Animator::play(int row, int max, float duration, bool shouldLoop) {
	// Pour vérifier si les valeurs doivent être changés
	if (currentRow == row && loop == shouldLoop && 
		maxFrames == max && frameDuration == duration) {
		return;
	}

	currentRow = row;
	maxFrames = max;
	frameDuration = duration;
	loop = shouldLoop;
	currentFrame = 0;
	elapsedTime = 0.f;
	finished = false;

	rectSource.position.x = 0;
	rectSource.position.y = currentRow * fH;
	sprite.setTextureRect(rectSource);
}



void Animator::update(float dt) {
	if (finished && !loop) return;

	elapsedTime += dt;
	if (elapsedTime >= frameDuration) {
		elapsedTime = 0.f;
		currentFrame++;
	}

	if (currentFrame >= maxFrames) {
		if (loop) {
			currentFrame = 0;
		}
		else {
			currentFrame = maxFrames - 1;
			finished = true;
		}
	}

	rectSource.position.x = currentFrame * fL;
	sprite.setTextureRect(rectSource);
}



void Animator::resetAnimation() {
	currentFrame = 0;
	elapsedTime = 0.f;
	finished = false;
	rectSource.position.x = 0;
	sprite.setTextureRect(rectSource);
}



bool Animator::isFinished() const
{
	return finished;
}
