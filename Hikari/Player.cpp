//===============================
// Fichier : Player.cpp
// Description : Implémentation de la classe Player pour gérer le personnage du joueur
// Auteur : Aboubacar Sanogo
// Version : 1.0
//===============================

//===============================
// Inclusion des bibliothèques nécessaires
//===============================
#include <iostream>
#include <string>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "Player.h"
#include "MapData.h"
#include "EntityData.h"
#include "Entity.h"



Player::Player(const std::string& tf, int frameWidth, int frameHeight)
	: /*playerFrames({ 0.3f, 8, 0, -1 }), rows({ 0 }),*/ 
	textureFile(tf), fL(frameWidth), fH(frameHeight), animator(sprite, frameWidth, frameHeight)
{
	// Chargement de la texture du joueur
	if (!texture.loadFromFile(textureFile))
	{
		std::cout << "Failed to load player texture!" << std::endl;
		return;
	}

	sprite.setTexture(texture);

	//// Calcul du nombre de colonnes et de lignes dans la texture
	//int x = columns * fL;                   // Position X de la partie de la texture à afficher
	//int y = lignes * fH;                    // Position Y de la partie de la texture à afficher

	//// Sprite du joueur : on affiche la partie de la texture qui correspond au personnage (32x32 pixels)
	//sf::IntRect r({ x,y }, { fL,fH });
	//rectSource = r;
	//sprite.setTextureRect(rectSource);

	sprite.setOrigin({ fL/2.f, fH/2.f });       // Centrer l'origine du sprite
	sprite.setPosition({ 400.f, 300.f });   // Position initiale du joueur au centre de la fenêtre
	sprite.setScale({ 1.0f, 1.0f });        // Modifier la taille du sprite du joueur

	// Initialiser les positions X et Y du rectangle source pour l'animation du joueur
	//rectSource.position.x = 0;

	// Les lignes pour l'état idle sont différentes pour chaque direction
	int IdleRow =
		directionInfo.direction == Direction::DOWN ? 9 :
		directionInfo.direction == Direction::UP ? 10 :
		directionInfo.direction == Direction::LEFT ? 12 :
		directionInfo.direction == Direction::RIGHT ? 11 : 9;

	IdleRow -= 1;

	// Initialiser la position Y du rectangle source en fonction de la direction du joueur
	//rectSource.position.y = IdleRow * fH;
	//sprite.setTextureRect(rectSource);

	// Initialiser l'animator avec l'animation IDLE
	animator.play(IdleRow, 8, 0.2f, true);

	playerState.previousState = playerState.state;
	directionInfo.previousDirection = directionInfo.direction;

	//animationClock.restart();
}



void Player::setPlayerPosition(float x, float y) {
	// Si le mouvement du joueur est verrouillé, on ne change pas sa position
	if (lock) return;
	sprite.setPosition({ x, y });
}



void Player::resetInputs() {
	// Reset key held flags
	heldState.leftHeld = false;
	heldState.rightHeld = false;
	heldState.upHeld = false;
	heldState.downHeld = false;
}



sf::FloatRect Player::getHitbox() const {
	// Custom hitbox dimensions
	float width = 16.f;
	float height = 24.f;

	sf::Vector2f pos = sprite.getPosition();

	// Calculate the top-left coordinate of the hitbox relative to centered origin
	// This positions the hitbox at the bottom-center of the character's sprite
	return sf::FloatRect(
		{ pos.x - width / 2.f, pos.y - 10.f },
		{ width, height }
	);
}



void Player::initialState(std::string characterName) {
	player.name = characterName;
	player.health = 100;
	player.maxHealth = 100;
	player.attackPower = 10;
	player.defense = 5;
	player.speed = (1.f * 60);
	player.stamina = 100;
	player.dashDistance = 3;
}



float Player::getHealth() const {
	return player.health;
}



float Player::getMaxHealth() const {
	return player.maxHealth;
}



void Player::takeDamage(float amount) {
	if (playerState.dead) return;

	player.health -= amount;
	if (player.health <= 0.f) {
		player.health = 0.f;
		playerState.dead = true;
		playerState.state = PlayerState::DEAD;
	}
	else {
		playerState.damaged = true;
		playerState.state = PlayerState::DAMAGED;
	}

	animator.resetAnimation(); 
}



