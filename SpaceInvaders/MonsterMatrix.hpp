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
    sf::FloatRect bounds;
    void calcBounds();
    void positionMonsters();
    struct {
        int begin, end;
    } aliveRange;
    
    int stepCounter;
    bool stepToRight;
    
public:
    MonsterMatrix(sf::FloatRect);
    
    virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
    
    bool collides(sf::Sprite&);
    void step();
};