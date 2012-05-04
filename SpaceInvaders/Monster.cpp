#include "Monster.hpp"

Monster::Monster(sf::Texture& texture)
{
    alive = true;
    sprite.setTexture(texture);
}

void Monster::die()
{
    alive = false;
    sprite.setColor(sf::Color::Transparent);
}

//
// Processes collision with another sprite if it occurs
//
bool Monster::collides(sf::Sprite& other)
{
    if (alive && sprite.getGlobalBounds().intersects(other.getGlobalBounds())) {
        die();
        return true;
    }
    return false;
}