void Player::useHeal(float amount) {
	if (playerState.dead) return;

	player.health += amount;
	if (player.health > player.maxHealth) {
		player.health = player.maxHealth;
	}
	else {
		playerState.healing = true;
		playerState.state = PlayerState::HEALING;
	}

	animator.resetAnimation();
}



bool Player::dashMechanic(float dt) {
	// 2. Manage Exhausted (Stumble) state
	if (playerState.state == PlayerState::EXHAUSTED) {
		if (exhaustionTimer.getElapsedTime().asSeconds() >= 0.4f) {
			playerState.state = PlayerState::IDLE; // Recover after 0.4 seconds
		}
		velocity = { 0.f, 0.f }; // Lock velocity to zero
		return false;
	}

	// 3. Pre-update the chain dash window check (needed for the input step below)
	if (playerState.state == PlayerState::DASHING) {
		float elapsed = dashTimer.getElapsedTime().asSeconds();
		float dashDuration = 0.21f;
		chainDash = (elapsed >= 0.15f && elapsed <= dashDuration);
	}
	else {
		chainDash = false;
	}

	// 4. Process new dash input (Space key pressed)
	if (playerState.dash) {
		playerState.dash = false;

		// Calculate dash direction based on currently held keys (allows zig-zag chain dashes)
		sf::Vector2f newDashDir = { 0.f, 0.f };

		// Determine horizontal direction using last pressed horizontal key
		if (heldState.leftHeld && heldState.rightHeld) {
			newDashDir.x = (directionInfo.lastHorizontal == Direction::LEFT) ? -1.f : 1.f;
		}
		else if (heldState.leftHeld) {
			newDashDir.x = -1.f;
		}
		else if (heldState.rightHeld) {
			newDashDir.x = 1.f;
		}

		// Determine vertical direction using last pressed vertical key
		if (heldState.upHeld && heldState.downHeld) {
			newDashDir.y = (directionInfo.lastVertical == Direction::UP) ? -1.f : 1.f;
		}
		else if (heldState.upHeld) {
			newDashDir.y = -1.f;
		}
		else if (heldState.downHeld) {
			newDashDir.y = 1.f;
		}

		if (newDashDir != sf::Vector2f(0.f, 0.f)) {
			// Normalize direction vector
			float len = std::sqrt(newDashDir.x * newDashDir.x + newDashDir.y * newDashDir.y);
			newDashDir = newDashDir / len;
		}
		else {
			// Default to current facing direction if no movement keys are held
			switch (directionInfo.direction) {
			case Direction::LEFT:  newDashDir = { -1.f, 0.f }; break;
			case Direction::RIGHT: newDashDir = { 1.f, 0.f }; break;
			case Direction::UP:    newDashDir = { 0.f, -1.f }; break;
			case Direction::DOWN:  newDashDir = { 0.f, 1.f }; break;
			}
		}

		if (playerState.state == PlayerState::DASHING) {
			if (chainDash) {
				// Perfect timing: Chain Dash!
				dashChainCount++;
				dashDirection = newDashDir;
				dashTimer.restart();
				animator.resetAnimation();
			}
			else {
				// Bad timing (Pressed too early/late): Stumble / Exhausted
				playerState.state = PlayerState::EXHAUSTED;
				exhaustionTimer.restart();
				dashChainCount = 0;
				velocity = { 0.f, 0.f }; // Stop movement instantly
				return false;
			}
		}
		else {
			// First dash initiation
			playerState.state = PlayerState::DASHING;
			dashDirection = newDashDir;
			dashTimer.restart();
			dashChainCount = 0;
		}
		return false;
	}

	// 5. Active Dash Movement
	if (playerState.state == PlayerState::DASHING) {
		float elapsed = dashTimer.getElapsedTime().asSeconds();
		float dashDuration = 0.21f;

		if (elapsed >= dashDuration) {
			playerState.state = PlayerState::IDLE;
			dashChainCount = 0;
		}
		else {
			// Speed increases by +15% per successful chain (capped at +60%)
			float chainMultiplier = 1.0f + (dashChainCount * 0.15f);
			if (chainMultiplier > 1.60f) chainMultiplier = 1.60f;

			float dashSpeed = (player.dashDistance * TILE_SIZE / dashDuration) * chainMultiplier;
			sprite.move(dashDirection * dashSpeed * dt);
			return false; // Return early ONLY during active movement step
		}
	}
	return true;
}



