#ifndef MULTIPLESPRITE__H
#define MULTIPLESPRITE__H
#include <string>
#include <vector>
#include "drawable.h"

class MultipleSprite : public Drawable {
public:
  MultipleSprite(const std::string&);
  MultipleSprite(const MultipleSprite&);
  MultipleSprite(const std::string& name,const Vector2f& pos,const Vector2f& vel);
  virtual ~MultipleSprite() { } 

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
  }
  void changeDirection(int dir)
  {
	 direction = dir; 
  }
  int getDirection()
  {
	 return direction;
  }

protected:
  const std::vector<Frame *> frames;
  int worldWidth;
  int worldHeight;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight; 
  

  void advanceFrame(Uint32 ticks);
  int direction;
};



#endif