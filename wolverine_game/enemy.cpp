#include "enemy.h"
#include "gamedata.h"
#include "frameFactory.h"






void Enemy::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
	if((currentFrame % numberOfFrames) == 5 && currState >= SHOOT ) changeState(RETREAT);
    currentFrame = ((currentFrame+1) % numberOfFrames)+(getDirection()*(numberOfFrames*states))    +((getState())*numberOfFrames);
         
		timeSinceLastFrame = 0;
        
	}
}

Enemy::Enemy( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  frames( FrameFactory::getInstance().getEnemyFrames(name) ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  direction(0),
  keyPressedX(false),
  keyPressedY(false),
  initialVelocity(Gamedata::getInstance().getXmlInt(name+"/speedX"), 
                    Gamedata::getInstance().getXmlInt(name+"/speedY")),
  slowDown(Gamedata::getInstance().getXmlFloat(name+"/slowDown")),
  states( Gamedata::getInstance().getXmlInt(name+"/states") ),
  currState(0),
  originPosY(0),
  upperBound(Gamedata::getInstance().getXmlInt(name+"/bound/upper")),
  bulletName("fireball"),
  bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/bulletInterval")),
  timeSinceLastBullet(Gamedata::getInstance().getXmlInt(bulletName+"/timeSinceLastBullet")),
  minBulletSpeed(Gamedata::getInstance().getXmlInt(bulletName+"/minBulletSpeed")),
  bullets(bulletName),
  isExploding(false)
{  }


Enemy::Enemy( const std::string& name, int num) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x")*(num+1), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  frames( FrameFactory::getInstance().getEnemyFrames(name) ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  direction(0),
  keyPressedX(false),
  keyPressedY(false),
  initialVelocity(Gamedata::getInstance().getXmlInt(name+"/speedX"), 
                    Gamedata::getInstance().getXmlInt(name+"/speedY")),
  slowDown(Gamedata::getInstance().getXmlFloat(name+"/slowDown")),
  states( Gamedata::getInstance().getXmlInt(name+"/states") ),
  currState(0),
  originPosY(0),
  upperBound(Gamedata::getInstance().getXmlInt(name+"/bound/upper")),
  bulletName("fireball"),
  bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/bulletInterval")),
  timeSinceLastBullet(Gamedata::getInstance().getXmlInt(bulletName+"/timeSinceLastBullet")),
  minBulletSpeed(Gamedata::getInstance().getXmlInt(bulletName+"/minBulletSpeed")),
  bullets(bulletName),
  isExploding(false)
{  }



Enemy::Enemy(const Enemy& s) :
  Drawable(s), 
  frames(s.frames),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight ),
  direction(s.direction),
  keyPressedX(s.keyPressedX),
  keyPressedY(s.keyPressedY),
  initialVelocity(s.initialVelocity),
  slowDown(s.slowDown),
  states(s.states),
  currState(s.currState),
  originPosY(s.originPosY),
  upperBound(s.upperBound),
  bulletName(s.bulletName),
  bulletInterval(s.bulletInterval),
  timeSinceLastBullet(s.timeSinceLastBullet),
  minBulletSpeed(s.minBulletSpeed),
  bullets(s.bullets),
  isExploding(s.isExploding) 
  { }



void Enemy::reset(){

   X(Gamedata::getInstance().getXmlInt(getName()+"/startLoc/x"));
   Y(Gamedata::getInstance().getXmlInt(getName()+"/startLoc/y")); 
   bullets.reset();


} 
bool Enemy::collidedWith(const Drawable* obj, int absDist)  {
     return bullets.collidedWith(obj,absDist);
}

int Enemy::getFreelistSize(){
    return bullets.getFreelistSize();
}
  
int Enemy::getBulletlistSize(){
    return bullets.getBulletlistSize();
}
void Enemy::draw() const { 
  if(!isExploding)
  {
   Uint32 x = static_cast<Uint32>(X());
   Uint32 y = static_cast<Uint32>(Y());
   frames[currentFrame]->draw(x, y);
   bullets.draw();
  }
  
}

void Enemy::update(Uint32 ticks) { 
  advanceFrame(ticks);
  if(isExploding)
  { 
    stop();
  }
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  
	  if ( Y() < upperBound) {
	    velocityY( abs( velocityY() ) );
	    changeState(0);

	  }
	  if ( Y() > worldHeight-frameHeight) {
	    Y(worldHeight-frameHeight);
	    velocityY( -abs( velocityY() ) );
		changeState(0);
	  }

	  if ( X() <= 0) {
	    velocityX( abs( velocityX() ) );
	    changeDirection(0);
	  }
	  if ( X() >= worldWidth-frameWidth) {
	    velocityX( -abs( velocityX() ) );
		changeDirection(1);
	  } 
  
 
  if( velocityX() > 0) changeDirection(0);
  if( velocityX() < 0) changeDirection(1);
  if(currState < 2)
  {
    if(!keyPressedX) stop();
    if(!keyPressedY) stop();
    keyPressedX = false;
    keyPressedY = false;
    
  }
  timeSinceLastBullet +=ticks;
  bullets.update(ticks);
  
  
}




void Enemy::punch(){
 if(currState == IDLE)
 {
  currentFrame = 0 + (currState) +(getDirection()* numberOfFrames*states);
  if(!keyPressedX ) velocityX(0);
  if(!keyPressedY ) velocityY(0);
  changeState(SHOOT);
  //see if its time to shoot 
  if(timeSinceLastBullet > bulletInterval) {
    Vector2f vel = getVelocity();
    float x;
    float y = Y()+ frameHeight/2;
    if(getDirection() == 0) {
      x = X() + frameWidth;
      vel[0] += minBulletSpeed;
      
    }
    else {
      x=X();
      vel[0] -= minBulletSpeed;
    }
    //std::cout << "computer vel:" <<vel << std::endl;
    bullets.shoot (Vector2f(x,y),vel);
    timeSinceLastBullet =0;
 }
}
}

void Enemy::stop(){
 if(!keyPressedX ) velocityX(slowDown*velocityX());
 if(!keyPressedY ) velocityY(0);
 changeState(IDLE);
}

void Enemy::retreat(){
  keyPressedY= true;
 if( X() < worldWidth-frameWidth && X() > 0){
     if(direction==0){
       velocityX(-initialVelocity[0]);
     }
     else{
	velocityX(initialVelocity[0]);
     }
 }
}


void Enemy::right(){
  keyPressedX= true;
 if( X() < worldWidth-frameWidth){
     velocityX(initialVelocity[0]);
     changeDirection(0);
      changeState(WALK);
 }
}


void Enemy::left(){
  keyPressedX= true;
 if( X() > 0){
     velocityX(-initialVelocity[0]);
     changeDirection(1);
     changeState(WALK);
 }
}


void Enemy::up(){
 keyPressedY= true;
 if( Y() >upperBound){
     velocityY(-initialVelocity[1]);
     changeState(WALK);
 }
}

void Enemy::down(){
 keyPressedY= true;
 if( Y() < worldHeight-frameHeight){
     velocityY(initialVelocity[1]);
     changeState(WALK);
 }
}



