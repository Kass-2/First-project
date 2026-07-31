//===============================
// Fichier : Animator.h
// Description : Déclaration de la classe Animator pour gérer les animations
// Auteur : Aboubacar Sanogo
// Version : 1.0
//===============================

#pragma once

//===============================
// Inclusion des bibliothèques
//===============================
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>



// Structure pour avoir les configurations pour l'animation
struct AnimationConfig {
	int startRow;			// Début de la ligne pour l'animation du sprite
	int maxFrames;			// Nombres maximum de frames pour l'animation
	float frameDuration;	// Durée pour chaque frame de l'animation
	bool loop;				// Pour savoir si l'animation recommence ou pas (e.g. IDLE, RUNNING)
};



//=========================================================
// Classe Animator pour gérer les animations du joueur.
// Cette classe est responsable de la mise à jour de l'animation du joueur en fonction de son état et de sa direction,
// ainsi que de la gestion des frames d'animation et du temps entre les frames.
//=========================================================
class Animator {
public:
	//===============================
	// \brief Constructeur pour la classe animator : 
	// 
	// \brief La fonction controle le sprite passé en référence 
	//===============================
	Animator(sf::Sprite& sprite, int frameWidth, int frameHeight);

	//===============================
	// Fonction pour jouer l'animation pour la rangée du spriteSheet
	// La fonction vérifie si la rangée à changé et change les valeurs de la classe si oui ou si l'animation ne recommence pas
	//===============================
	void play(int row, int maxFrames, float frameduration, bool loop = true);

	//===============================
	// Fonction pour mettre à jour le frame timing de l'animation
	//===============================
	void update(float dt);

	//===============================
	// Fonction pour recommencer l'animation à la frame 0
	//===============================
	void resetAnimation();

	//===============================
	// Fonction pour voir si l'animation qui ne loop pas est fini
	//===============================
	bool isFinished() const;

	//===============================
	// Fonction getter pour avoir le frame courant de l'animation
	//===============================
	int getCurrentFrame() const {
		return currentFrame;
	}

private:
	sf::Sprite& sprite;		// Pour permettre l'animation
	sf::IntRect rectSource;	// Pour
	int fH;					// FrameHauteur : Hauteur de la partie de la texture à afficher
	int fL;					// FrameLargeur : Largeur de la partie de la texture à afficher

	int currentFrame = 0;		// Frame courant de l'animation
	int maxFrames = 8;			// Nombres maximal de frames pour l'animation
	float frameDuration = 0.5f;	// Durée de chaque frame pour jouer avec la vitesse de l'animation
	float elapsedTime = 0.f;	// Temps écoulé par l'animation
	bool loop = true;			// Pour savoir si l'animation doit continuer en boucle
	bool finished = false;		// Pour savoir si l'animation est terminé
	int currentRow = -1;		// Rangée courante pour l'animation (Basé sur le spriteSheet)
};
