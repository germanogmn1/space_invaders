#include "Shot.hpp"

Shot::Shot(sf::Texture& texture, Direction direction, float speed)
{
    sprite.setTexture(texture);
    
    this->speed = speed;
    this->direction = direction;
    alive = false;
}

void Shot::spawnAt(float x, float y)
{
    sprite.setPosition(x, y);
    alive = true;
}

void Shot::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
    if (alive)
        target.draw(sprite);
}

void Shot::step()
{
    if (!alive)
        return;
    
    sf::Vector2f pos(sprite.getPosition());
    
    // TODO: remove magic numbers
    if (pos.y < 0 || pos.y > 600) {
        alive = false;
        return;
    }
    
    
    switch (direction)
    {
        case UP:
            pos.y -= speed;
            break;
        case DOWN:
            pos.y += speed;
            break;
    }
    
    sprite.setPosition(pos);
}