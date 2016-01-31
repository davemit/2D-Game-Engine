#include <iostream>
#include <string>
#include <iomanip>
#include "multisprite.h"
#include "sprite.h"
#include "player.h"
#include "enemy.h"
#include "gamedata.h"
#include "manager.h"
#include "multiplesprite.h"

#define stEnemy dynamic_cast<Enemy*>
#define stPlayer dynamic_cast<Player*>




Manager::~Manager() {
  // These deletions eliminate "definitely lost" and
  // "still reachable"s in Valgrind.
   std::list<Drawable*>::iterator itr = sprites.begin();
  while(itr!=sprites.end()){
    delete *itr;
     ++itr;
  }
  std::list<Drawable*>::iterator bitr = bSprites.begin();
  while(bitr!=bSprites.end()){
    delete *bitr;
     ++bitr;
  }
  //Exploding sprites
  std::list<ExplodingSprite*>::iterator exitr = exSprites.begin();
  while(exitr!=exSprites.end()){
    delete *exitr;
     ++exitr;
  }
   
  
}

Manager::Manager(int n) :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  world("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  world_river("river", Gamedata::getInstance().getXmlInt("river/factor") ),
  world_city("city", Gamedata::getInstance().getXmlInt("city/factor") ),
  hud_flag(false),
  lose_flag(false),
  win_flag(false),
  totalEnemy(n),
  enemyKilled(0),
  bar(),
  viewport( Viewport::getInstance() ),
  sprites(),
  //sortSprites(),
  currentSprite(0),
  bSprites(),
  exSprites(),
  playerPt(),
  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ),
  sound(),
  bList(0),
  fList(0),
  playerHealth(100),
  godMode(false)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);
  
  sprites.push_back( new Player("wolverine") );
  for(int i =0; i< n;i++){
    sprites.push_back( new Enemy("sentinel",i) );
  }  

  makeSprites("boat",Gamedata::getInstance().getXmlInt("boat/totalInstance"));
  std::list<Drawable*>::iterator itr = sprites.begin();
  playerPt = dynamic_cast<Player*>(*itr);
  for(int i=0;i<currentSprite;++i){
     ++itr;
  }
  
  viewport.setObjectToTrack(*itr);
}



void Manager::makeSprites(const std::string& name, int totalInstance)
{
	for(int i=0;i< totalInstance;i++)
	{
	  bSprites.push_back(new MultiSprite(name, i));
	}
}


void Manager::draw() const {
  world_city.draw();
  world_river.draw();
  std::list<Drawable*>::const_iterator bitr = bSprites.begin();
  while(bitr!=bSprites.end()){
     (*bitr)->draw();
      ++bitr;
  }
  world.draw();
  
  
  std::list<Drawable*>::const_iterator itr = sprites.begin();
  while(itr!=sprites.end()){
     (*itr)->draw();
      ++itr;
  }
  //Exploding sprites
  std::list<ExplodingSprite*>::const_iterator exitr = exSprites.begin();
  while(exitr!=exSprites.end()){
    (*exitr)->draw();
     ++exitr;
  }
  //io.printMessageValueAt("Seconds: ", clock.getSeconds(), 10, 20);
  //io.printMessageValueAt("fps: ", clock.getAvgFps(), 10, 40);
  //io.printMessageAt("Press T to switch sprites", 10, 70);
  io.printMessageAt(title, 10, 400);
  viewport.draw();
  if(clock.getSeconds() <3){
    bar.drawHud(screen, io, clock.getSeconds(),clock.getAvgFps(),bList,fList,playerHealth);
   }
  else{
    if(hud_flag)
    {
	bar.drawHud(screen, io,clock.getSeconds(),clock.getAvgFps(),bList,fList,playerHealth);


    }
  }
  if(lose_flag){

	bar.drawLose(screen,io);
    
  }
  if(win_flag){

	bar.drawWin(screen,io);
    
  }
  

  
 

  
  SDL_Flip(screen);
}