void Player::update(float dt, const sf::RenderWindow& window) {
	// Gestion des entrées clavier pour déplacer le joueur et changer son état
	handleInput(dt);

	// Empêcher le personnage de sortir de la fenêtre de jeu
	sf::Vector2f pos = sprite.getPosition();
	sf::Vector2u size = { MAP_WIDTH * TILE_SIZE, MAP_HEIGHT * TILE_SIZE };

	// Calculer les limites du sprite pour éviter que le personnage ne sorte de la fenêtre
	float halfX = fL / 5.f;
	float halfY = fH / 3.f;

	if (pos.x - halfX < 0) pos.x = halfX;
	if (pos.x + halfX > size.x) pos.x = size.x - halfX;
	if (pos.y - halfY < 0) pos.y = halfY;
	if (pos.y + halfY > size.y) pos.y = size.y - halfY;

	// Appliquer les limites à la position du sprite
	sprite.setPosition(pos);

	// Mise à jour de l'état du joueur en fonction des booléens si le joueur n'est pas verrouillé
	if (!lock) handlePlayerState(playerState, AInfo);

	// Mise à jour de l'animation du joueur en fonction de son état et de sa direction
	updateAnimation(dt);
}



void Player::handleInput(float dt) {
	// Si le mouvement du joueur est verrouillé ou s'il est mort, on ne gère pas les entrées clavier
	if (lock || playerState.dead || playerState.damaged || playerState.healing) return;

	if (!dashMechanic(dt)) return;

	//// Vérifie le cooldown avant de permettre au joueur de dash
	//if (playerState.dash) {
	//	if (playerState.state != PlayerState::DASHING && 
	//		dashCooldownClock.getElapsedTime().asSeconds() >= player.dashCooldown) {
	//		playerState.state = PlayerState::DASHING;
	//		dashCooldownClock.restart();
	//		animator.resetAnimation();
	//	}
	//	playerState.dash = false;
	//}

	//// Mouvement de dash du joueur
	//if (playerState.state == PlayerState::DASHING) {
	//	sf::Vector2f dashDirection = { 0.f, 0.f };

	//	switch (directionInfo.direction) {
	//	case Direction::LEFT:
	//		dashDirection = { -1.f, 0.f };
	//		break;
	//	case Direction::RIGHT:
	//		dashDirection = { 1.f, 0.f };
	//		break;
	//	case Direction::UP:
	//		dashDirection = { 0.f, -1.f };
	//		break;
	//	case Direction::DOWN:
	//		dashDirection = { 0.f, 1.f };
	//		break;
	//	}
	//	// Données pour calculer le temps d,animation pour l'animation de dash
	//	auto dashConfig = playerAnimationMap[PlayerState::DASHING];

	//	// Temps de l'animation de dash
	//	float totalDashTime = dashConfig.maxFrames * dashConfig.frameDuration;
	//	if (totalDashTime > 0.f) {
	//		float dashSpeed = player.dashDistance * TILE_SIZE / totalDashTime;
	//		sprite.move(dashDirection * dashSpeed * dt);
	//	}
	//	return;
	//}

	// Mouvement pour l'attaque du joueur
	if (AInfo.attacking && animator.getCurrentFrame() == 2) {
		float dashSpeed = 50.f;
		switch (directionInfo.direction) {
		case Direction::LEFT:
			AInfo.attackVelocity = { -dashSpeed, 0.f };
			break;
		case Direction::RIGHT:
			AInfo.attackVelocity = { dashSpeed, 0.f };
			break;
		case Direction::UP:
			AInfo.attackVelocity = { 0.f, -dashSpeed };
			break;
		case Direction::DOWN:
			AInfo.attackVelocity = { 0.f, dashSpeed };
			break;
		}
	}

	// Vecteur de mouvement du joueur en fonction des entrées clavier
	sf::Vector2f movement = { 0.f, 0.f };

	// Si le joueur est en train d'attaquer, déplacer légèrement le personnage dans la direction de l'attaque
	sprite.move(AInfo.attackVelocity * dt);

	// Appliquer une friction pour réduire progressivement la vitesse d'attaque
	AInfo.attackVelocity *= 0.9f;

	if (AInfo.attacking) return;

	movement = inputManager.handleMovement(sprite, playerState, player, heldState, directionInfo, AInfo);

	// Défini la vitesse du joueur selon sa direction
	sf::Vector2f targetVelocity = movement * player.speed;

	// Défini le taux d'acceleration/friction du joueur (25.f when starting to move, 15.f when sliding to a stop)
	float speedRate = (movement != sf::Vector2f(0.f, 0.f)) ? 25.f : 15.f;

	// Smoothly interpolate current velocity towards target velocity
	velocity = velocity + (targetVelocity - velocity) * speedRate * dt;

	// Appliquer les calcules du mouvement sur le sprite
	sprite.move(velocity * dt);

	// Gère l'attaque du joueur
	if (AInfo.attackPressed && !AInfo.attacking && AInfo.attackCooldown.getElapsedTime().asSeconds() >= AInfo.attackCooldownTime) {
		playerState.state = PlayerState::ATTACKING;
		AInfo.attacking = true;
		AInfo.attackCooldown.restart();

		animator.resetAnimation();

		//AInfo.attackDirection = directionInfo.direction;
	}

	// Réinitialiser l'état d'attaque après la gestion des entrées clavier
	AInfo.attackPressed = false; 
}



