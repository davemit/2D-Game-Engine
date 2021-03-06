#ifndef PLAYER__H
#define PLAYER__H
#include <string>
#include <vector>
#include "drawable.h"
//#include "multibullets.h"

class Player : public Drawable {
public:
  Player(const std::string&);
  Player(const Player&);
  virtual ~Player() { } 

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
  }


  int getWidth() const{return frameWidth;}
  int getHeight()const{return frameHeight;}
  void changeDirection(int dir)
  {
	 direction = dir; 
  }
  void changeState(int st)
  {
	 currState = st;

  }
  int getState()
  {
	 return currState; 
  }
  int getDirection()
  {
	 return direction;
  }
 void left();
 void right();
 void up();
 void down();
 void stop();
 void punch();
 void jumpAttack();
 void changeExplode(bool isCase){ isExploding = isCase;}
 bool getExplode(){return isExploding;}  
 void reset();
 virtual bool collidedWith(const Drawable* obj1,const Drawable* obj2);
protected:
  enum STATES {IDLE, WALK, SLICE,JUMP};
  //Animation Frames
  const std::vector<Frame *> frames;
  int worldWidth;
  int worldHeight;
  
  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight; 
  int direction;
  bool keyPressedX;
  bool keyPressedY;

  Vector2f initialVelocity;
  const float slowDown;
  int states;
  int currState;
  int originPosY;
  int upperBound;
  bool isExploding;
  void advanceFrame(Uint32 ticks);
 
};



#endif
