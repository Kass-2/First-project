//===============================
// Fichier : Player.cpp
// Description : Implémentation de la classe Player pour gérer le personnage du joueur
// Auteur : Aboubacar Sanogo
// Version : 1.0
//===============================

//===============================
// Inclusion des bibliothèques nécessaires
//===============================
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "Player.h"
#include "MapData.h"



Player::Player(const std::string& tf, int frameWidth, int frameHeight)
	: playerFrames({ 0.3f, 8, 0, -1 }), rows({ 0 }), 
	textureFile(tf), fL(frameWidth), fH(frameHeight)
{
	// Chargement de la texture du joueur
	if (!texture.loadFromFile(textureFile))
	{
		std::cout << "Failed to load player texture!" << std::endl;
		return;
	}

	sprite.setTexture(texture);

	// Calcul du nombre de colonnes et de lignes dans la texture
	int x = columns * fL;                   // Position X de la partie de la texture à afficher
	int y = lignes * fH;                    // Position Y de la partie de la texture à afficher

	// Sprite du joueur : on affiche la partie de la texture qui correspond au personnage (32x32 pixels)
	sf::IntRect r({ x,y }, { fL,fH });
	rectSource = r;
	sprite.setTextureRect(rectSource);

	sprite.setOrigin({ fL/2.f, fH/2.f });       // Centrer l'origine du sprite
	sprite.setPosition({ 400.f, 300.f });   // Position initiale du joueur au centre de la fenêtre
	sprite.setScale({ 1.0f, 1.0f });        // Modifier la taille du sprite du joueur

	// Initialiser les positions X et Y du rectangle source pour l'animation du joueur
	rectSource.position.x = 0;

	// Les lignes pour l'état idle sont différentes pour chaque direction
	int IdleRow =
		DInfo.direction == Direction::DOWN ? 9 :
		DInfo.direction == Direction::UP ? 10 :
		DInfo.direction == Direction::LEFT ? 12 :
		DInfo.direction == Direction::RIGHT ? 11 : 9;

	IdleRow -= 1;

	// Initialiser la position Y du rectangle source en fonction de la direction du joueur
	rectSource.position.y = IdleRow * fH;

	sprite.setTextureRect(rectSource);

	playerState.previousState = playerState.state;
	DInfo.previousDirection = DInfo.direction;

	animationClock.restart();
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
	player.dashDistance = 2;
	player.dashCooldown = 1.f;
}



float Player::getHealth() const {
	return player.health;
}



float Player::getMaxHealth() const {
	return player.maxHealth;
}



