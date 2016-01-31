#ifndef MULTISPRITE__H
#define MULTISPRITE__H
#include <string>
#include <vector>
#include "drawable.h"
#include <cmath>

class MultiSprite : public Drawable {
public:
  MultiSprite(const std::string&,int repNum);
  MultiSprite(const MultiSprite&);
  MultiSprite(const std::string& name,const Vector2f& pos,const Vector2f& vel);
  virtual ~MultiSprite() { } 

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
  float getScale() const{return scale;}
  
protected:
  float scale;
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
