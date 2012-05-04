#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include "ResourcePath.hpp"

#include "MonsterMatrix.hpp"

#define WIN_W  800
#define WIN_H 600

#define SHIP_W 46
#define SHIP_H 28

#define SHOT_W 5
#define SHOT_H 15

#define SHOT_VEL 0.5f

sf::Vector2f getSpriteCenter(sf::Sprite&);
bool spritesColide(sf::Sprite&, sf::Sprite&);

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
    
    // Load Shot
    sf::Texture shotTexture;
    if (!shotTexture.loadFromFile(resourcePath() + "shot.png"))
        return EXIT_FAILURE;
    sf::Sprite shotSprite(shotTexture);
    
    bool shot = false;
    sf::Vector2f shotPos(0, 0);
    
    // Load Monsters
    MonsterMatrix matrix(sf::FloatRect(20, 20, 760, 400));
    
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
        
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (!shot && event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                // Start shot centered on the ship
                shot = true;
                shotPos.x = getSpriteCenter(shipSprite).x - (SHOT_W / 2);
                shotPos.y = shipSprite.getPosition().y - SHOT_H;
            }
        }
        
        if (shot) {
            // Handle Shot Position
            shotPos.y -= SHOT_VEL;
            shotSprite.setPosition(shotPos);
            
            if (shotPos.y < 0)
                shot = false;
            
            // Handle shot colision
            if (matrix.collides(shotSprite)) {
                shot = false;
            }
            
        }
        
        matrix.step();
        
        // ======
        // Render
        // ======
        window.clear();
        
        window.draw(shipSprite);
        window.draw(matrix);
        
        if (shot)
            window.draw(shotSprite);
        
        window.display();
        
    }

	return EXIT_SUCCESS;
}

sf::Vector2f getSpriteCenter(sf::Sprite &sprite)
{
    sf::FloatRect rect = sprite.getGlobalBounds();
    return sf::Vector2f(rect.width / 2 + rect.left, rect.height / 2 + rect.top);
}

bool spritesColide(sf::Sprite &sa, sf::Sprite &sb)
{
    return sa.getGlobalBounds().intersects(sb.getGlobalBounds());
}

