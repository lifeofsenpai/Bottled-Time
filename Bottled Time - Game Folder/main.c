#include <gb/gb.h>
#include <rand.h>

//Bank 0
void Change_Graphics(void);

//Bank 2
void performant_delay();
void build_sprite();
void anim_sprite();
void move_npc();
void walk_npc();
void fade_out();
void fade_in();

void Anim_PlayerWalk(void);

void Update_Map(void);
void Update_Joypad(void);

void Set_Soran_Sprite(void);

void Gameplay();

//Bank 3
void set_bkg_map();
void set_tileset();

void Load_Font_1(void);
void Draw_Titlescreen(void);

void Load_Soran(void);
//

void Reset_Graphics(void)
{
    disable_interrupts();
    SWITCH_ROM_MBC5(3);

    SWITCH_ROM_MBC5(2);
    enable_interrupts();

    Gameplay();
}

void main() NONBANKED
{
    BGP_REG = 0xFF;
    SWITCH_ROM_MBC5(3);

    Load_Font_1();

    Draw_Titlescreen();

    Load_Soran();
    SWITCH_ROM_MBC5(2);

    Set_Soran_Sprite();
    move_npc(0, 10, 10, 1);

    waitpad(J_START);

    fade_in();

    Gameplay();
}