void Player::handleInput(float dt) {
	// Si le mouvement du joueur est verrouillé ou s'il est mort, on ne gère pas les entrées clavier
	if (lock || playerState.dead || playerState.damaged || playerState.healing) return;

	// Vérifie le cooldown avant de permettre au joueur de dash
	if (playerState.dash) {
		if (playerState.state != PlayerState::DASHING && 
			dashCooldownClock.getElapsedTime().asSeconds() >= player.dashCooldown) {
			playerState.state = PlayerState::DASHING;
			dashCooldownClock.restart();
			PlayerFrameAnimation();
		}
		playerState.dash = false;
	}

	// Mouvement de dash du joueur
	if (playerState.state == PlayerState::DASHING) {
		//PlayerFrameAnimation();

		sf::Vector2f dashDirection = { 0.f, 0.f };

		switch (DInfo.direction) {
		case Direction::LEFT:
			dashDirection = { -1.f, 0.f };
			break;
		case Direction::RIGHT:
			dashDirection = { 1.f, 0.f };
			break;
		case Direction::UP:
			dashDirection = { 0.f, -1.f };
			break;
		case Direction::DOWN:
			dashDirection = { 0.f, 1.f };
			break;
		}
		// Temps de l'animation de dash
		float totalDashTime = playerFrames.mf * playerFrames.ft;
		if (totalDashTime > 0.f) {
			float dashSpeed = player.dashDistance * TILE_SIZE / totalDashTime;
			sprite.move(dashDirection * dashSpeed * dt);
		}
		return;
	}

	// Vecteur de mouvement du joueur en fonction des entrées clavier
	sf::Vector2f movement = { 0.f, 0.f };

	// Si le joueur est en train d'attaquer, déplacer légèrement le personnage dans la direction de l'attaque
	sprite.move(AInfo.attackVelocity * dt);

	// Appliquer une friction pour réduire progressivement la vitesse d'attaque
	AInfo.attackVelocity *= 0.9f;

	movement = inputManager.handleMovement(sprite, playerState, player, heldState, DInfo, AInfo);

	sprite.move(movement * player.speed * dt);

	if (AInfo.attackPressed && !AInfo.attacking && AInfo.attackCooldown.getElapsedTime().asSeconds() >= AInfo.attackCooldownTime) {
		playerState.state = PlayerState::ATTACKING;
		AInfo.attacking = true;
		AInfo.attackCooldown.restart();

		AInfo.attackDirection = DInfo.direction;
	}

	AInfo.attackPressed = false; // Réinitialiser l'état d'attaque après la gestion des entrées clavier
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



void Player::updateAnimation(float deltaTime) {
	// Variable pour l'animation du joueur : on utilise la largeur et la hauteur d'une frame pour calculer les positions dans la texture
	Animations playerAnimation(fH, fL);

	// Determiner les lignes de la texture pour chaque état du joueur en fonction de la direction du joueur
	handleRows(DInfo, rows);

	int targetRow = 0;
	currentLineTexture(targetRow);

	// Vérifier si la ligne de la texture a changé
	bool rowChanged = !AInfo.attacking && (rectSource.position.y != targetRow * fH);

	// Déterminer si l'animation doit être réinitialisée
	bool animationChanged =
		(playerState.state != playerState.previousState)
		|| (!AInfo.attacking && DInfo.direction != DInfo.previousDirection)
		|| rowChanged;

	// Déterminer la ligne de la texture à utiliser en fonction de l'état du joueur
	resetPlayer(animationChanged, rows, playerAnimation);

	// Mettre à jour les frames de l'animation du joueur en fonction de son état
	PlayerFrameAnimation();

	// Mettre à jour l'animation du joueur en fonction du temps écoulé
	if (animationClock.getElapsedTime().asSeconds() >= playerFrames.ft) {
		rectSource.position.x += fL;

		// Mettre à jour les frames précédentes et actuelles de l'animation du joueur
		playerFrames.cf = rectSource.position.x / fL;

		// Pendant l'attaque, maintenir la direction précédente pour que le personnage ne change pas de direction pendant l'animation d'attaque
		if (AInfo.attacking)
		{
			// Mémoriser la direction de l'attaque pour éviter que le personnage ne change de direction pendant l'animation d'attaque
			DInfo.direction = AInfo.attackDirection;

			// De plus, déplacer légèrement le personnage dans la direction de l'attaque pendant les premières frames
			// dans la direction de l'attaque pour donner une impression de mouvement
			if (playerFrames.cf == 2 && playerFrames.pf != 2)
			{
				float dashSpeed = 50.f;

				switch (DInfo.direction)
				{
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
		}

		// TODO : Revoir ce code
		if (playerFrames.cf >= 8)
		{
			// Remettre le personnage un peu en arrière à la fin de l'animation
			sprite.move(-AInfo.attackVelocity * 0.5f); // Ajustez la valeur pour contrôler la distance de recul)
		}

		if (rectSource.position.x >= playerFrames.mf * fL) {
			// Si l'animation est terminée, on réinitialise la position de la source en fonction de l'état du joueur
			if (playerState.state == PlayerState::DEAD)
			{
				rectSource.position.x = (playerFrames.mf - 1) * fL;
			}
			else if (playerState.state == PlayerState::DAMAGED)
			{
				// Réinitialiser l'état de dommage après l'animation
				playerState.damaged = false;

				rectSource.position.x = 0;
				
				// Revenir à l'état idle après l'animation de dommage
				playerState.state = PlayerState::IDLE;
			}
			else if (playerState.state == PlayerState::ATTACKING)
			{
				// Réinitialiser l'état d'attaque après l'animation
				AInfo.attacking = false;

				rectSource.position.x = 0;

				// Revenir à l'état idle après l'animation d'attaque
				playerState.state = PlayerState::IDLE;
			}
			else if (playerState.state == PlayerState::DASHING)
			{
				// Réinitialiser l'état d'attaque après l'animation
				playerState.dash = false;

				rectSource.position.x = 0;

				// Revenir à l'état idle après l'animation d'attaque
				playerState.state = PlayerState::IDLE;
			}
			else if (playerState.state == PlayerState::HEALING)
			{
				// Réinitialiser l'état de soin après l'animation
				playerState.healing = false;

				rectSource.position.x = 0;

				// Revenir à l'état idle après l'animation de soin
				playerState.state = PlayerState::IDLE;
			}
			else
			{
				rectSource.position.x = 0;
			}
		}
		sprite.setTextureRect(rectSource);
		animationClock.restart();
	}
	playerFrames.pf = playerFrames.cf;
}



void Player::PlayerFrameAnimation() {
	switch (playerState.state) {
		case PlayerState::DASHING:
			// Animation pour l'état de dashing
			StateFrameTime(0.03f, 7);
			break;
		case PlayerState::RUNNING:
			// Animation pour l'état de course
			StateFrameTime(0.06, 8);
			break;
		case PlayerState::WALKING:
			// Animation pour l'état de marche
			StateFrameTime(0.1f, 8);
			break;
		case PlayerState::ATTACKING:
			// Animation pour l'état d'attaque
			StateFrameTime(0.05, 8);
			break;
		case PlayerState::IDLE:
			// Animation pour l'état idle
			StateFrameTime(0.2, 8);
			break;
		case PlayerState::DAMAGED:
			// Animation pour l'état de dommage
			StateFrameTime(0.05, 4);
			break;
		case PlayerState::DEAD:
			// Animation pour l'état de mort
			StateFrameTime(0.1, 7);
			break;
		case PlayerState::HEALING:
			// Animation pour l'état de soin
			StateFrameTime(0.1, 8);
			break;
	}
}



void Player::StateFrameTime(const float frameTime, const int maxFrames)
{
	// Déterminer le temps entre les frames et le nombre de frames en fonction de l'état du joueur
	playerFrames.ft = frameTime;
	playerFrames.mf = maxFrames;
	
}



void Player::currentLineTexture(int& targetRow) const
{
	if (playerState.state == PlayerState::IDLE)
		targetRow = rows.idleRow;
	else if (playerState.state == PlayerState::WALKING)
		targetRow = rows.walkingRow;
	else if (playerState.state == PlayerState::RUNNING)
		targetRow = rows.runningRow;
	else if (playerState.state == PlayerState::DASHING)
		targetRow = rows.dashRow;
	else if (playerState.state == PlayerState::ATTACKING)
		targetRow = rows.attackingRow;
	else if (playerState.state == PlayerState::DAMAGED)
		targetRow = rows.damagedRow;
	else if (playerState.state == PlayerState::DEAD)
		targetRow = rows.deadRow;
	else if (playerState.state == PlayerState::HEALING)
		targetRow = rows.healingRow;
}



void Player::handlePlayerState(PlayerStates& playerState, AttackInfo& AInfo) const {
	if (playerState.dead) playerState.state = PlayerState::DEAD;
	else if (playerState.damaged) playerState.state = PlayerState::DAMAGED;
	else if (playerState.healing) playerState.state = PlayerState::HEALING;
	else if (AInfo.attacking) playerState.state = PlayerState::ATTACKING;

	else if (playerState.state == PlayerState::DASHING);

	else if (playerState.dash) playerState.state = PlayerState::DASHING;
	else if (playerState.run) playerState.state = PlayerState::RUNNING;
	else if (playerState.walk) playerState.state = PlayerState::WALKING;
	else playerState.state = PlayerState::IDLE;
}



void Player::handleRows(DirectionInfo& DInfo, SpriteRows& rows) const {
	// Les lignes pour l'état d'attaque sont différentes pour chaque direction
	rows.attackingRow =
		DInfo.direction == Direction::DOWN ? 1 :
		DInfo.direction == Direction::UP ? 2 :
		DInfo.direction == Direction::LEFT ? 4 :
		DInfo.direction == Direction::RIGHT ? 3 : 1;
	// Les lignes pour l'état de marche sont différentes pour chaque direction
	rows.walkingRow =
		DInfo.direction == Direction::DOWN ? 33 :
		DInfo.direction == Direction::UP ? 34 :
		DInfo.direction == Direction::LEFT ? 36 :
		DInfo.direction == Direction::RIGHT ? 35 : 33;
	// Les lignes pour l'état de course sont différentes pour chaque direction
	rows.runningRow =
		DInfo.direction == Direction::DOWN ? 29 :
		DInfo.direction == Direction::UP ? 30 :
		DInfo.direction == Direction::LEFT ? 32 :
		DInfo.direction == Direction::RIGHT ? 31 : 29;
	// Les lignes pour l'état de dash sont différentes pour chaque direction
	rows.dashRow =
		DInfo.direction == Direction::DOWN ? 9 :
		DInfo.direction == Direction::UP ? 10 :
		DInfo.direction == Direction::LEFT ? 12 :
		DInfo.direction == Direction::RIGHT ? 11 : 9;
	// Les lignes pour l'état idle sont différentes pour chaque direction
	rows.idleRow =
		DInfo.direction == Direction::DOWN ? 25 :
		DInfo.direction == Direction::UP ? 26 :
		DInfo.direction == Direction::LEFT ? 28 :
		DInfo.direction == Direction::RIGHT ? 27 : 25;

	// Les lignes pour l'état de dommage sont différentes pour chaque direction
	rows.damagedRow =
		DInfo.direction == Direction::DOWN ? 21 :
		DInfo.direction == Direction::UP ? 22 :
		DInfo.direction == Direction::LEFT ? 24 :
		DInfo.direction == Direction::RIGHT ? 23 : 21;

	// Les lignes pour l'état de mort sont différentes pour chaque direction
	rows.deadRow =
		DInfo.direction == Direction::DOWN ? 13 :
		DInfo.direction == Direction::UP ? 14 :
		DInfo.direction == Direction::LEFT ? 16 :
		DInfo.direction == Direction::RIGHT ? 15 : 13;

	// Les lignes pour l'état de soin sont différentes pour chaque direction
	rows.healingRow =
		DInfo.direction == Direction::DOWN ? 17 :
		DInfo.direction == Direction::UP ? 18 :
		DInfo.direction == Direction::LEFT ? 20 :
		DInfo.direction == Direction::RIGHT ? 19 : 17;

	// Soustraire 1 à chaque ligne pour correspondre à l'index de la texture (commence à 0)
	if (rows.attackingRow > 0)
		rows.attackingRow -= 1;
	if (rows.walkingRow > 0)
		rows.walkingRow -= 1;
	if (rows.runningRow > 0)
		rows.runningRow -= 1;
	if (rows.dashRow > 0)
		rows.dashRow -= 1;
	if (rows.idleRow > 0)
		rows.idleRow -= 1;
	if (rows.damagedRow > 0)
		rows.damagedRow -= 1;
	if (rows.deadRow > 0)
		rows.deadRow -= 1;
	if (rows.healingRow > 0)
		rows.healingRow -= 1;
}



void Player::resetPlayer(bool animationChanged, SpriteRows rows, Animations reset) {
	// Si l'état du joueur a changé, on réinitialise l'animation
	if (animationChanged)
	{
		// Restaurer la santé du joueur si l'état précédent était "mort" et que l'état actuel n'est pas "mort"
		if (playerState.previousState == PlayerState::DEAD && playerState.state != PlayerState::DEAD)
			player.health = player.maxHealth;

		if (playerState.state == PlayerState::IDLE)
			reset.resetAnimation(rectSource, rows.idleRow);
		else if (playerState.state == PlayerState::WALKING)
			reset.resetAnimation(rectSource, rows.walkingRow);
		else if (playerState.state == PlayerState::RUNNING)
			reset.resetAnimation(rectSource, rows.runningRow);
		else if (playerState.state == PlayerState::DASHING)
			reset.resetAnimation(rectSource, rows.dashRow);
		else if (playerState.state == PlayerState::HEALING) 
		{
			// Réinitialiser l'animation de soin et augmenter la santé du joueur
			reset.resetAnimation(rectSource, rows.healingRow);
			player.health += 10;
			if (player.health > player.maxHealth) 
				player.health = player.maxHealth;
		}
		else if (playerState.state == PlayerState::ATTACKING)
			reset.resetAnimation(rectSource, rows.attackingRow);
		else if (playerState.state == PlayerState::DAMAGED)
		{
			// Réinitialiser l'animation de dommage et diminuer la santé du joueur
			reset.resetAnimation(rectSource, rows.damagedRow);
			player.health -= 10;
			if (player.health <= 0.f) {
				player.health = 0.f;
				playerState.dead = true;
			}
		}
		else if (playerState.state == PlayerState::DEAD)
		{
			reset.resetAnimation(rectSource, rows.deadRow);
			player.health = 0.f;
		}

		sprite.setTextureRect(rectSource);

		animationClock.restart();
		playerState.previousState = playerState.state;
		DInfo.previousDirection = DInfo.direction;
	}
}



void Player::handleEvent(const sf::Event& event)
{
	// Si le mouvement du joueur est verrouillé, on ne gère pas les événements clavier
	if (lock) return;

	inputManager.handleEvent(event, heldState, DInfo, AInfo, playerState, inventory);
}



//sf::Vector2f Player::getPosition(const sf::Sprite& sprite) const
//{
//	return sprite.getPosition();
//}



void Player::draw(sf::RenderWindow& window) {
	window.draw(sprite);
}