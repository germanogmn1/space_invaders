#include <SFML/Graphics.hpp>
#include "ResourcePath.hpp"
#include "BarrierTile.hpp"
#include "Shot.hpp"

#ifndef BARRIER_HPP
#define BARRIER_HPP

class Barrier : public sf::Drawable
{
    sf::Vector2f origin;
    BarrierTile * tiles[2][2];
    
public:
    Barrier(sf::Vector2f);
    bool strikeWith(Shot&);
    virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
};

#endif