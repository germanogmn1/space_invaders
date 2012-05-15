#include "BarrierTile.hpp"

BarrierTile::BarrierTile(const char * normalImage, const char * damagedImage)
{
    normal.loadFromFile(resourcePath() + normalImage);
    damaged.loadFromFile(resourcePath() + damagedImage);
    sprite.setTexture(normal);
    health = 4;
}

bool BarrierTile::strikeWith(Shot &shot)
{
    if (health && shot.alive && sprite.getGlobalBounds().intersects(shot.sprite.getGlobalBounds())) {
        takeDamage();
        shot.alive = false;
        return true;
    }
    return false;
}

void BarrierTile::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(sprite);
}

void BarrierTile::takeDamage()
{
    health--;
    if (!health)
        sprite.setColor(sf::Color::Transparent);
    else if (health <= 2)
        sprite.setTexture(damaged);
}
