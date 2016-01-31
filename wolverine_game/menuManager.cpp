#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include "menuManager.h"
#include "manager.h"

MenuManager::MenuManager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  screen( IOManager::getInstance().getScreen() ),
  clock( Clock::getInstance() ),
  backColor(),
  menu(),
  numberOfSprites(1)
{ 
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  backColor.r = Gamedata::getInstance().getXmlInt("back/red");
  backColor.g = Gamedata::getInstance().getXmlInt("back/green");
  backColor.b = Gamedata::getInstance().getXmlInt("back/blue");
  atexit(SDL_Quit); 
}

void MenuManager::drawBackground() const {
  SDL_FillRect( screen, NULL, 
    SDL_MapRGB(screen->format, backColor.r, backColor.g, backColor.b) );
  SDL_Rect dest = {0, 0, 0, 0};
  SDL_BlitSurface( screen, NULL, screen, &dest );
}

void MenuManager::getNumberOfSprites() {
  IOManager& io = IOManager::getInstance().getInstance();
  SDL_Event event;
  bool done = false;
  bool nameDone = false;
 
  const string msg("How many Sentinels do you want to fight?(Maximum 8): ");
  io.clearString();
  while ( not done ) {
    Uint8 *keystate = SDL_GetKeyState(NULL);
    while ( SDL_PollEvent(&event) )
    switch (event.type) {
      case SDL_KEYDOWN: {
        if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
          done = true;
        }
        if (keystate[SDLK_RETURN]) {
          nameDone = true;
        }
        io.buildString(event);
      }
    }
    drawBackground();
    
    io.printStringAfterMessage(msg, 20, 120);
    if ( nameDone ) {
      std::string number = io.getString();
      std::stringstream strm;

      strm << number;
      strm >> numberOfSprites;
      if(numberOfSprites > 8){numberOfSprites = 8;}
      strm.clear(); // clear error flags
      strm.str(std::string()); // clear contents
      strm << "Okay -- you'll see " << numberOfSprites << " Enemies";
      io.printMessageAt(strm.str(), 20, 160);
      SDL_Flip(screen);
      SDL_Delay(1000);
      done = true;
    }
    if ( !done ) {
      SDL_Flip(screen);
    }
  }
}

void MenuManager::play() {
  SDL_Event event;
  bool done = false;
  // Here, we need to create an instance of the Manager,
  // the one that manages the game not the menu:
  Manager manager(numberOfSprites);
  manager.pause();

  while ( not done ) {
    drawBackground();
    menu.draw();
    SDL_Flip(screen);
    while ( SDL_PollEvent(&event) ) {
      if (event.type ==  SDL_QUIT) { break; }
      if(event.type == SDL_KEYDOWN) {
        //std::cout << "KEY: " << event.key.keysym.sym << std::endl;
        switch ( event.key.keysym.sym ) {
          case SDLK_ESCAPE :
          case SDLK_q : {
            done = true;
            break;
          }
          case SDLK_RETURN : {
            menu.lightOn();
            if ( menu.getIconClicked() == "Start Game" ) {
              // Here is where we call the play() function in Manager;
              // but first, unpause the game:
              manager.unpause();
              manager.~Manager();
              new(&manager)Manager(numberOfSprites);
              manager.play();
              manager.pause();
            }
            if ( menu.getIconClicked() == "Parameters" ) {
              getNumberOfSprites();
              // After we get the number, we must set it to Manager:
              
            }
            
            if ( menu.getIconClicked() == "Help" ) {

              IOManager& io = IOManager::getInstance().getInstance();
		  SDL_Event event;
		  bool done = false;
		  //bool nameDone = false;
		 
		  const string msg1("Wolverine- The Game");
		  const string msg2("How To Play:");
		  const string msg3("W- Forward  S- Back  A- Left  D- Right");
		  const string msg4("Space Bar- Splice   V- Jump Smash");
		  const string msg5("Aim- Kill the Sentinels to Win the Game");

		  const string msg6("Credits:");
		  const string msg7("Developed by- Mit Dave");
		  const string msg8("Special Thanks- Dr. Brian Malloy for his wonderful guidance.");
		  
		  while ( not done ) {
		    Uint8 *keystate = SDL_GetKeyState(NULL);
		    while ( SDL_PollEvent(&event) )
		    switch (event.type) {
		      case SDL_KEYDOWN: {
			if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
			  done = true;
			}
			if (keystate[SDLK_RETURN]) {
			  done = true;
			}
		  
		      }
		    }
		    drawBackground();
		    
		    io.printStringAfterMessage(msg1, 300, 120);
		    io.printStringAfterMessage(msg2, 150, 160);
		    io.printStringAfterMessage(msg3, 150, 190);
		    io.printStringAfterMessage(msg4, 150, 220);
		    io.printStringAfterMessage(msg5, 150, 260);
		    io.printStringAfterMessage(msg6, 150, 300);
		    io.printStringAfterMessage(msg7, 150, 330);
		    io.printStringAfterMessage(msg8, 150, 360);

		    
		    if ( !done ) {
		      SDL_Flip(screen);
		    }
		  }
            }
            if ( menu.getIconClicked() == "Exit" ) {
              drawBackground();
              menu.draw();
              SDL_Flip(screen);
              SDL_Delay(250);
              done = true;
            }
          }
          break;
          case SDLK_DOWN   : {
              menu.increment();
            break;
          }
          case SDLK_UP   : {
              menu.decrement();
            break;
          }
          default          : break;
        }
      }
      if(event.type == SDL_KEYUP) { 
        menu.lightOff();
      }
    }
  }
}
