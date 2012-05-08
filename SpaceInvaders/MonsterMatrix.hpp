#include <cstdlib>
#include <SFML/Graphics.hpp>
#include "ResourcePath.hpp"
#include "Monster.hpp"
#include "Shot.hpp"

#ifndef MONSTER_MATRIX_HPP
#define MONSTER_MATRIX_HPP

class MonsterMatrix : public sf::Drawable
{
    sf::Texture texture;
    
    const static int columns = 11, rows = 5;
    Monster * monsters[rows][columns];
    Shot * shot;
    
    sf::FloatRect sandBox;
    sf::Vector2f origin;
    void positionMonsters();
    
    int stepCounter;
    bool stepToRight;
    
    bool leftCollision(sf::Vector2f);
    bool rightCollision(sf::Vector2f);
    int firstAliveCol, lastAliveCol;
    bool isColumnAlive(int);
    
    void randomShot();
    
public:
    MonsterMatrix(sf::FloatRect, Shot&);
    ~MonsterMatrix();
    
    virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
    
    int strikeWith(Shot&);
    void step();
};

#endif