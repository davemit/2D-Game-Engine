#include <SDL.h>

class ExtractSurface {
public:
  static ExtractSurface& getInstance();
  SDL_Surface* get(SDL_Surface*, int, int, int, int) const;
  SDL_Surface* getFlip(SDL_Surface*, int, int, int, int) const;
private:
  Uint32 getpixel(SDL_Surface*, int, int) const;
  void putpixel(SDL_Surface *, int, int, Uint32) const;
};
