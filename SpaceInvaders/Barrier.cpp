#include "Barrier.hpp"

#define BARRIER_TILE_W 40
#define BARRIER_TILE_H 25

Barrier::Barrier(sf::Vector2f origin)
{
    this->origin = origin;
    
    tiles[0][0] = new BarrierTile("barrier_tl.png", "barrier_tl_dmg.png");
    tiles[0][1] = new BarrierTile("barrier_tr.png", "barrier_tr_dmg.png");
    tiles[1][0] = new BarrierTile("barrier_bl.png", "barrier_bl_dmg.png");
    tiles[1][1] = new BarrierTile("barrier_br.png", "barrier_br_dmg.png");
    
    // Textures & Sprites
    sf::Vector2f pos(origin);
    for (int r = 0; r < 2; r++) {
        for (int c = 0; c < 2; c++) {
            tiles[r][c]->sprite.setPosition(pos);
            pos.x += BARRIER_TILE_W;
        }
        pos.x = origin.x;
        pos.y += BARRIER_TILE_H;
    }
}

bool Barrier::strikeWith(Shot& shot)
{
    for (int r = 0; r < 2; r++)
        for (int c = 0; c < 2; c++)
            if (tiles[r][c]->strikeWith(shot))
                return true;
    
    return false;
}

void Barrier::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (int r = 0; r < 2; r++)
        for (int c = 0; c < 2; c++)
            tiles[r][c]->draw(target, states);
}

