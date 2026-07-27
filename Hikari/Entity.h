////////////////////////////////////////////////////////////////
// \brief Base class for all entities in the game
// This class is abstract and cannot be instantiated directly. It defines the common interface for all entities, such as updating and drawing.
// Each entity has a sprite and a texture, which can be used to display the entity on the screen. The position of the entity can be set and retrieved using the getPosition and setPosition methods.
// \author Aboubacar Sanogo
// \version 1.0
///////////////////////////////////////////////////////////////

#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>

//===============================
// Classe Entity qui sert de base pour toutes les entités du jeu (joueur, ennemis, objets, etc.)
// Cette classe est abstraite et ne peut pas être instanciée directement. Elle définit l'interface commune pour toutes les entités, telles que la mise à jour et le dessin.
// Chaque entité a un sprite et une texture, qui peuvent être utilisés pour afficher l'entité à l'écran. La position de l'entité peut être définie et récupérée à l'aide des méthodes getPosition et setPosition.
//===============================
class Entity {
public:
	virtual ~Entity() = default;

	virtual void update(float deltaTime, const sf::RenderWindow& window) = 0;
	virtual void draw(sf::RenderWindow& window) = 0;

	sf::Vector2f getPosition() const { return sprite.getPosition(); }
	void setPosition(const sf::Vector2f& pos) { sprite.setPosition(pos); }

	sf::Sprite& getSprite() {
		return sprite;
	}

	const sf::Sprite& getSprite() const {
		return sprite;
	}

protected:
	sf::Sprite sprite;
	sf::Texture texture;
};