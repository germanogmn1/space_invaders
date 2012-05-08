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

void splashScreen(sf::RenderWindow& window)
{
    sf::Texture splashTexture;
    splashTexture.loadFromFile(resourcePath() + "splash.png");
    sf::Sprite splashSprite(splashTexture);
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape)
                window.close();
            
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Space)
                return;
        }
        
        window.clear();
        window.draw(splashSprite);
        window.display();
    }
}

void gameOverScreen(sf::RenderWindow& window, int score, sf::Font& font)
{
    sf::Texture splashTexture;
    splashTexture.loadFromFile(resourcePath() + "game_over.png");
    sf::Sprite splashSprite(splashTexture);
    
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setOrigin(-300, -400);
    
    char scoreBuffer[20];
    std::sprintf(scoreBuffer, "SCORE: %d", score);
    scoreText.setString(sf::String(scoreBuffer));
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape)
                window.close();
        }
        
        window.clear();
        window.draw(splashSprite);
        window.draw(scoreText);
        window.display();
    }
}

int main (int argc, const char * argv[])
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
    
    Shot shipShot(shotTexture, Shot::UP, 0.01);
    Shot monsterShot(shotTexture, Shot::DOWN, 0.3);
    
    // Font for interface
    sf::Font font;
    if (!font.loadFromFile(resourcePath() + "Andale Mono.ttf"))
        return EXIT_FAILURE;
    
    // Score 
    int score = 0;
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setOrigin(-10, -10);
    
    // Lives
    int lives = 3;
    sf::Text livesText;
    livesText.setFont(font);
    livesText.setCharacterSize(20);
    livesText.setOrigin(-700, -10);
    
    // Load Monsters
    MonsterMatrix matrix(sf::FloatRect(20, 40, 760, 380), monsterShot);
    
    // =============
    // Splash Screen
    // =============
    splashScreen(window);
    
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
                
                sf::FloatRect shipRect = shipSprite.getGlobalBounds();
                shipShot.spawnAt(
                    shipRect.width / 2 + shipRect.left,
                    shipSprite.getPosition().y
                );
            }
        }
        
        // Move objects
        matrix.step();
        shipShot.step();
        monsterShot.step();
        
        // Handle collisions
        if (monsterShot.alive &&
            monsterShot.sprite.getGlobalBounds().intersects(shipSprite.getGlobalBounds())) {
            lives--;
            monsterShot.alive = false;
        }
        
        if (!lives) {
            gameOverScreen(window, score, font);
            return EXIT_SUCCESS;
        }
        
        score += matrix.strikeWith(shipShot);
        
        // Score text
        char scoreBuffer[20];
        std::sprintf(scoreBuffer, "SCORE: %d", score);
        scoreText.setString(sf::String(scoreBuffer));
        
        // Lives text
        char livesBuffer[20];
        std::sprintf(livesBuffer, "LIVES: %d", lives);
        livesText.setString(sf::String(livesBuffer));
        
        // ======
        // Render
        // ======
        window.clear();
        
        window.draw(shipSprite);
        window.draw(matrix);
        window.draw(shipShot);
        window.draw(monsterShot);
        window.draw(scoreText);
        window.draw(livesText);
        
        window.display();
    }

	return EXIT_SUCCESS;
}

sf::Vector2f getSpriteCenter(sf::Sprite &sprite)
{
    sf::FloatRect rect = sprite.getGlobalBounds();
    return sf::Vector2f(rect.width / 2 + rect.left, rect.height / 2 + rect.top);
}
