//===============================
// Fichier : InputManager.cpp
// Description : Implémentation de la classe InputManager pour gérer les entrées clavier du joueur
// Auteur : Aboubacar Sanogo
// Version : 1.0
//===============================

//===============================
// Inclusion des bibliothèques nécessaires
//===============================
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Vector2.hpp>

#include "InputManager.h"
#include "entity.h"



sf::Vector2f InputManager::handleMovement(
	sf::Sprite& sprite, 
	PlayerStates& playerState, 
	PlayerInfo& player, 
	HeldDirection& heldState, 
	DirectionInfo& directionInfo, 
	AttackInfo& AInfo) const
{	
	// Vecteur de mouvement du joueur en fonction des entrées clavier
	sf::Vector2f movement = { 0.f, 0.f };

	// Mémoriser les états des touches pour gérer les cas où les deux touches sont pressées en même temps
	if (heldState.leftHeld && heldState.rightHeld)
	{
		if (directionInfo.lastHorizontal == Direction::LEFT)
		{
			movement.x = -1.f;
			directionInfo.direction = Direction::LEFT;
		}
		else
		{
			movement.x = 1.f;
			directionInfo.direction = Direction::RIGHT;
		}
	}
	else if (heldState.leftHeld)
	{
		movement.x = -1.f;
		directionInfo.direction = Direction::LEFT;
	}
	else if (heldState.rightHeld)
	{
		movement.x = 1.f;
		directionInfo.direction = Direction::RIGHT;
	}

	// Mémoriser la dernière direction horizontale pour gérer les cas où les deux touches sont pressées en même temps
	if (heldState.upHeld && heldState.downHeld)
	{
		if (directionInfo.lastVertical == Direction::UP)
		{
			movement.y = -1.f;
			directionInfo.direction = Direction::UP;
		}
		else
		{
			movement.y = 1.f;
			directionInfo.direction = Direction::DOWN;
		}
	}
	else if (heldState.upHeld)
	{
		movement.y = -1.f;
		directionInfo.direction = Direction::UP;
	}
	else if (heldState.downHeld)
	{
		movement.y = 1.f;
		directionInfo.direction = Direction::DOWN;
	}

	bool isMovingHorizontal = heldState.leftHeld || heldState.rightHeld;
	bool isMovingVertical = heldState.upHeld || heldState.downHeld;

	if (isMovingHorizontal && isMovingVertical)
	{
		if (lastPressedAxis == Axis::HORIZONTAL)
		{
			// Horizontal was pressed last, so vertical was pressed first. Face vertical.
			if (heldState.upHeld && heldState.downHeld)
			{
				directionInfo.direction = (directionInfo.lastVertical == Direction::UP) ?
					Direction::UP : Direction::DOWN;
			}
			else
			{
				directionInfo.direction = heldState.upHeld ? Direction::UP :
					Direction::DOWN;
			}
		}
		else
		{
			// Vertical was pressed last, so horizontal was pressed first. Face horizontal.
			if (heldState.leftHeld && heldState.rightHeld)
			{
				directionInfo.direction = (directionInfo.lastHorizontal ==
					Direction::LEFT) ?
					Direction::LEFT : Direction::RIGHT;
			}
			else
			{
				directionInfo.direction = heldState.leftHeld ? Direction::LEFT :
					Direction::RIGHT;
			}
		}
	}

	bool moving = (movement.x != 0.f || movement.y != 0.f);

	playerState.jog = moving && (playerState.run == false) && (playerState.walk == false);

	playerState.run = moving && playerState.run;

	playerState.walk = moving && playerState.walk;

	playerState.idle = !moving && !AInfo.attacking;

	// Si le joueur est en train d'attaquer, on réduit sa vitesse de déplacement
	if (AInfo.attacking) player.speed *= 0.25f;
	else player.speed = playerState.run ? (3.f * 60) : (1.9f * 60);

	// Normaliser le vecteur de mouvement pour éviter que le personnage ne se déplace plus vite en diagonale
	if (movement.x != 0.f || movement.y != 0.f)
	{
		float length = std::sqrt(
			movement.x * movement.x +
			movement.y * movement.y);

		movement = (movement / length);
	}
	// Appliquer une légère réduction de la vitesse en diagonale pour éviter que le personnage ne se déplace plus vite en diagonale
	if (movement.x != 0.f && movement.y != 0.f)
	{
		movement *= 0.9f;
	}
	
	return movement;
}



