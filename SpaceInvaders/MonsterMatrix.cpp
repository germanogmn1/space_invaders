#include "MonsterMatrix.hpp"

#define MATRIX_STEP_SIZE 10
#define MATRIX_SHOT_CHANCE 5

#define MATRIX_MONSTER_W 40.f
#define MATRIX_MONSTER_H 26.f
#define MATRIX_MONSTER_MARGIN 20.f

//
// Area is the global rectangle in which the matrix may inhabit
//
MonsterMatrix::MonsterMatrix(sf::FloatRect area, Shot& monsterShot)
{
    sandBox = area;
    shot = &monsterShot;
    texture1.loadFromFile(resourcePath() + "monster1.png");
    texture2.loadFromFile(resourcePath() + "monster2.png");
    texture3.loadFromFile(resourcePath() + "monster3.png");
    
    sf::Texture * rowTextures[] = {
        &texture3, &texture2, &texture2, &texture1, &texture1
    };
    
    // Load monsters
    for (int row = 0; row < rows; row++)
        for (int col = 0; col < columns; col++)
            monsters[row][col] = new Monster(*rowTextures[row]);
    
    // Initial velocity
    stepDelay = 250;
    stepCounter = stepDelay;
    // Initial direction
    stepToRight = true;
    
    // Start at top-left of the sandbox
    origin.x = sandBox.left;
    origin.y = sandBox.top;
    
    // Everyone is alive
    lastAliveCol = columns - 1;
    firstAliveCol = 0;
    
    // Random
    std::srand((unsigned) time(0));
    
    level = 1;
    
    positionMonsters();
}

MonsterMatrix::~MonsterMatrix()
{
    for (int row = 0; row < rows; row++)
        for (int col = 0; col < columns; col++)
            delete monsters[row][col];
}

//
// Sets monsters positions in global coordinates
//
void MonsterMatrix::positionMonsters()
{
    float x = origin.x,
          y = origin.y;
    
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < columns; col++)
        {
            monsters[row][col]->sprite.setPosition(x, y);
            x += MATRIX_MONSTER_W + MATRIX_MONSTER_MARGIN;
        }
        y += MATRIX_MONSTER_H + MATRIX_MONSTER_MARGIN;
        x = origin.x;
    }
}

//
// Drawable implementation
//
void MonsterMatrix::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (int row = 0; row < rows; row++)
        for (int col = 0; col < columns; col++)
            monsters[row][col]->draw(target, states);
}

//
// Check if the shot collides with some monster in matrix
// then process it and returns the achieved score
//
int MonsterMatrix::strikeWith(Shot& shot)
{
    if (!shot.alive)
        return false;
    // start at last row for more chance of collision
    for (int row = rows - 1; row >= 0; row--)
    {
        for (int col = 0; col < columns; col++)
        {
            if (monsters[row][col]->alive && monsters[row][col]->collides(shot)) {
                bool anyAlive = false;
                
                for (int row = 0; row < rows; row++)
                    for (int col = 0; col < columns; col++)
                        if (monsters[row][col]->alive)
                            anyAlive = true;
                
                if (!anyAlive) {
                    level++;
                    stepDelay -= 25;
                    origin.x = sandBox.left;
                    origin.y = sandBox.top;
                    positionMonsters();
                    for (int row = 0; row < rows; row++)
                        for (int col = 0; col < columns; col++)
                            monsters[row][col]->alive = true;
                    firstAliveCol = 0;
                    lastAliveCol = columns - 1;
                }
                
                return (rows - row) * level; // higher row, higher score
            }
        }
    }
    return 0;
}

//
// Move the matrix a step inside sandbox
//
void MonsterMatrix::step()
{
    // Delay
    if (stepCounter--)
        return;
    stepCounter = stepDelay;
    
    randomShot();
    
    sf::Vector2f newPos(origin);
    
    newPos.x += MATRIX_STEP_SIZE * (stepToRight ? 1 : -1);
    
    if (stepToRight && rightCollision(newPos)) {
        stepToRight = false;
        origin.y += MATRIX_STEP_SIZE;
    }
    else if (!stepToRight && leftCollision(newPos)) {
        stepToRight = true;
        origin.y += MATRIX_STEP_SIZE;
    }
    else {
        origin = newPos;
    }
    
    positionMonsters();
}

//
// Check if there was a collision with the left edge of the sandbox
//
bool MonsterMatrix::leftCollision(sf::Vector2f newOrigin)
{
    while (!isColumnAlive(firstAliveCol))
        firstAliveCol++;
    
    float padding = firstAliveCol * (MATRIX_MONSTER_W + MATRIX_MONSTER_MARGIN);
    
    if (origin.x + padding <= sandBox.left)
        return true;
    
    return false;
}

//
// Check if there was a collision with the right edge of the sandbox
//
bool MonsterMatrix::rightCollision(sf::Vector2f newOrigin)
{
    while (!isColumnAlive(lastAliveCol))
        lastAliveCol--;
    
    float width = (MATRIX_MONSTER_W + MATRIX_MONSTER_MARGIN) * (lastAliveCol + 1);
    width -= MATRIX_MONSTER_MARGIN; // ignore last monster margin
    
    if (origin.x + width >= sandBox.left + sandBox.width)
        return true;
    
    return false;
}

bool MonsterMatrix::isColumnAlive(int col)
{
    for (int row = 0; row < rows; row++)
    {
        if (monsters[row][col]->alive) {
            return true;
        }
    }
    return false;
}

void MonsterMatrix::randomShot()
{
    if (shot->alive)
        return;
    
    // chance of shot
    bool willShot = (std::rand() % MATRIX_SHOT_CHANCE) == 0;
    
    Monster * shooter;
    int row, col;
    do {
        row = (std::rand() % rows);
        col = (std::rand() % columns);
    }
    while (!monsters[row][col]->alive);
    
    shooter = monsters[row][col];
    
    if (willShot) {
        float x = shooter->sprite.getPosition().x + shooter->sprite.getTexture()->getSize().x / 2;
        shot->spawnAt(x, shooter->sprite.getPosition().y);
    }
}

bool MonsterMatrix::gotOffSandBox()
{
    // last row monster
    sf::FloatRect monsterBox = monsters[rows - 1][0]->sprite.getGlobalBounds();
    if (monsterBox.top + monsterBox.height > sandBox.top + sandBox.height)
        return true;
    return false;
}
