#include "Monster.hpp"

Monster::Monster(sf::Texture& texture)
{
    alive = true;
    sprite.setTexture(texture);
}

//
// Processes collision with another sprite if it occurs
//
bool Monster::collides(Shot& shot)
{
    if (alive && sprite.getGlobalBounds().intersects(shot.sprite.getGlobalBounds())) {
        alive = false;
        shot.alive = false;
        return true;
    }
    return false;
}

//
// Drawable implementation
//
void Monster::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (alive)
        target.draw(sprite);
}