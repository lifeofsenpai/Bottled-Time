#include <gb/gb.h>
#include <rand.h>
#include "Game_Character.c"
#include "Game_Sprite.c"
#include "Game_Map.c"
#include "Map_PlumVillage_Info.c"


UBYTE Playing, Joy;

UINT8 PlayerDirection = 1;

UINT8 PlayerAnimCycle;

UINT8 PlayerAnimTimer;

UINT16 grid_x, grid_y;

//Flags:
UBYTE PlayerControlFlag;

//Game Characters:
GameCharacter char_player;

//Game Maps:
GameMap map_plum_village;

//Game Sprites:
GameSprite sprite_soran_up_0;
GameSprite sprite_soran_down_0;

UBYTE Collision_Check_TL(GameCharacter* character, INT16 move_x, INT16 move_y)
{
    grid_x = (((character->pos_x + move_x) - 8) / 8);
    grid_y = (((character->pos_y + move_y) - 16) / 8);
    return PlumVillageColl[(20 * grid_y) + grid_x];
}

UBYTE Collision_Check_TR(GameCharacter* character, INT16 move_x, INT16 move_y)
{
    grid_x = ((((character->pos_x + 15) + move_x) - 8) / 8);
    grid_y = (((character->pos_y + move_y) - 16) / 8);
    return PlumVillageColl[(20 * grid_y) + grid_x];
}

UBYTE Collision_Check_BL(GameCharacter* character, INT16 move_x, INT16 move_y)
{
    grid_x = (((character->pos_x + move_x) - 8) / 8);
    grid_y = ((((character->pos_y + 15) + move_y) - 16) / 8);
    return PlumVillageColl[(20 * grid_y) + grid_x];
}

UBYTE Collision_Check_BR(GameCharacter* character, INT16 move_x, INT16 move_y)
{
    grid_x = ((((character->pos_x + 15) + move_x) - 8) / 8);
    grid_y = ((((character->pos_y + 15) + move_y) - 16) / 8);
    return PlumVillageColl[(20 * grid_y) + grid_x];
}

void Reset_Graphics();

void performant_delay(UINT8 num_loops)
{
    UINT8 i;
    for(i = 0; i < num_loops; i++)
    {
        wait_vbl_done();
    }
}

void build_sprite(GameSprite* sprite)
{
    disable_interrupts();

    if(sprite->flip == 0)
    {
        set_sprite_tile(sprite->sprite_id, sprite->tile_num[0]);
        set_sprite_prop(sprite->sprite_id, sprite->palette_num[0]);
        set_sprite_tile(sprite->sprite_id + 1, sprite->tile_num[1]);
        set_sprite_prop(sprite->sprite_id + 1, sprite->palette_num[1]);
        set_sprite_tile(sprite->sprite_id + 2, sprite->tile_num[2]);
        set_sprite_prop(sprite->sprite_id + 2, sprite->palette_num[2]);
        set_sprite_tile(sprite->sprite_id + 3, sprite->tile_num[3]);
        set_sprite_prop(sprite->sprite_id + 3, sprite->palette_num[3]);
    }
    else if(sprite->flip == 1)
    {
        set_sprite_tile(sprite->sprite_id, sprite->tile_num[0]);
        set_sprite_prop(sprite->sprite_id, sprite->palette_num[0]);
        set_sprite_tile(sprite->sprite_id + 1, sprite->tile_num[1]);
        set_sprite_prop(sprite->sprite_id + 1, sprite->palette_num[1]);
        set_sprite_tile(sprite->sprite_id + 2, sprite->tile_num[0]);
        set_sprite_prop(sprite->sprite_id + 2, sprite->palette_num[0] | S_FLIPX);
        set_sprite_tile(sprite->sprite_id + 3, sprite->tile_num[1]);
        set_sprite_prop(sprite->sprite_id + 3, sprite->palette_num[1] | S_FLIPX);
    }

    enable_interrupts(); 
}

void anim_sprite()
{
}

void move_npc(UINT8 first_sprite, UINT8 tile_x, UINT8 tile_y, UINT8 pixel_offset)
{
    move_sprite(first_sprite, tile_x * 8, tile_y * 8);
    move_sprite(first_sprite + 1, tile_x * 8, tile_y * 8 + 8);
    move_sprite(first_sprite + 2, tile_x * 8 + 8 - pixel_offset, tile_y * 8);
    move_sprite(first_sprite + 3, tile_x * 8 + 8 - pixel_offset, tile_y * 8 + 8);
}

