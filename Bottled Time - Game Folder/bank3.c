#include <gb/gb.h>
#include <stdio.h>
#include "Font_1.c"
#include "Tileset_1.c"
#include "Map_PlumVillage.c"
#include "Soran_Sprites.c"

extern UBYTE PlayerControlFlag;

const UWORD Palette_BKG[] =
{
    RGB(13,23,8), RGB(18,26,8), RGB(4,14,6), RGB(0,7,5),
    RGB(13,23,8), RGB(23,23,23), RGB(18,18,18), RGB(9,9,12),
    RGB(13,23,8), RGB(25,20,12), RGB(20,13,6), RGB(12,7,0),
    RGB(30,30,30), RGB(27,27,31), RGB(18,18,18), RGB(2,2,10),
    RGB(10,29,18), RGB(9,20,16), RGB(6,10,12), RGB(2,2,10),
    RGB(30,30,30), RGB(25,20,12), RGB(20,13,6), RGB(12,7,0),
};

const UWORD Palette_Sprite_1[] =
{
    0, RGB(31,27,8), RGB(23,10,18), RGB(2,2,10),
    0, RGB(31,27,21), RGB(23,10,18), RGB(2,2,10), 
};

void set_bkg_map(UINT8 tile_x, UINT8 tile_y, UINT8 width, UINT8 height, unsigned char *map_1, unsigned char *map_0)
{
    VBK_REG = 1;
    set_bkg_tiles(tile_x, tile_y, width, height, map_1);
    VBK_REG = 0;

    set_bkg_tiles(tile_x, tile_y, width, height, map_0);

}

void set_bkg_tileset(UINT8 first_tile, UINT8 num_tile, unsigned char *tileset)
{
    set_bkg_data(first_tile, num_tile, tileset);
}

void Load_Font_1(void)
{
    set_bkg_tileset(118, 10, Font_1);
}

void Draw_Titlescreen(void)
{
    wait_vbl_done();
    DISPLAY_OFF;

    set_bkg_palette(0, 6, Palette_BKG);
    set_bkg_tileset(0, 48, Tileset_1);
    set_bkg_map(0, 0, 20, 20, Map_PlumVillagePLN1, Map_PlumVillagePLN0);

    SHOW_BKG;
    HIDE_WIN;

    wait_vbl_done();
    DISPLAY_ON;
}

void Load_Soran(void)
{
    set_sprite_palette(0, 2, Palette_Sprite_1);
    set_sprite_data(0, 16, Soran_Sprites);
}
