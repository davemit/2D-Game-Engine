#include <iostream>
#include "multiplesprite.h"
#include <string>


using namespace std;
class MultiBullet : public MultipleSprite {
public:
  MultiBullet(const string&, const Vector2f&, const Vector2f&);
  MultiBullet(const MultiBullet&);
  virtual void update(Uint32 ticks);
  bool goneTooFar() const {return tooFar;}
  void reset();
private:
  float distance;
  float maxDistance;
  bool tooFar;
  MultiBullet& operator = (const MultiBullet&);
  
};
  