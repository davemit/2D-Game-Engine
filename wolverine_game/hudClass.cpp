#include "hudClass.h"
#include <iostream>
#include <string>
#include <iomanip>
#include "gamedata.h"
#include "aaline.h"

HudClass::HudClass(){}
void HudClass::drawHud(SDL_Surface * const& screen, const IOManager& io,int seconds,int fps, int bList, int fList, int health) const{

const int HUD_W = 350;
const int HUD_H = 200;
int x = 30;
int y = 30;

const Uint32 BLUE = SDL_MapRGB(screen->format, 0xff, 0xff,0);
Draw_AALine(screen, x, y+HUD_H/2, x+HUD_W, y+HUD_H/2, HUD_H, 0, 0, 0, 0xff/2);
io.printMessageAt("Press:", x+5, y+5);
io.printMessageAt("W -> UP     S -> DOWN  V->SMASH", x+5, y+30);
io.printMessageAt("A -> LEFT  D -> RIGHT  SPACE->SPLICE", x+5, y+55);
io.printMessageAt("     G->GOD MODE", x+25, y+80);
io.printMessageAt("F1-> To read Help", x+10, y+105);
io.printMessageValueAt("Seconds: ", seconds, x+10,y+135);
io.printMessageValueAt("fps: ", fps, x+10, y+160);
io.printMessageValueAt("Bullet List: ", bList, x+120,y+135);
io.printMessageValueAt("FreeLis: ", fList, x+120, y+160);
io.printMessageValueAt("Health: ", health, x+250, y+160);
Draw_AALine(screen, x,y,x+HUD_W,y, BLUE);

}

void HudClass::drawLose(SDL_Surface * const& screen, const IOManager& io ) const{
const int HUD_W = 300;
const int HUD_H = 100;
int x = 300;
int y = 200;
const Uint32 BLUE = SDL_MapRGB(screen->format, 0xff, 0,0xff);
Draw_AALine(screen, x, y+HUD_H/2, x+HUD_W, y+HUD_H/2, HUD_H, 0, 0, 0, 0xff/2);
io.printMessageAt("Game Over", x+5, y+5);
io.printMessageAt("Sentinels Thrive!", x+5, y+30);
io.printMessageAt("Press ESC to go to Main Menu", x+5, y+55);
Draw_AALine(screen, x,y,x+HUD_W,y, BLUE);

}


void HudClass::drawWin(SDL_Surface * const& screen, const IOManager& io ) const{
const int HUD_W = 300;
const int HUD_H = 100;
int x = 300;
int y = 200;
const Uint32 BLUE = SDL_MapRGB(screen->format, 0xff, 0xff,0);
Draw_AALine(screen, x, y+HUD_H/2, x+HUD_W, y+HUD_H/2, HUD_H, 0, 0, 0, 0xff/2);
io.printMessageAt("You Win!", x+5, y+5);
io.printMessageAt("Great going Logan!", x+5, y+30);
io.printMessageAt("Press ESC to go to Main Menu", x+5, y+55);
Draw_AALine(screen, x,y,x+HUD_W,y, BLUE);

}