void InputManager::handleEvent(const sf::Event& event, 
	HeldDirection& heldState, 
	DirectionInfo& directionInfo, 
	AttackInfo& AInfo, 
	PlayerStates& playerState, 
	bool& inventory)
{
	if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
	{
		switch (keyPressed->code)
		{
		case sf::Keyboard::Key::A:
		case sf::Keyboard::Key::Left:
			heldState.leftHeld = true;
			directionInfo.lastHorizontal = Direction::LEFT;
			lastPressedAxis = Axis::HORIZONTAL;
			break;

		case sf::Keyboard::Key::D:
		case sf::Keyboard::Key::Right:
			heldState.rightHeld = true;
			directionInfo.lastHorizontal = Direction::RIGHT;
			lastPressedAxis = Axis::HORIZONTAL;
			break;

		case sf::Keyboard::Key::W:
		case sf::Keyboard::Key::Up:
			heldState.upHeld = true;
			directionInfo.lastVertical = Direction::UP;
			lastPressedAxis = Axis::VERTICAL;
			break;

		case sf::Keyboard::Key::S:
		case sf::Keyboard::Key::Down:
			heldState.downHeld = true;
			directionInfo.lastVertical = Direction::DOWN;
			lastPressedAxis = Axis::VERTICAL;
			break;
		case sf::Keyboard::Key::Space:
			playerState.dash = true;
			break;
		}
		
	}

	if (const auto* keyReleased = event.getIf<sf::Event::KeyReleased>())
	{
		switch (keyReleased->code)
		{
			// Touches de déplacement vers la gauche
		case sf::Keyboard::Key::A:
		case sf::Keyboard::Key::Left:
			heldState.leftHeld = false;
			break;
			// Touches de déplacement vers la droite
		case sf::Keyboard::Key::D:
		case sf::Keyboard::Key::Right:
			heldState.rightHeld = false;
			break;
			// Touches de déplacement vers le haut
		case sf::Keyboard::Key::W:
		case sf::Keyboard::Key::Up:
			heldState.upHeld = false;
			break;
			// Touches de déplacement vers le bas
		case sf::Keyboard::Key::S:
		case sf::Keyboard::Key::Down:
			heldState.downHeld = false;
			break;
			// Si le joueur est en train de courir, on le fait marcher, sinon on le fait courir
		case sf::Keyboard::Key::LControl:
			playerState.walk = !playerState.walk;
			break;
			// Si le joueur est en train de courir, on le fait marcher, sinon on le fait courir
		case sf::Keyboard::Key::LShift:
			playerState.run = !playerState.run;
			break;
			// Touches d'attaque (entrée)
		case sf::Keyboard::Key::Enter:
			AInfo.attackPressed = true;
			break;
			// Touche de test pour endommager le joueur (à supprimer plus tard)
		case sf::Keyboard::Key::J:
			playerState.damaged = true;
			break;
			// Touche de test pour revive le joueur (à supprimer plus tard)
		case sf::Keyboard::Key::K:
			playerState.dead = false;
			break;
			// Touche de test pour tuer le joueur (à supprimer plus tard)
		case sf::Keyboard::Key::L:
			playerState.dead = true;
			break;
		case sf::Keyboard::Key::H :
			playerState.healing = true;
			break;
		case sf::Keyboard::Key::I:
			inventory = !inventory; // Toggle inventory
			break;
		}
	}

	/*if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>())
	{
		if (mouseButtonPressed->button == sf::Mouse::Button::Left)
		{
			attackPressed = true;
		}
	}*/
}
