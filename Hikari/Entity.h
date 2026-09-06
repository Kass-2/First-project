////////////////////////////////////////////////////////////////
// \brief Base class for all entities in the game
// This class is abstract and cannot be instantiated directly. It defines the common interface for all entities, such as updating and drawing.
// Each entity has a sprite and a texture, which can be used to display the entity on the screen. The position of the entity can be set and retrieved using the getPosition and setPosition methods.
// \author Aboubacar Sanogo
// \version 1.0
///////////////////////////////////////////////////////////////

#pragma once

//===============================
// Inclusion des bibliothèques
//===============================
#include <string>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Clock.hpp>

// Enumération pour les différents états du joueur
enum class PlayerState
{
	DAMAGED,
	DEAD,
	IDLE,
	WALKING,
	JOGGING,
	RUNNING,
	DASHING,
	ATTACKING,
	HEALING,
	EXHAUSTED
};



// Enumération pour les différentes entrées clavier
enum class KeyState
{
	PRESSED,
	RELEASED
};



// Enumération pour les différentes touches du clavier
enum class Keys
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	SHIFT
};



// Enumération pour les différentes directions du joueur
enum class Direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};



// Structure pour stocker les informations d'animation du joueur
struct PFrames
{
	// FrameTime : Temps écoulé depuis la dernière mise à jour de l'animation
	float ft;
	// MaxFrames : Nombre de frames d'animation pour chaque état du joueur
	int mf;
	// CurrentFrame : Frame actuelle de l'animation du joueur
	int cf;
	// PreviousFrame : Frame précédente de l'animation du joueur
	int pf;
};



// Structure pour stocker les différentes directions du joueur
struct DirectionInfo
{
	// Direction actuelle du joueur
	Direction direction = Direction::DOWN;
	// Direction précédente du joueur pour gérer les transitions d'animation
	Direction previousDirection = Direction::DOWN;
	// Mémoriser la dernière direction horizontale du joueur
	Direction lastHorizontal = Direction::RIGHT;
	// Mémoriser la dernière direction verticale du joueur
	Direction lastVertical = Direction::DOWN;
};



// Structure pour stocker les différentes directions du joueur sous forme de booléens
struct HeldDirection
{
	// Booléens pour gérer le déplacement à gauche
	bool leftHeld = false;
	// Booléens pour gérer le déplacement à droite
	bool rightHeld = false;
	// Booléens pour gérer le déplacement en haut
	bool upHeld = false;
	// Booléens pour gérer le déplacement en bas
	bool downHeld = false;
	// Booléens pour gérer le sprint du joueur
	bool shiftHeld = false;
};



struct AttackInfo
{
	// Horloge pour gérer le temps entre les attaques du joueur
	sf::Clock attackCooldown;
	// Vecteur de déplacement du joueur en fonction des entrées clavier
	sf::Vector2f attackVelocity = { 0.f, 0.f };
	// Direction de l'attaque du joueur
	Direction attackDirection = Direction::RIGHT;
	// Booléen pour gérer l'attaque du joueur
	bool attackPressed = false;
	// Booléen pour savoir si le joueur est en train d'attaquer
	bool attacking = false;
};



// Structure pour stocker les informations du joueur
struct PlayerInfo
{
	std::string name = "";
	float health = 0;
	float maxHealth = 0;
	float attackPower = 0;
	float defense = 0;
	float speed = 0;
	float stamina = 0;
	float dashDistance = 0;
	float dashCooldown = 0;
};



// Structure pour stocker les différents états du joueur sous forme de booléens
struct PlayerStates
{
	bool damaged = false;
	bool dead = false;
	bool revived = false;
	bool walk = false;
	bool idle = false;
	bool jog = false;
	bool run = false;
	bool dash = false;
	bool attack = false;
	bool healing = false;

	// État actuel du joueur
	PlayerState state = PlayerState::IDLE;
	// État précédent du joueur pour gérer les transitions d'animation
	PlayerState previousState = PlayerState::IDLE;
};



//===============================
// Classe Entity qui sert de base pour toutes les entités du jeu (joueur, ennemis, objets, etc.)
// Cette classe est abstraite et ne peut pas être instanciée directement. Elle définit l'interface commune pour toutes les entités, telles que la mise à jour et le dessin.
// Chaque entité a un sprite et une texture, qui peuvent être utilisés pour afficher l'entité à l'écran. La position de l'entité peut être définie et récupérée à l'aide des méthodes getPosition et setPosition.
//===============================
class Entity {
public:
	//===============================
	// Constructeur pour une entité
	//===============================
	Entity() : sprite(texture) {};

	//===============================
	// Destructeur pour une entité
	//===============================
	virtual ~Entity() = default;

	// Fonction pour mettre à jour une entité
	virtual void update(float deltaTime, const sf::RenderWindow& window) = 0;
	
	// Fonction pour dessiner une entité
	virtual void draw(sf::RenderWindow& window) = 0;

	// Fonction pour avoir la position d'un sprite
	sf::Vector2f getPosition() const { return sprite.getPosition(); }

	// Fonction pour choisir la position d'un sprite
	void setPosition(const sf::Vector2f& pos) { sprite.setPosition(pos); }

	// Getter pour le sprite
	sf::Sprite& getSprite() {
		return sprite;
	}
	// Getter pour le sprite
	const sf::Sprite& getSprite() const {
		return sprite;
	}

protected:
	sf::Texture texture;	// Texture utiliser pour l'entité (Fichier .png)
	sf::Sprite sprite;		// Le sprite pour l'entité basé sur la texture

	int getDirectionOffSet(Direction dir) {
		switch (dir) {
			case Direction::DOWN:	return 0;
			case Direction::UP:		return 1;
			case Direction::RIGHT:	return 2;
			case Direction::LEFT:	return 3;
		}
		return 0;
	}
};