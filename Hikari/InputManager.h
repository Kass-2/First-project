//====================================
// Fichier : InputManager.h
// Description : Implémentation de la classe InputManager pour gérer les entrées clavier du joueur
// Auteur : Aboubacar Sanogo
// Version : 1.0
//====================================

#pragma once

//===============================
// Inclusion des bibliothèques
//===============================
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

#include "Entity.h"



class InputManager
{
public:
	//===============================
	// Fonction pour gérer les entrées clavier du joueur et mettre à jour les booléens correspondants
	//===============================
	sf::Vector2f handleMovement(
		sf::Sprite& sprite, 
		PlayerStates& playerState, 
		PlayerInfo& player, 
		HeldDirection& heldState, 
		DirectionInfo& directionInfo,
		AttackInfo& AInfo) const;

	//===============================
	// Fonction pour gérer les événements clavier et mettre à jour les booléens correspondants
	//===============================
	void handleEvent(const sf::Event& event, 
		HeldDirection& heldState, 
		DirectionInfo& directionInfo, 
		AttackInfo& AInfo, 
		PlayerStates& playerState, 
		bool& inventory);

private:
	// Énumération pour représenter l'axe de mouvement du joueur
	enum class Axis { NONE, HORIZONTAL, VERTICAL };

	// Variable pour mémoriser le dernier axe de mouvement du joueur
	Axis lastPressedAxis = Axis::NONE;
};