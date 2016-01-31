#include <vector>
#include <list>
#include <SDL.h>
#include "ioManager.h"
#include "clock.h"
#include "world.h"
#include "hudClass.h"
#include "viewport.h"
#include "sound.h"
#include "explodingSprite.h"
#include "player.h"
class Manager {
public:

  void setHud(bool show) {
    hud_flag = show;
  
  }
  void makeSprites(const std::string&, int totalInstance);
  Manager (int n);
  ~Manager ();
  void play();
  void pause() { clock.pause(); }
  void unpause() { clock.unpause(); }
  void setNumberOfSprites(int);
  //For player enemey interaction
  int getAbs(int x1, int x2);
  void manageEnemy();


private:
  const bool env;
  const IOManager& io;
  Clock& clock;

  SDL_Surface * const screen;
  World world;
  World world_river;
  World world_city;
  bool hud_flag;
  bool lose_flag;
  bool win_flag;
  int  totalEnemy;
  int  enemyKilled;
  HudClass bar;
  Viewport& viewport;

  std::list<Drawable*> sprites;
  //std::list<Drawable*> sortSprites;
  int currentSprite;
  std::list<Drawable*> bSprites;
  std::list<ExplodingSprite*> exSprites;
  Player* playerPt;
  bool makeVideo;
  int frameCount;
  const std::string username;
  const std::string title;
  const int frameMax;
  SDLSound sound;
  int bList;
  int fList;
  int playerHealth;
  bool godMode;
  void draw() const;
  void update();
  

  Manager(const Manager&);
  Manager& operator=(const Manager&);
  void makeFrame();
};
