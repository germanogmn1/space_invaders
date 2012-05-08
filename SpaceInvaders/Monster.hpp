#include <SFML/Graphics.hpp>
#include "Shot.hpp"

#ifndef MONSTER_HPP
#define MONSTER_HPP

class Monster : public sf::Drawable
{
    static sf::Texture texture();
    
public:
    Monster(sf::Texture&);
    bool collides(Shot&);
    
    sf::Sprite sprite;
    bool alive;
    
    virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
};

#endif