#include "MonsterMatrix.hpp"

#define MATRIX_STEP_DELAY 250
#define MATRIX_STEP_SIZE 5
#define MATRIX_MONSTER_W 40.f
#define MATRIX_MONSTER_H 26.f
#define MATRIX_MONSTER_MARGIN 20.f

//
// Area is the global rectangle in which the matrix may inhabit
//
MonsterMatrix::MonsterMatrix(sf::FloatRect area)
{
    sandBox = area;
    
    texture.loadFromFile(resourcePath() + "monster1.png");
    
    // Start at top-left of the sandbox
    bounds.left = sandBox.left;
    bounds.top = sandBox.top;
    bounds.width = (MATRIX_MONSTER_W * columns) + (MATRIX_MONSTER_MARGIN * (columns - 1));
    bounds.height = (MATRIX_MONSTER_H * rows) + (MATRIX_MONSTER_MARGIN * (rows - 1));
    
    // Setup steps
    stepCounter = MATRIX_STEP_DELAY;
    stepToRight = true;
    
    for (int row = 0; row < rows; row++)
        for (int col = 0; col < columns; col++)
            monsters[row][col] = new Monster(texture);

    // Everyone is alive
    aliveRange.begin = 0;
    aliveRange.end = columns - 1;
    
    positionMonsters();
}

//
// Sets monsters positions in global coordinates
//
void MonsterMatrix::positionMonsters()
{
    float x = bounds.left,
          y = bounds.top;
    
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < columns; col++)
        {
            monsters[row][col]->sprite.setPosition(x, y);
            x += MATRIX_MONSTER_W + MATRIX_MONSTER_MARGIN;
        }
        y += MATRIX_MONSTER_H + MATRIX_MONSTER_MARGIN;
        x = bounds.left;
    }
}

//
// Drawable implementation
//
void MonsterMatrix::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (int row = 0; row < rows; row++)
        for (int col = 0; col < columns; col++)
            target.draw(monsters[row][col]->sprite);
}

//
// Check if some monster in matrix collides with the shot and process it
//
bool MonsterMatrix::collides(sf::Sprite& shot)
{
    // start at last row for more chance of collision
    for (int row = rows - 1; row >= 0; row--)
    {
        for (int col = 0; col < columns; col++)
        {
            if (monsters[row][col]->collides(shot)) {
                calcBounds();
                return true;
            }
        }
    }
    return false;
}

//
// Move the matrix a step
//
void MonsterMatrix::step()
{
    // Delay
    if (stepCounter--)
        return;
    stepCounter = MATRIX_STEP_DELAY;
    
    sf::FloatRect newPos(bounds);
    
    newPos.left += MATRIX_STEP_SIZE * (stepToRight ? 1 : -1);
    
    if (stepToRight && (newPos.left + newPos.width) > (sandBox.left + sandBox.width)) {
        // right collision
        stepToRight = false;
        bounds.top += MATRIX_STEP_SIZE;
    }
    else if (newPos.left < sandBox.left) {
        // left collision
        stepToRight = true;
        bounds.top += MATRIX_STEP_SIZE;
    }
    else {
        // no collision
        bounds = newPos;
    }
    
    positionMonsters();
}

void MonsterMatrix::calcBounds()
{
    // TODO
    int col = aliveRange.begin;
    
    for (int row = 0; row < rows; row++)
    {
        if (monsters[row][col]->alive) {
            
        }
    }
}