void Player::updateAnimation(float deltaTime) {
	PlayerState state = playerState.state;
	auto config = playerAnimationMap[state];

	// Réduire 1 pour offset l'indexage à 0 des textures
	int targetRow = (config.startRow) + getDirectionOffSet(directionInfo.direction);

	animator.play(targetRow, config.maxFrames, config.frameDuration, config.loop);
	animator.update(deltaTime);

	if (animator.isFinished()) {
		if (state == PlayerState::DAMAGED) playerState.damaged = false;
		else if (state == PlayerState::ATTACKING) AInfo.attacking = false; //playerState.attacking = false;
		else if (state == PlayerState::DASHING) playerState.dash = false;
		else if (state == PlayerState::HEALING) playerState.healing = false;

		playerState.state = PlayerState::IDLE;
	}

	ghostTrails(deltaTime);
}



void Player::ghostTrails(float deltaTime) {
	// === GHOST TRAILS LOGIC ===
		// 1. Spawn ghosts during dashing (every 0.04 seconds)
	if (playerState.state == PlayerState::DASHING && ghostSpawnTimer.getElapsedTime().asSeconds() >= 0.04f) {
		GhostTrail ghost(texture);
		ghost.sprite = sprite; // Copy player's current sprite, position, scale, and frame

		// Tint it cyan/light blue and make it semi-transparent
		ghost.sprite.setColor(sf::Color(0, 191, 255, 120));
		ghost.lifetime = 0.25f; // Dissolves in 0.25 seconds
		ghost.maxLifetime = 0.25f;

		ghosts.push_back(ghost);
		ghostSpawnTimer.restart();
	}

	// 2. Update lifetimes and fade alpha values
	for (auto it = ghosts.begin(); it != ghosts.end();) {
		it->lifetime -= deltaTime;
		if (it->lifetime <= 0.f) {
			it = ghosts.erase(it); // Remove expired ghosts
		}
		else {
			// Fade out based on remaining lifetime ratio
			float ratio = it->lifetime / it->maxLifetime;
			sf::Color color = it->sprite.getColor();
			color.a = static_cast<unsigned int>(ratio * 120);
			it->sprite.setColor(color);
			it++;
		}
	}
}



void Player::handlePlayerState(PlayerStates& playerState, AttackInfo& AInfo) const {
	if (playerState.dead) playerState.state = PlayerState::DEAD;
	else if (playerState.damaged) playerState.state = PlayerState::DAMAGED;
	else if (playerState.healing) playerState.state = PlayerState::HEALING;
	else if (AInfo.attacking) playerState.state = PlayerState::ATTACKING;
	else if (playerState.state == PlayerState::DASHING);
	else if (playerState.state == PlayerState::EXHAUSTED);
	else if (playerState.dash) playerState.state = PlayerState::DASHING;
	else if (playerState.run) playerState.state = PlayerState::RUNNING;
	else if (playerState.jog) playerState.state = PlayerState::JOGGING;
	else if (playerState.walk) playerState.state = PlayerState::WALKING;
	else playerState.state = PlayerState::IDLE;
}



void Player::handleEvent(const sf::Event& event)
{
	// Si le mouvement du joueur est verrouillé, on ne gère pas les événements clavier
	if (lock) return;

	inputManager.handleEvent(event, heldState, directionInfo, AInfo, playerState, inventory);
}



void Player::draw(sf::RenderWindow& window) {
	// Draw ghost trails first (so they are drawn underneath the player)
	for (const auto& ghost : ghosts) {
		window.draw(ghost.sprite);
	}

	window.draw(sprite);
}