void Manager::makeFrame() {
  std::stringstream strm;
  strm << "frames/" << username<< '.' 
       << std::setfill('0') << std::setw(4) 
       << frameCount++ << ".bmp";
  std::string filename( strm.str() );
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screen, filename.c_str());
}

void Manager::update() {
  ++clock;
  Uint32 ticks = clock.getElapsedTicks();

  std::list<Drawable*>::iterator itr = sprites.begin();
  while(itr!=sprites.end()){
      (*itr)->update(ticks);
      ++itr;
  }
  manageEnemy();
  //creating collision for player
  
  std::list<Drawable*>::iterator colitr = sprites.begin();
//   ++colitr;
  while(colitr!=sprites.end() ){
    
    if(stEnemy(*colitr)){
      if(stEnemy(*colitr)->collidedWith(playerPt,
         getAbs(playerPt->Y()+playerPt->getHeight(),stEnemy(*colitr)->Y()+stEnemy(*colitr)->getHeight() )
	)&&!godMode){
	  sound[6];
	  playerHealth -= 20 ;
          hud_flag = true;
          if(playerHealth == 0){	
            playerPt->changeExplode(true);
	    Vector2f vel = playerPt->getVelocity();
            Vector2f pos = playerPt->getPosition();
	    Sprite *temp = new Sprite("wExp",pos, vel);
       	    exSprites.push_back(new ExplodingSprite(*temp));
            delete temp;
          }
          else{
		playerPt->stop();
		
          }
        }
      bList = stEnemy(*colitr)->getBulletlistSize();
      fList = stEnemy(*colitr)->getFreelistSize();
     //for player
     if(playerPt->collidedWith(playerPt,*colitr)&& !stEnemy(*colitr)->getExplode()){
       	  sound[3];
       	  enemyKilled++;
          std::cout << enemyKilled <<std::endl;
          stEnemy(*colitr)->changeExplode(true);
          Vector2f vel = (*colitr)->getVelocity();
          Vector2f pos = (*colitr)->getPosition();
          Sprite *temp = new Sprite("sExp",pos, vel);
          exSprites.push_back(new ExplodingSprite(*temp));
          delete temp;
	
     }

     //Exploding sprites
  	std::list<ExplodingSprite*>::iterator exitr = exSprites.begin();
  	while(exitr!=exSprites.end()){
    	(*exitr)->update(ticks);
    	if((*exitr)->done()){ 
        	if(playerPt->getExplode()){

		 clock.pause();
                 lose_flag =true;
                 hud_flag = false;
	 	
		}
		if(stEnemy(*colitr)->getExplode()){

                if(enemyKilled == totalEnemy){
		  win_flag =true;
                  hud_flag = false;
                }

		}
        exitr = exSprites.erase(exitr);
        
	}
      ++exitr;
      }
    }//end of if
      ++colitr;
  }


  

  //end of player collision
  std::list<Drawable*>::iterator bitr = bSprites.begin();
  while(bitr!=bSprites.end()){
      (*bitr)->update(ticks);
      ++bitr;
  }
 
  if ( makeVideo && frameCount < frameMax ) {
    makeFrame();
  }
  if(clock.getSeconds() %9 ==0)hud_flag = false;
  world_city.update();
  world_river.update();
  world.update();
 

  viewport.update(); // always update viewport last
}



//Player Enemy Interaction
int Manager::getAbs(int x1, int x2){

	return abs(x1-x2);
}

