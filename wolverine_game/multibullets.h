#include <list>
#include "multibullet.h"
#include "explodingSprite.h"

class MultiBullets {
public:
  MultiBullets(const std::string &);
  MultiBullets(const MultiBullets&);
  ~MultiBullets(){}
  void draw() const;
  void update(Uint32 ticks);
  void shoot(const Vector2f& pos, const Vector2f& objVel);
  
  unsigned int bulletCount() const {return bulletList.size();}
  unsigned int freeCount() const {return freeList.size();}
  bool shoot() const { return !bulletList.empty();}
  bool collidedWith(const Drawable* obj, int absDist) ;
  int getFreelistSize();
  int getBulletlistSize();
  void reset();
private:
  std::string name;
  Vector2f myVelocity;
  std::vector<Frame*> bulletFrames;
  std::list<MultiBullet> bulletList;
  std::list<MultiBullet> freeList;
  std::list<ExplodingSprite*> exSprites;
};
