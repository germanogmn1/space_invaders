#include <SFML/Graphics.hpp>

#ifndef SHOT_HPP
#define SHOT_HPP

class Shot : public sf::Drawable
{
public:
    enum Direction {
        UP, DOWN
    };
    
private:
    Direction direction;
    float speed;
    
public:
    Shot(sf::Texture&, Direction, float);
    void step();
    void spawnAt(float, float);
    
    sf::Sprite sprite;
    bool alive;
    
    virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
};

#endif
