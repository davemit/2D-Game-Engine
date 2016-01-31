#ifndef HUDCLASS__H
#define HUDCLASS__H
#include <string>
#include <SDL.h>
#include "ioManager.h"


class HudClass {
public:
  HudClass();
  void drawHud(SDL_Surface * const&, const IOManager&, int seconds, int fps, int bList, int fList,int health) const;

  void drawLose(SDL_Surface * const& screen, const IOManager& io) const;
  void drawWin(SDL_Surface * const& screen, const IOManager& io) const;
};
#endif
