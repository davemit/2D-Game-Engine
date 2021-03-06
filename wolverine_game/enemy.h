#ifndef ENEMY__H
#define ENEMY__H
#include <string>
#include <vector>
#include "drawable.h"
#include "multibullets.h"

class Enemy : public Drawable {
public:
  Enemy(const std::string&);
  Enemy(const std::string&, int num);
  Enemy(const Enemy&);
  virtual ~Enemy() { } 

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
 void retreat();
 virtual bool collidedWith(const Drawable*, int) ;
 int getFreelistSize();
 int getBulletlistSize();
 void reset();
 void changeExplode(bool isCase){ 
    isExploding = isCase;
 }
 bool getExplode(){return isExploding;}  
protected:
  
  //Animation Frames
  enum STATES {IDLE, WALK, RETREAT, SHOOT};
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
  //Bullet
  std::string bulletName;
  float bulletInterval;
  float timeSinceLastBullet;
  float minBulletSpeed;
  MultiBullets bullets;
  bool isExploding;
//  bool isIdle;
  void advanceFrame(Uint32 ticks);
  
};



#endif
