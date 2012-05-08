#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include "ResourcePath.hpp"

#include "MonsterMatrix.hpp"
#include "Shot.hpp"

#define WIN_W 800
#define WIN_H 600

#define SHIP_W 46
#define SHIP_H 28

sf::Vector2f getSpriteCenter(sf::Sprite&);

int main (int argc, const char* argv[])
{
    // ==============
    // Init Resources
    // ==============
    sf::RenderWindow window(sf::VideoMode(WIN_W, WIN_H), "Space Invaders");
    
    // Load Ship
    sf::Texture shipTexture;
    if (!shipTexture.loadFromFile(resourcePath() + "ship.png"))
        return EXIT_FAILURE;
    sf::Sprite shipSprite(shipTexture);
    
    // Load Shots
    sf::Texture shotTexture;
    if (!shotTexture.loadFromFile(resourcePath() + "shot.png"))
        return EXIT_FAILURE;
    
    Shot shipShot(shotTexture, Shot::UP, 0.5);
    Shot monsterShot(shotTexture, Shot::DOWN, 0.3);
    
    // Score
    int score = 0;
    sf::Font font;
    if (!font.loadFromFile(resourcePath() + "sansation.ttf"))
        return EXIT_FAILURE;
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setOrigin(-10, -10);
    
    // Load Monsters
    MonsterMatrix matrix(sf::FloatRect(20, 20, 760, 400), monsterShot);
    
    while (window.isOpen())
    {
        // ============
        // Handle Input
        // ============
        
        // Handle mouse position
        float shipX = sf::Mouse::getPosition(window).x;
        float shipOffset = (SHIP_W / 2);
        
        if (shipX < shipOffset) {
            shipX = 0;
        }
        else if (shipX > (WIN_W - SHIP_W / 2)) {
            shipX = (WIN_W - SHIP_W);
        }
        else {
            shipX -= shipOffset;
        }
        
        shipSprite.setPosition(shipX, (WIN_H - 50));
        
        // ======
        // Events
        // ======
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (!shipShot.alive &&
                event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                
                shipShot.spawnAt(
                    getSpriteCenter(shipSprite).x,
                    shipSprite.getPosition().y
                );
            }
        }
        
        // Move objects
        matrix.step();
        shipShot.step();
        monsterShot.step();
        
        // Handle collisions
        score += matrix.strikeWith(shipShot);
        
        // Format score text
        char * scoreBuffer;
        std::sprintf(scoreBuffer, "SCORE: %d", score);
        scoreText.setString(sf::String(scoreBuffer));
        
        // ======
        // Render
        // ======
        window.clear();
        
        window.draw(shipSprite);
        window.draw(matrix);
        window.draw(shipShot);
        window.draw(monsterShot);
        window.draw(scoreText);
        
        window.display();   
    }

	return EXIT_SUCCESS;
}

sf::Vector2f getSpriteCenter(sf::Sprite &sprite)
{
    sf::FloatRect rect = sprite.getGlobalBounds();
    return sf::Vector2f(rect.width / 2 + rect.left, rect.height / 2 + rect.top);
}
