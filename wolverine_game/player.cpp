#include "player.h"
#include "gamedata.h"
#include "frameFactory.h"






void Player::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
  	if((currentFrame % numberOfFrames) >= 5 && currState >= SLICE ) {
      changeState(IDLE);
    }
    currentFrame = ((currentFrame+1) % numberOfFrames)+(getDirection()*(numberOfFrames*states))    +((getState())*numberOfFrames);
         
		timeSinceLastFrame = 0;
        
	}
}

Player::Player( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  frames( FrameFactory::getInstance().getPlayerFrames(name) ),
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
  currState(IDLE),
  originPosY(0),
  upperBound(Gamedata::getInstance().getXmlInt(name+"/bound/upper")),
  isExploding(false)
{
	Y(Gamedata::getInstance().getXmlInt(name+"/startLoc/y"));  
}

Player::Player(const Player& s) :
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
  isExploding(s.isExploding)
  { }

void Player::draw() const { 
  if(!isExploding)
  {
   Uint32 x = static_cast<Uint32>(X());
   Uint32 y = static_cast<Uint32>(Y());
   frames[currentFrame]->draw(x, y);
  }
  
}
void Player::reset(){

   X(Gamedata::getInstance().getXmlInt(getName()+"/startLoc/x"));
   Y(Gamedata::getInstance().getXmlInt(getName()+"/startLoc/y")); 
                   


} 

bool Player::collidedWith(const Drawable* obj1,const Drawable* obj2){

	if(execute(*obj1, *obj2)){
           if(currState == SLICE)
           {

		if((currentFrame % numberOfFrames) == 3 ||(currentFrame % numberOfFrames) == 5 ){ 
			if(abs(obj1->Y()-obj2->Y())< 10){
			  return true;
			}
		}
	   }
           if(currState == JUMP)
           {

		if((currentFrame % numberOfFrames) == 4 ||(currentFrame % numberOfFrames) == 5 ) {
		if(abs(obj1->Y()-obj2->Y())< 20){
			  return true;
		  }
		}
	   } 
	   return false;
	
	}
	return false;
}
void Player::update(Uint32 ticks) { 
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  if(currState != JUMP && currState != SLICE){
	  if ( Y() < upperBound) {
	    velocityY( abs( velocityY() ) );
	    changeState(IDLE);

	  }
	  if ( Y() > worldHeight-frameHeight) {
	    velocityY( -abs( velocityY() ) );
	    Y(worldHeight-frameHeight);
		  changeState(IDLE);
	  }

	  if ( X() <= 0) {
	    velocityX( abs( velocityX() ) );
	    changeState(IDLE);
	  }
	  if ( X() >= worldWidth-frameWidth) {
	    velocityX( -abs( velocityX() ) );
		changeState(IDLE);
	  } 
  }
  int jX;
  int jY;

  int yChange[] = {0,-20,-60,-40,-20,0};
  if(currState == JUMP)
  { 

     if(direction ==0)
     {
      jX= X()+7;
     }
     else
     {
      jX= X()-7;
     }
     X(jX);
     jY = originPosY + yChange[currentFrame%6];
     Y(jY);
      
  }
  if( velocityX() > 0) changeDirection(0);
  if( velocityX() < 0) changeDirection(1);
  if(currState < SLICE)
  {
    if(!keyPressedX) stop();
    if(!keyPressedY) stop();
    keyPressedX = false;
    keyPressedY = false;
    
  }
  
  
  
}


void Player::jumpAttack(){
 if(currState == IDLE)
 {
  currentFrame = 0 + (currState) +(getDirection()* numberOfFrames*states);
  if(!keyPressedX ) velocityX(0);
  if(!keyPressedY ) velocityY(0);
  originPosY = Y();
  changeState(JUMP);
  
 }
}

void Player::punch(){
 if(currState == IDLE)
 {
  currentFrame = 0 + (currState) +(getDirection()* numberOfFrames*states);
  if(!keyPressedX ) velocityX(0);
  if(!keyPressedY ) velocityY(0);
  changeState(SLICE);
  
}
}

void Player::stop(){
 if(!keyPressedX ) velocityX(slowDown*velocityX());
 if(!keyPressedY ) velocityY(0);
 changeState(IDLE);
 if(Y()>worldHeight-frameHeight) Y(worldHeight-frameHeight);
}

void Player::right(){
 keyPressedX= true;
 if( X() < worldWidth-frameWidth){
     velocityX(initialVelocity[0]);
     changeDirection(0);
      changeState(WALK);
 }
}


void Player::left(){
 keyPressedX= true;
 if( X() > 0){
     velocityX(-initialVelocity[0]);
     changeDirection(1);
     changeState(WALK);
 }
}


void Player::up(){
 keyPressedY= true;
 if( Y() >upperBound){
     velocityY(-initialVelocity[1]);
     changeState(WALK);
 }
}

void Player::down(){
 keyPressedY= true;
 if( Y() < worldHeight-frameHeight){
     velocityY(initialVelocity[1]);
     changeState(WALK);
 }
}



