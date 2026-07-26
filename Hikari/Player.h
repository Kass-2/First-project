//=========================================================
// Classe Player qui hérite de Entity et qui gère les mouvements, les animations et les états du joueur
// Cette classe est responsable de la mise à jour de la position du joueur en fonction des entrées clavier, 
// de la gestion des animations en fonction de l'état du joueur (idle, walk, run, attack) et de l'affichage 
// du sprite du joueur à l'écran.
// \author Aboubacar Sanogo
// \version 1.0
// \note Cette classe utilise une structure booléenne pour stocker les différents états du joueur, ainsi qu'une énumération pour les différentes directions du joueur.
// \warning Cette classe est un exemple de base pour la gestion d'un personnage joueur dans un jeu 2D, et peut être améliorée et étendue pour inclure des fonctionnalités supplémentaires telles que les collisions, les interactions avec d'autres entités, etc.
// 
// \todo Ajouter la gestion des collisions avec les murs et les autres entités du jeu
// \todo Ajouter la gestion des interactions avec les objets du jeu (ramasser des objets, ouvrir des portes, etc.)
// \todo Ajouter la gestion de la santé du joueur et des dégâts subis
// \todo Ajouter la gestion de l'inventaire du joueur et des objets ramassés
// \todo Ajouter la gestion des compétences et des attaques du joueur
// \todo Ajouter la gestion de l'expérience et du niveau du joueur
// \todo Ajouter la gestion de l'animation du joueur en fonction de l'état et de la direction du joueur
//=========================================================

#pragma once

//===============================
// Inclusion des bibliothèques
//===============================
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "PlayerData.h"
#include "Animations.h"
#include "InputManager.h"

//=========================================================
// Classe Player qui hérite de Entity et qui gère les mouvements, les animations et les états du joueur.
// Cette classe est responsable de la mise à jour de la position du joueur en fonction des entrées clavier, 
// de la gestion des animations en fonction de l'état du joueur (idle, walk, run, attack) et de l'affichage
// du sprite du joueur à l'écran.
//==========================================================
class Player {
public:
	//===============================
	// Création du sprite du joueur
	//===============================
	Player(sf::Texture& texture, sf::Sprite& sprite, const std::string& textureFile, int frameWidth, int frameHeight);
	
	//===============================
	// Fonction pour définir la position du joueur
	//===============================
	void setPlayerPosition(sf::Sprite& sprite, float x, float y);

	//===============================
	// Fonction pour obtenir la hitbox du joueur en fonction de son sprite
	//===============================
	sf::FloatRect getHitbox(const sf::Sprite& sprite) const;

	//===============================
	// Mise à jour de la position du joueur en fonction des entrées clavier et de l'état du joueur
	//===============================
	void update(float dt, const sf::RenderWindow & window, sf::Sprite& sprite);
	
	//===============================
	// Initialisation des informations du joueur (nom, santé, puissance d'attaque, défense, vitesse, etc.)
	//===============================
	void initialState(std::string);

	//===============================
	// Getter pour la santé actuelle du joueur
	//===============================
	float getHealth() const;

	//===============================
	// Getter pour la santé maximale du joueur
	//===============================
	float getMaxHealth() const;

	//===============================
	// Gestion des événements clavier pour mettre à jour les booléens correspondants aux touches pressées ou relâchées
	//===============================
	void handleEvent(const sf::Event& event);

	//===============================
	// Getter pour la position du joueur
	//===============================
	sf::Vector2f getPosition(const sf::Sprite& sprite) const;

	//===============================
	// Affichage du sprite du joueur à l'écran
	//===============================
	void draw(sf::RenderWindow& window, sf::Sprite& sprite);

	//===============================
	// Verrouillage du mouvement du joueur (par exemple, lors d'une attaque ou d'une interaction avec un objet)
	//===============================
	void LockPlayer() { lock = true; }

	//===============================
	// Déverrouillage du mouvement du joueur
	//===============================
	void UnlockPlayer() { lock = false; }

	//===============================
	// Réinitialisation les états du joueur
	//===============================
	void resetInputs();

private:
	//===============================
	// Gestion des entrées clavier pour déplacer le joueur et changer son état
	//===============================
	void handleInput(sf::Sprite& sprite, float dt);

	//===============================
	// Mise à jour de l'animation du joueur en fonction de son état et de sa direction
	//===============================
	void updateAnimation(float deltaTime, sf::Sprite& sprite);

	//===============================
	// Gestion des frames de l'animation du joueur en fonction de son état
	//===============================
	void PlayerFrameAnimation();

	//===============================
	// Détermination du temps entre les frames et du nombre de frames en fonction de l'état du joueur
	//===============================
	void StateFrameTime(const float frameTime, const int maxFrames);

	//===============================
	// Détermination de la ligne de la texture à utiliser en fonction de l'état du joueur
	//===============================
	void currentLineTexture(int& targetRow) const;

	//===============================
	// Mise à jour de l'état du joueur en fonction des booléens
	//===============================
	void handlePlayerState(PlayerStates& playerState, AttackInfo& AInfo) const;

	//===============================
	// Mise à jour des lignes de la texture pour chaque état du joueur en fonction de la direction du joueur
	//===============================
	void handleRows(DirectionInfo& DInfo, SpriteRows& rows) const;

	//===============================
	// Fonction pour réinitialiser l'animation du joueur lorsque son état ou sa direction change
	//===============================
	void resetPlayer(bool animationChanged, sf::Sprite& sprite, SpriteRows rows, Animations reset);

	// Informations d'animation du joueur
	PFrames playerFrames;

	// FrameLargeur : Largeur de la partie de la texture à afficher
	int fL;
	// FrameHauteur : Hauteur de la partie de la texture à afficher
	int fH;
	// Nombre de colonnes dans la texture
	int columns = 0;
	// Nombre de lignes dans la texture
	int lignes = 0;
	
	// Texture et sprite du joueur
	std::string textureFile;

	// Rectangle source pour l'animation du joueur
	sf::IntRect rectSource;
	// Horloge pour gérer le temps entre les frames d'animation
	sf::Clock animationClock;

	// Horloge pour le temps de cooldown du dash
	sf::Clock dashCooldownClock;

	// Structure pour stocker les différents états du joueur sous forme de booléens
	PlayerStates playerState;
	// Informations sur le joueur
	PlayerInfo player;

	// Informations sur l'attaque du joueur
	AttackInfo AInfo;

	// Informations sur les directions maintenues
	HeldDirection heldState;

	// Informations sur les différentes lignes de la texture pour chaque état du joueur
	SpriteRows rows;

	// Booléen pour verrouiller le mouvement du joueur
	bool lock = false;

	// Informations sur les directions du joueur
	DirectionInfo DInfo;

	// Gestionnaire d'entrées clavier pour le joueur
	InputManager inputManager;

	// TODO: À utiliser
	// Booléens pour gérer l'inventaire du joueur
	bool inventory = false;
	// TODO: À utiliser
	// Booléens pour gérer l'interaction avec les objets du jeu
	bool interact = false;
};