void walk_npc(UINT8 first_sprite, INT8 move_x, INT8 move_y)
{
    while(move_x != 0)
    {
        scroll_sprite(first_sprite, move_x < 0 ? -1 : 1, 0);
        scroll_sprite(first_sprite + 1, move_x < 0 ? -1 : 1, 0);
        scroll_sprite(first_sprite + 2, move_x < 0 ? -1 : 1, 0);
        scroll_sprite(first_sprite + 3, move_x < 0 ? -1 : 1, 0);
        move_x += (move_x < 0 ? 1 : -1);
        wait_vbl_done();
    }
    while(move_y != 0)
    {
        scroll_sprite(first_sprite, 0, move_y < 0 ? -1 : 1);
        scroll_sprite(first_sprite + 1, 0, move_y < 0 ? -1 : 1);
        scroll_sprite(first_sprite + 2, 0, move_y < 0 ? -1 : 1);
        scroll_sprite(first_sprite + 3, 0, move_y < 0 ? -1 : 1);
        move_y += move_y < 0 ? 1 : -1;
        wait_vbl_done();
    }
}

void fade_out()
{
    UINT8 i;
    for(i=0;i<4;i++)
    {
        switch(i)
        {
            case 0:
                BGP_REG = 0xE4;
                break;
            case 1:
                BGP_REG = 0xF9;
                break;
            case 2:
                BGP_REG = 0xFE;
                break;
            case 3:
                BGP_REG = 0xFF;
                SHOW_SPRITES;
                break;
        }
        performant_delay(10);
    }
}

void fade_in()
{
    UINT8 i;
    for(i=0;i<4;i++)
    {
        switch(i)
        {
            case 0:
                BGP_REG = 0xFF;
                break;
            case 1:
                BGP_REG = 0xFE;
                break;
            case 2:
                BGP_REG = 0xF9;
                break;
            case 3:
                SHOW_SPRITES;
                BGP_REG = 0xE4;
                break;
        }
        performant_delay(10);
    }
}

void Anim_PlayerWalk()
{
    if(PlayerAnimCycle == 1)
    {
        anim_sprite(0, 0, 0, 8, 0, 1, 0, 0, 1);
        PlayerAnimCycle == 0;
    }
    else if(PlayerAnimCycle == 2)
    {
        anim_sprite(0, 6, 4, 8, 0, 1, 0, 0, 1);
        PlayerAnimCycle == 0;
    }
    else if(PlayerAnimCycle == 3)
    {
        anim_sprite(0, 12, 8, 8, 0, 1, 0, 1, 0);
        PlayerAnimCycle == 0;
    }
    else if(PlayerAnimCycle == 4)
    {
        build_sprite(0, 8, 0, 1, 0, 1, 2);
    }
}

//Updates current map info.
void Update_Map()
{

}

//Updates joypad and detects for input.
void Update_Joypad(void)
{
    Joy = joypad();

    if(Joy & J_UP && PlayerControlFlag == 1)
    {
        PlayerDirection = 1;
        PlayerAnimCycle = 1; 

        if(Collision_Check_TL(&char_player, 0, 0) != 1 && Collision_Check_TR(&char_player, 0, 0) != 1)
        {
            char_player.pos_y -= 8;
            walk_npc(0, 0, -8);
        }
    }

    if(Joy & J_DOWN && PlayerControlFlag == 1)
    {
        PlayerDirection = 2;
        PlayerAnimCycle = 2;

        if(Collision_Check_BL(&char_player, 0, 8) != 1 && Collision_Check_BR(&char_player, 0, 8) != 1)
        {
            char_player.pos_y += 8;
            walk_npc(0, 0, 8);
        }
    }

    if(Joy & J_LEFT && PlayerControlFlag == 1)
    {
        PlayerDirection = 3;
        PlayerAnimCycle = 3;

        if(Collision_Check_BL(&char_player, -8, 0) != 1)
        {
            char_player.pos_x -= 8;
            walk_npc(0, -8, 0);
        }
    }

    if(Joy & J_RIGHT && PlayerControlFlag == 1)
    {
        PlayerDirection = 3;
        PlayerAnimCycle = 3;

        if(Collision_Check_BR(&char_player, 8, 0) != 1)
        {
            char_player.pos_x += 8;
            walk_npc(0, 8, 0);
        }
    }

    if(Joy & J_A)
    {
        build_sprite(&sprite_soran_up_0);
    }
}

void Set_Soran_Sprite(void)
{
    sprite_soran_up_0.sprite_id = 0;
    sprite_soran_up_0.tile_num[0] = 0;
    sprite_soran_up_0.tile_num[1] = 1;
    sprite_soran_up_0.palette_num[0] = 0;
    sprite_soran_up_0.palette_num[1] = 1;
    sprite_soran_up_0.flip = 1;

    sprite_soran_down_0.sprite_id = 0;
    sprite_soran_down_0.tile_num[0] = 4;
    sprite_soran_down_0.tile_num[1] = 5;
    sprite_soran_down_0.palette_num[0] = 0;
    sprite_soran_down_0.palette_num[1] = 1;
    sprite_soran_down_0.flip = 1;

    build_sprite(&sprite_soran_down_0);
}

void Gameplay(void)
{
    PlayerControlFlag = 1;
    char_player.pos_x = 80;
    char_player.pos_y = 80;

    while(1)
    {
        Update_Joypad();
    }
}