#include "multibullets.h"
#include "gamedata.h"
#include "frameFactory.h"

MultiBullets::MultiBullets(const std::string & str) :
  name(str),
  myVelocity(Gamedata::getInstance().getXmlInt(name+"/speedX"),
             Gamedata::getInstance().getXmlInt(name+"/speedY")
  ),
  bulletFrames(FrameFactory::getInstance().getFrames(name)),
  bulletList(),
  freeList(),
  exSprites()
{}
MultiBullets::MultiBullets(const MultiBullets& b) :
  name(b.name),
  myVelocity(b.myVelocity),
  bulletFrames(b.bulletFrames),
  bulletList(b.bulletList),
  freeList(b.freeList),
  exSprites(b. exSprites)
{}

void MultiBullets::shoot(const Vector2f& pos, const Vector2f& objVel){
  if(freeList.empty()) {
    MultiBullet b(name,pos, objVel);
    bulletList.push_back(b);
  }
  else{
    MultiBullet b = freeList.front();
    freeList.pop_front();
    b.reset();
    b.setVelocity(objVel);
    b.setPosition(pos);
    bulletList.push_back(b);
  }
  
}

void MultiBullets::draw() const {
  std::list<MultiBullet>::const_iterator bitr = bulletList.begin();
  while(bitr!=bulletList.end()){
     bitr->draw();
      ++bitr;
  }
  std::list<ExplodingSprite*>::const_iterator exitr = exSprites.begin();
  while(exitr!=exSprites.end()){
     (*exitr)->draw();
      ++exitr;
  }
}
void MultiBullets::reset(){
  std::list<MultiBullet>::iterator bitr = bulletList.begin();
  while(bitr!=bulletList.end()){
     freeList.push_back(*bitr);

     bitr = bulletList.erase(bitr);
 }

  std::list<ExplodingSprite*>::iterator exitr = exSprites.begin();
  while(exitr!=exSprites.end()){      
        exitr = exSprites.erase(exitr);
  }     
  
}
int MultiBullets::getFreelistSize(){
   return freeList.size();
}

int MultiBullets::getBulletlistSize(){
   return bulletList.size();
}

bool MultiBullets::collidedWith(const Drawable * obj, int absDist)  {
     std::list<MultiBullet>::iterator bitr = bulletList.begin();
    //
     while(bitr!=bulletList.end()){
	if(bitr->execute(*bitr, *obj)&& absDist <15){
          Vector2f vel = bitr->getVelocity();
          Vector2f pos = bitr->getPosition();
	  Sprite *temp = new Sprite("fireball",pos, vel);
       	  exSprites.push_back(new ExplodingSprite(*temp));
          delete temp;
          //putting bullet in free list 
          freeList.push_back(*bitr);
          bitr = bulletList.erase(bitr);
	   return true;
	}
      ++bitr;
     }
     return false;
}
 
void MultiBullets::update(Uint32 ticks)  {
  std::list<MultiBullet>::iterator bitr = bulletList.begin();
  while(bitr!=bulletList.end()){
     bitr->update(ticks);
     if(bitr->goneTooFar()){
        
	freeList.push_back(*bitr);
        bitr = bulletList.erase(bitr);
     }
      else ++bitr;
  }
  //Exploding sprites
  std::list<ExplodingSprite*>::iterator exitr = exSprites.begin();
  while(exitr!=exSprites.end()){
      (*exitr)->update(ticks);
      if((*exitr)->done()){
     
        exitr = exSprites.erase(exitr);
      }
      else ++exitr;
  }
} 
 
 
