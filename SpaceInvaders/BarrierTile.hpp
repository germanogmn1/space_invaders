#include <SFML/Graphics.hpp>
#include "ResourcePath.hpp"
#include "Shot.hpp"

#ifndef BARRIER_TILE_HPP
#define BARRIER_TILE_HPP

class BarrierTile : public sf::Drawable
{
    sf::Texture normal, damaged;
    void takeDamage();
    int health;
    
public:
    BarrierTile(const char *, const char *);
    bool strikeWith(Shot& shot);
    sf::Sprite sprite;
    virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
};

#endif