void Manager::manageEnemy(){
  enum STATES {IDLE, WALK, RETREAT, SHOOT};
   std::list<Drawable*>::iterator colitr = sprites.begin();
  
   
   while(colitr!=sprites.end()){
   if(stEnemy(*colitr)){
	  int pX = playerPt->X();
	  int pY = playerPt->Y()+playerPt->getHeight();
	  int eX = stEnemy(*colitr)->X();
	  int eY = stEnemy(*colitr)->Y()+stEnemy(*colitr)->getHeight();
	  //Direction   
	  if(pX<eX)
	  {
		stEnemy(*colitr)->changeDirection(1);
	  }
	  else
	  {
		stEnemy(*colitr)->changeDirection(0);
	  }
	  //chase
	  if(getAbs(pX,eX) <= 250 && getAbs(pY,eY) <= 5)
	  {
	    if(stEnemy(*colitr)->getState()<RETREAT)
	    {
	      if(!playerPt->getExplode() && !stEnemy(*colitr)->getExplode())
	      {
		stEnemy(*colitr)->punch();
	      }
	      else
	      {
		stEnemy(*colitr)->stop();
	      }
	    }
	    
             

	  }
	  else //chasing code
	  {
	    if(stEnemy(*colitr)->getState()<RETREAT &&!playerPt->getExplode() && !stEnemy(*colitr)->getExplode())
	    {
		stEnemy(*colitr)->changeState(WALK);
		if(pY < eY){stEnemy(*colitr)->up();}
		else{stEnemy(*colitr)->down();}
		if(pX < eX){stEnemy(*colitr)->left();}
		else{stEnemy(*colitr)->right();}
	    }
     	 }
       
	if( stEnemy(*colitr)->getState()==RETREAT){
		if(getAbs(pX,eX) <= 350){
		  stEnemy(*colitr)->retreat();
                }
                else{
		  stEnemy(*colitr)->stop();
		}
	}
	
    }
    ++colitr;
  
  }
     
}

class Compare{
 public:
   bool operator()(Drawable* lhs, Drawable* rhs){
     return lhs->Y() < rhs->Y();
   }
 }; 

void Manager::play() {
  SDL_Event event;

  bool done = false;
  bool keyCatch = false;
  while ( not done ) {

    SDL_PollEvent(&event);
    Uint8 *keystate = SDL_GetKeyState(NULL);
    if (event.type ==  SDL_QUIT) { done = true; break; }
    if(event.type == SDL_KEYUP) { 
      keyCatch = false; 
    }
    if(event.type == SDL_KEYDOWN) {
      if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
        done = true;
        break;
      }
      if (keystate[SDLK_F1] && !keyCatch) {
        keyCatch =true;
        if(!hud_flag){
        hud_flag=true;
	}
        else{
         hud_flag=false;
        }
        draw();
      }
      
      if (keystate[SDLK_t] && !keyCatch) {
        keyCatch = true;
        currentSprite = (currentSprite+1) % sprites.size();
        std::list<Drawable*>::iterator itr = sprites.begin();
        for(int i=0;i<currentSprite;++i){
          ++itr;
        }
	viewport.setObjectToTrack(*itr);
      }
      if (keystate[SDLK_g] && !keyCatch) {
        keyCatch = true;
        if(!godMode){
        godMode=true;
	}
        else{
         godMode=false;
        }
      }
      

      if (keystate[SDLK_m] && !keyCatch) {
        keyCatch = true;
        clock.toggleSloMo();
      }
        if (keystate[SDLK_p] && !keyCatch) {
          keyCatch = true;
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if (keystate[SDLK_a] ) {
          if(!playerPt->getExplode())
          playerPt->left();
          
        }
        if (keystate[SDLK_d] ) {
        if(!playerPt->getExplode())
	playerPt->right();
          
        }
        if (keystate[SDLK_s] ) {
	if(!playerPt->getExplode())
	playerPt->down();
          
        }
        if (keystate[SDLK_w] ) {
	if(!playerPt->getExplode())
	playerPt->up();
          
        }
        if (keystate[SDLK_SPACE] ) {
        keyCatch = true;
	if(!playerPt->getExplode()){
       	   
	   playerPt->punch();
	}
          
        }
        if (keystate[SDLK_v] ) {
        keyCatch = true;
	if(!playerPt->getExplode())
	playerPt->jumpAttack();
          
        }
        
     
        if (keystate[SDLK_F4] && !makeVideo) {
          std::cout << "Making video frames" << std::endl;
          makeVideo = true;
        }
    }
    
    sprites.sort(Compare());
    draw();
    update();
    
  }
}



