//====================================
// Fichier : PlayerData.h
// Description : Implémentation des types de données pour le joueur
// Auteur : Aboubacar Sanogo
//====================================

#pragma once

#include <SFML/System/Vector2.hpp>
#include <string>
#include <SFML/System/Clock.hpp>

//// Enumération pour les différentes directions du joueur
//enum class Direction
//{
//	UP,
//	DOWN,
//	LEFT,
//	RIGHT
//};
//
//// Enumération pour les différents états du joueur
//enum class PlayerState
//{
//	DAMAGED,
//	DEAD,
//	IDLE,
//	WALKING,
//	RUNNING,
//	DASHING,
//	ATTACKING,
//	HEALING
//};
//
//// Enumération pour les différentes entrées clavier
//enum class KeyState
//{
//	PRESSED,
//	RELEASED
//};
//
//// Enumération pour les différentes touches du clavier
//enum class Keys
//{
//	UP,
//	DOWN,
//	LEFT,
//	RIGHT,
//	SHIFT
//};
//
//// Structure pour stocker les différents états du joueur sous forme de booléens
//struct PlayerStates
//{
//	bool damaged = false;
//	bool dead = false;
//	bool revived = false;
//	bool walk = false;
//	bool idle = false;
//	bool run = false;
//	bool dash = false;
//	bool attack = false;
//	bool healing = false;
//
//	// État actuel du joueur
//	PlayerState state = PlayerState::IDLE;
//	// État précédent du joueur pour gérer les transitions d'animation
//	PlayerState previousState = PlayerState::IDLE;
//};
//
//// Structure pour stocker les informations du joueur
//struct PlayerInfo
//{
//	std::string name = "";
//	float health = 0;
//	float maxHealth = 0;
//	float attackPower = 0;
//	float defense = 0;
//	float speed = 0;
//	float stamina = 0;
//	float dashDistance = 0;
//	float dashCooldown = 0;
//};
//
//// Structure pour stocker les informations d'animation du joueur
//struct PFrames
//{
//	// FrameTime : Temps écoulé depuis la dernière mise à jour de l'animation
//	float ft;
//	// MaxFrames : Nombre de frames d'animation pour chaque état du joueur
//	int mf;
//	// CurrentFrame : Frame actuelle de l'animation du joueur
//	int cf;
//	// PreviousFrame : Frame précédente de l'animation du joueur
//	int pf;
//};
//
//// Structure pour stocker les différentes directions du joueur
//struct DirectionInfo
//{
//	// Direction actuelle du joueur
//	Direction direction = Direction::DOWN;
//	// Direction précédente du joueur pour gérer les transitions d'animation
//	Direction previousDirection = Direction::DOWN;
//	// Mémoriser la dernière direction horizontale du joueur
//	Direction lastHorizontal = Direction::RIGHT;
//	// Mémoriser la dernière direction verticale du joueur
//	Direction lastVertical = Direction::DOWN;
//};
//
//// Structure pour stocker les différentes directions du joueur sous forme de booléens
//struct HeldDirection
//{
//	// Booléens pour gérer le déplacement à gauche
//	bool leftHeld = false;
//	// Booléens pour gérer le déplacement à droite
//	bool rightHeld = false;
//	// Booléens pour gérer le déplacement en haut
//	bool upHeld = false;
//	// Booléens pour gérer le déplacement en bas
//	bool downHeld = false;
//};
//
//struct AttackInfo
//{
//	// Horloge pour gérer le temps entre les attaques du joueur
//	sf::Clock attackCooldown;
//	// Temps de cooldown entre les attaques du joueur
//	float attackCooldownTime = 0.25f;
//	// Vecteur de déplacement du joueur en fonction des entrées clavier
//	sf::Vector2f attackVelocity = { 0.f, 0.f };
//	// Direction de l'attaque du joueur
//	Direction attackDirection = Direction::RIGHT;
//	// Booléen pour gérer l'attaque du joueur
//	bool attackPressed = false;
//	// Booléen pour savoir si le joueur est en train d'attaquer
//	bool attacking = false;
//};
//
//struct SpriteRows
//{
//	int idleRow;
//	int walkingRow;
//	int runningRow;
//	int dashRow;
//	int attackingRow;
//	int damagedRow;
//	int deadRow;
//	int healingRow;
//};