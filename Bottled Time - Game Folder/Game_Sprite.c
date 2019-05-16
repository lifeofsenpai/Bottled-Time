#include <gb/gb.h>

typedef struct GameSprite
{
    UINT8 sprite_id;
    UBYTE tile_num[4];
    UBYTE palette_num[4];
    UBYTE flip;
} GameSprite;