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

#define BONUS_SHIP_CHANCE 10000
#define BONUS_SHIP_VEL 0.15f

sf::Vector2f getSpriteCenter(sf::Sprite&);
void splashScreen(sf::RenderWindow&);
void gameOverScreen(sf::RenderWindow&, int, sf::Font&);

int main (int argc, const char * argv[])
{
    // ==============
    // Init Resources
    // ==============
    std::srand((unsigned) std::time(0));
    sf::RenderWindow window(sf::VideoMode(WIN_W, WIN_H), "Space Invaders");
    
    // Load Player Ship
    sf::Texture shipTexture;
    if (!shipTexture.loadFromFile(resourcePath() + "ship.png"))
        return EXIT_FAILURE;
    sf::Sprite shipSprite(shipTexture);
    
    // Load Shots
    sf::Texture shotTexture;
    if (!shotTexture.loadFromFile(resourcePath() + "shot.png"))
        return EXIT_FAILURE;
    
    // Load Bonus Ship
    sf::Texture bonusShipTexture;
    if (!bonusShipTexture.loadFromFile(resourcePath() + "bonus_ship.png"))
        return EXIT_FAILURE;
    sf::Sprite bonusShipSprite(bonusShipTexture);
    bool isBonusShipAlive = false;
    sf::Vector2f bonusShipPos(0, 35);
    bonusShipSprite.setPosition(bonusShipPos);
    
    Shot shipShot(shotTexture, Shot::UP, 0.6);
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
    
    // Level
    sf::Text levelText;
    levelText.setFont(font);
    levelText.setCharacterSize(20);
    levelText.setOrigin(-350, -10);
    
    // Lives
    int lives = 3;
    sf::Text livesText;
    livesText.setFont(font);
    livesText.setCharacterSize(20);
    livesText.setOrigin(-700, -10);
    
    // Load Monsters
    MonsterMatrix matrix(sf::FloatRect(20, 60, 760, 380), monsterShot);
    
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
        
        // Bonus ship
        if (isBonusShipAlive) {
            if (bonusShipPos.x > WIN_W) {
                isBonusShipAlive = false;
            }
            else {
                bonusShipPos.x += BONUS_SHIP_VEL;
                bonusShipSprite.setPosition(bonusShipPos);
            }
        }
        else {
            if (std::rand() % BONUS_SHIP_CHANCE == 0) {
                isBonusShipAlive = true;
                bonusShipPos.x = 0;
            }
        }
        
        
        // Handle collisions
        if (monsterShot.alive &&
            monsterShot.sprite.getGlobalBounds().intersects(shipSprite.getGlobalBounds())) {
            lives--;
            monsterShot.alive = false;
        }
        
        // Handle collisions
        if (shipShot.alive &&
            shipShot.sprite.getGlobalBounds().intersects(bonusShipSprite.getGlobalBounds())) {
            isBonusShipAlive = false;
            bonusShipPos.x = 0;
            score += 50;
            shipShot.alive = false;
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
        
        // Level text
        char levelBuffer[20];
        std::sprintf(levelBuffer, "LEVEL: %d", matrix.level);
        levelText.setString(sf::String(levelBuffer));
        
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
        window.draw(levelText);
        window.draw(livesText);
        
        if (isBonusShipAlive) {
            window.draw(bonusShipSprite);
        }
        
        window.display();
    }

	return EXIT_SUCCESS;
}

sf::Vector2f getSpriteCenter(sf::Sprite &sprite)
{
    sf::FloatRect rect = sprite.getGlobalBounds();
    return sf::Vector2f(rect.width / 2 + rect.left, rect.height / 2 + rect.top);
}


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
