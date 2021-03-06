#include <cstdlib>
#include <SFML/Graphics.hpp>
#include "ResourcePath.hpp"
#include "Monster.hpp"
#include "Shot.hpp"

#ifndef MONSTER_MATRIX_HPP
#define MONSTER_MATRIX_HPP

class MonsterMatrix : public sf::Drawable
{
    const static int columns = 10, rows = 5;
    
    sf::Texture texture1, texture2, texture3;
    
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
    
    float stepDelay;
    void randomShot();
    
public:
    int level;
    MonsterMatrix(sf::FloatRect, Shot&);
    ~MonsterMatrix();
    bool gotOffSandBox();
    
    virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
    
    int strikeWith(Shot&);
    void step();
};

#endif