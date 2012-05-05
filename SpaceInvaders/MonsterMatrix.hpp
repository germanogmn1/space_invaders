#include <SFML/Graphics.hpp>
#include "ResourcePath.hpp"
#include "Monster.hpp"

// TODO destructor
class MonsterMatrix : public sf::Drawable
{
    sf::Texture texture;
    
    const static int columns = 11, rows = 5;
    Monster * monsters[rows][columns];
    
    sf::FloatRect sandBox;
    sf::Vector2f origin;
    void positionMonsters();
    
    int stepCounter;
    bool stepToRight;
    
    bool leftCollision(sf::Vector2f);
    bool rightCollision(sf::Vector2f);
    int firstAliveCol, lastAliveCol;
    bool isColumnAlive(int);
    
public:
    MonsterMatrix(sf::FloatRect);
    ~MonsterMatrix();
    
    virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
    
    bool collides(sf::Sprite&);
    void step();
};