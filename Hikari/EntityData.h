//===============================
// Fichier : EntityData.h
// Description : Données pour les différentes entités du jeu.
// Auteur : Aboubacar Sanogo
//===============================

#pragma once

//===============================
// Inclusion des bibliothèques
//===============================
#include <unordered_map>

#include "Entity.h"
#include "Animator.h"



std::unordered_map<PlayerState, AnimationConfig> playerAnimationMap = {
	{ PlayerState::IDLE,		{ 24, 8, 0.2f,  true } },
	{ PlayerState::WALKING,		{ 32, 8, 0.1f,  true } },
	{ PlayerState::JOGGING,		{ 28, 8, 0.08f, true } },
	{ PlayerState::RUNNING,		{ 28, 8, 0.06f, true } },
	{ PlayerState::DASHING,		{ 8,  7, 0.03f, false } },
	{ PlayerState::ATTACKING,	{ 0,  8, 0.05f, false } },
	{ PlayerState::EXHAUSTED,	{ 20,  1, 0.4f, false } },
	{ PlayerState::DAMAGED,		{ 20, 4, 0.05f, false } },
	{ PlayerState::HEALING,		{ 16, 8, 0.1f,  false } },
	{ PlayerState::DEAD,		{ 12, 7, 0.1f,  false } }
};
