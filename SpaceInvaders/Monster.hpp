#include <SFML/Graphics.hpp>

class Monster {
    static sf::Texture texture();
    bool alive;
    void die();
    
public:
    Monster(sf::Texture&);
    bool collides(sf::Sprite&);
    sf::Sprite sprite;
    bool isAlive();
};