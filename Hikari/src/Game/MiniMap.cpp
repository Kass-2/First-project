//===============================
// Fichier : MiniMap.cpp
// Description : Implémentation de la classe MiniMap pour gérer la mini-carte
// Auteur : Aboubacar Sanogo
// Version : 1.0
//===============================

//===============================
// Inclusion des bibliothèques nécessaires
//===============================
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "C:\MyProjects\Hikari\Hikari\include\Game\MiniMap.h"
#include "../../include/Game/Player.h"
#include "../../include/Game/TileMap.h"



MiniMap::MiniMap(float mapWidth, float mapHeight, float windowWidth, float windowHeight, int tileSize) {
	// Convertir les dimensions de la carte en pixels
    mapWidth *= tileSize;
	mapHeight *= tileSize;
    
    // Définir la taille de la mini-carte comme 18.75% de la taille de la fenêtre
    float viewWidth = windowWidth * 0.1875f;
    float viewHeight = windowHeight * 0.1875f;

    // Position 20px from top-right corner
    float viewX = windowWidth - viewWidth - 20.f;
    float viewY = 20.f;

	// Positionner la vue de la mini-carte dans le coin supérieur droit de la fenêtre
    view.setViewport(sf::FloatRect(
        { viewX / windowWidth, viewY / windowHeight },
        { viewWidth / windowWidth, viewHeight / windowHeight }
    ));

	// Mettre à l'échelle la vue pour correspondre à la taille de la carte
    view.setSize({ mapWidth, mapHeight });
    view.setCenter({ mapWidth / 2.f, mapHeight / 2.f });

	// Arrière-plan semi-transparent (UI coordinates)
    background.setSize({ viewWidth, viewHeight });
    background.setPosition({ viewX, viewY });
    background.setFillColor(sf::Color(25, 25, 25, 180));

	// Bordure de la mini-carte (UI coordinates)
    border.setSize({ viewWidth, viewHeight });
    border.setPosition({ viewX, viewY });
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(sf::Color(0, 220, 255, 200)); // Glowing
    border.setOutlineThickness(2.f);

	// Indicateur du joueur sur la mini-carte (Vibrant Green)
    playerDot.setRadius(12.f);
    playerDot.setFillColor(sf::Color(50, 255, 50));
    playerDot.setOutlineColor(sf::Color::White);
    playerDot.setOutlineThickness(2.f);
    playerDot.setOrigin({ 12.f, 12.f }); // Center origin

    // Indicateur du personnage de test (Vibrant Red)
    testCharDot.setRadius(12.f);
    testCharDot.setFillColor(sf::Color(255, 50, 50));
    testCharDot.setOutlineColor(sf::Color::White);
    testCharDot.setOutlineThickness(2.f);
    testCharDot.setOrigin({ 12.f, 12.f });
}



void MiniMap::updateWindowSize(float windowWidth, float windowHeight) {
    float viewWidth = windowWidth * 0.1875f;
    float viewHeight = windowHeight * 0.1875f;
    float viewX = windowWidth - viewWidth - 20.f;
    float viewY = 20.f;

    view.setViewport(sf::FloatRect(
        { viewX / windowWidth, viewY / windowHeight },
        { viewWidth / windowWidth, viewHeight / windowHeight }
    ));
    background.setSize({ viewWidth, viewHeight });
    background.setPosition({ viewX, viewY });
    border.setSize({ viewWidth, viewHeight });
    border.setPosition({ viewX, viewY });
}



void MiniMap::handleEvent(const sf::Event & event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::M) {
            miniMapVisible = !miniMapVisible;
        }
    }
}



void MiniMap::draw(sf::RenderWindow & window, const TileMap & tileMap, 
    const Player & player, const Player & testChar) 
{
    if (!miniMapVisible) return;

	// Sauvegarder la vue actuelle de la fenêtre pour la restaurer après le dessin de la mini-carte
    const sf::View oldView = window.getView();

    // Dessiner le fond de la mini-carte (UI)
    window.setView(window.getDefaultView());
    window.draw(background);

	// Dessiner la mini-carte avec la vue de la carte
    window.setView(view);
    window.draw(tileMap);

    // Mettre à jour les positions et dessiner les indicateurs
    playerDot.setPosition(player.getPosition());
    window.draw(playerDot);

    testCharDot.setPosition(testChar.getPosition());
    window.draw(testCharDot);

	// Dessiner la bordure de la mini-carte (UI)
    window.setView(window.getDefaultView());
    window.draw(border);

	// Restaurer la vue précédente de la fenêtre
    window.setView(oldView);
}
