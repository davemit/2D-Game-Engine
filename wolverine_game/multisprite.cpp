#include "multisprite.h"
#include "gamedata.h"
#include "frameFactory.h"
#include <cmath>
#include <math.h>


float getPoints(float start, float end, float totalPoints, float pointNumber)
{
	
    return (start + ((end - start)/totalPoints)*pointNumber);
}


void MultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = ((currentFrame+1) % numberOfFrames)+(getDirection()*numberOfFrames);
		timeSinceLastFrame = 0;
	}
}

MultiSprite::MultiSprite( const std::string& name,int repNum) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x")+ 
		     getPoints(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),Gamedata::getInstance().getXmlInt(name+"/endLoc/x"),Gamedata::getInstance().getXmlInt(name+"/totalInstance"),repNum), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  scale(Gamedata::getInstance().getRandFloat(Gamedata::getInstance().getXmlFloat(name+"/scale/min"),Gamedata::getInstance().getXmlFloat(name+"/scale/max"))),
  frames( FrameFactory::getInstance().getFrames(name )),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  direction(1)
{
  
  velocityX(  velocityX() *scale );
}





MultiSprite::MultiSprite(const MultiSprite& s) :
  Drawable(s), 
  scale(s.scale),
  frames(s.frames),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight ),
  direction(s.direction)
  { }
  
  
  
MultiSprite::MultiSprite( const std::string& name,const Vector2f& pos,const Vector2f& vel) :
  Drawable(name, pos, vel),
  scale(Gamedata::getInstance().getRandFloat(Gamedata::getInstance().getXmlFloat(name+"/scale/min"),Gamedata::getInstance().getXmlFloat(name+"/scale/max"))),
  frames( FrameFactory::getInstance().getFrames(name )),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  direction(1)
{
  
  
}


void MultiSprite::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->drawScaled(x, y,scale);
}



void MultiSprite::update(Uint32 ticks) { 
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );
  }

  if ( X() < -frameWidth) {
    setPosition(Vector2f(Gamedata::getInstance().getXmlInt("world/width"), 
                    Y()));
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
       
  }  

}
