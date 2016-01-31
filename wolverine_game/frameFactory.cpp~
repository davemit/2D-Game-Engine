#include "frameFactory.h"
#include "extractSurface.h"
#include "ioManager.h"
#include "SDL/SDL_rotozoom.h"
#include "vector2f.h"

FrameFactory::~FrameFactory() {

  std::map<std::string, SDL_Surface*>::iterator itSurf = surfaces.begin();
  while ( itSurf != surfaces.end() ) {
    SDL_FreeSurface( itSurf->second );
    ++itSurf;
  }
  std::map<std::string, std::vector<SDL_Surface*> >::iterator 
    surfaces = multiSurfaces.begin();
  while ( surfaces != multiSurfaces.end() ) {
    for (unsigned int i = 0; i < surfaces->second.size(); ++i) {
      SDL_FreeSurface( surfaces->second[i] );
    }
    ++surfaces;
  }
  std::map<std::string, std::vector<SDL_Surface*> >::iterator 
    pSurfaces = playerSurfaces.begin();
  while ( pSurfaces != playerSurfaces.end() ) {
    for (unsigned int i = 0; i < pSurfaces->second.size(); ++i) {
      SDL_FreeSurface( pSurfaces->second[i] );
    }
    ++pSurfaces;
  }
  std::map<std::string, std::vector<SDL_Surface*> >::iterator 
    eSurfaces = enemySurfaces.begin();
  while ( eSurfaces != enemySurfaces.end() ) {
    for (unsigned int i = 0; i < eSurfaces->second.size(); ++i) {
      SDL_FreeSurface( eSurfaces->second[i] );
    }
    ++eSurfaces;
  }
  
  std::map<std::string, Frame*>::iterator frame = frames.begin();
  while ( frame != frames.end() ) {
    delete frame->second;
    ++frame;
  }
  std::map<std::string, std::vector<Frame*> >::iterator 
    frames = multiFrames.begin();
  while ( frames != multiFrames.end() ) {
    for (unsigned int i = 0; i < frames->second.size(); ++i) {
      delete frames->second[i];
    }
    ++frames;
  }
  std::map<std::string, std::vector<Frame*> >::iterator 
    pFrames = playerFrames.begin();
  while ( pFrames != playerFrames.end() ) {
    for (unsigned int i = 0; i < pFrames->second.size(); ++i) {
      delete pFrames->second[i];
    }
    ++pFrames;
  }

  std::map<std::string, std::vector<Frame*> >::iterator 
    eFrames = enemyFrames.begin();
  while ( eFrames != enemyFrames.end() ) {
    for (unsigned int i = 0; i < eFrames->second.size(); ++i) {
      delete eFrames->second[i];
    }
    ++eFrames;
  }

  
}

FrameFactory& FrameFactory::getInstance() {
  static FrameFactory factory;
  return factory;
}

Frame* FrameFactory::getFrame(const std::string& name) {
    std::map<std::string, Frame*>::const_iterator pos = frames.find(name); 
  if ( pos == frames.end() ) {
    SDL_Surface * const surface =
      IOManager::getInstance().loadAndSet(
          gdata.getXmlStr(name+"/file"),
          gdata.getXmlBool(name+"/transparency"));
    surfaces[name] = surface;
    Frame * const frame =new Frame(name, surface);
    frames[name] = frame;
    return frame;
  }
  else {
    return pos->second;
  }
}

std::vector<Frame*> FrameFactory::getFrames(const std::string& name) { 
   
  std::map<std::string, std::vector<Frame*> >::const_iterator 
    pos = multiFrames.find(name); 
  if ( pos != multiFrames.end() ) {
    return pos->second;
  }

  SDL_Surface* zSurf = IOManager::
     getInstance().loadAndSet(gdata.getXmlStr(name+"/file"), true);

  unsigned numberOfFrames = gdata.getXmlInt(name+"/frames");
  std::vector<Frame*> frames;
  std::vector<SDL_Surface*> surfaces;
  frames.reserve(numberOfFrames);
  Uint16 srcX = gdata.getXmlInt(name+"/srcX");
  Uint16 srcY = gdata.getXmlInt(name+"/srcY");
  Uint16 width = gdata.getXmlInt(name+"/width");
  Uint16 height =gdata.getXmlInt(name+"/height");

  SDL_Surface* surf;
  
  for (unsigned i = 0; i < 2*numberOfFrames; ++i) {
    unsigned frameX = (i%numberOfFrames) * width + srcX;
   if(i<numberOfFrames){
   surf = ExtractSurface::getInstance().
               get(zSurf, width, height, frameX, srcY); 
  
   }
   else {
    surf = ExtractSurface::getInstance().
               getFlip(zSurf, width, height, frameX, srcY);
  
	
   }

      surfaces.push_back( surf );
      frames.push_back( new Frame(name,surf) );
  }
//  SDL_FreeSurface(surface);
  SDL_FreeSurface(zSurf);
  //SDL_FreeSurface(surf);
  multiSurfaces[name] = surfaces;
  multiFrames[name] = frames;
  return frames;
}
//Player Frames
std::vector<Frame*> FrameFactory::getPlayerFrames(const std::string& name) {
  // First search map to see if we've already made it:
  std::map<std::string, std::vector<Frame*> >::const_iterator 
    pos = playerFrames.find(name); 
  if ( pos != playerFrames.end() ) {
    return pos->second;
  }

  // It wasn't in the map, so we have to make the vector of Frames:
  SDL_Surface* surface = IOManager::
     getInstance().loadAndSet(gdata.getXmlStr(name+"/file"), true);
  unsigned framesInState = gdata.getXmlInt(name+"/frames");
  unsigned states = gdata.getXmlInt(name+"/states");
  unsigned numberOfFrames = framesInState*states;

  std::vector<Frame*> frames;
  std::vector<SDL_Surface*> surfaces;
  frames.reserve(numberOfFrames);
  Uint16 srcX = gdata.getXmlInt(name+"/srcX");
  Uint16 srcY = gdata.getXmlInt(name+"/srcY");
  Uint16 width = gdata.getXmlInt(name+"/width");
  Uint16 height = gdata.getXmlInt(name+"/height");

  SDL_Surface* surf;
  
  for (unsigned i = 0; i < 2*numberOfFrames; ++i) {
    unsigned frameX = (i%numberOfFrames) * width + srcX;
  
   if(i<numberOfFrames){
   surf = ExtractSurface::getInstance().
               get(surface, width, height, frameX, srcY); 
   
   }
   else {
    surf = ExtractSurface::getInstance().
               getFlip(surface, width, height, frameX, srcY);
   
	
   }


   surfaces.push_back( surf );
   frames.push_back( new Frame(name, surf) );
  }
  SDL_FreeSurface(surface);
  playerSurfaces[name] = surfaces;
  playerFrames[name] = frames;
  return frames;
}




//Enemy Frames
std::vector<Frame*> FrameFactory::getEnemyFrames(const std::string& name) {
  // First search map to see if we've already made it:
  std::map<std::string, std::vector<Frame*> >::const_iterator 
    pos = enemyFrames.find(name); 
  if ( pos != enemyFrames.end() ) {
    return pos->second;
  }

  // It wasn't in the map, so we have to make the vector of Frames:
  SDL_Surface* surface = IOManager::
     getInstance().loadAndSet(gdata.getXmlStr(name+"/file"), true);
  unsigned framesInState = gdata.getXmlInt(name+"/frames");
  unsigned states = gdata.getXmlInt(name+"/states");
  unsigned numberOfFrames = framesInState*states;

  std::vector<Frame*> frames;
  std::vector<SDL_Surface*> surfaces;
  frames.reserve(numberOfFrames);
  Uint16 srcX = gdata.getXmlInt(name+"/srcX");
  Uint16 srcY = gdata.getXmlInt(name+"/srcY");
  Uint16 width = gdata.getXmlInt(name+"/width");
  Uint16 height = gdata.getXmlInt(name+"/height");

  SDL_Surface* surf;
  
  for (unsigned i = 0; i < 2*numberOfFrames; ++i) {
    unsigned frameX = (i%numberOfFrames) * width + srcX;
  
   if(i<numberOfFrames){
   surf = ExtractSurface::getInstance().
               get(surface, width, height, frameX, srcY); 
   
   }
   else {
    surf = ExtractSurface::getInstance().
               getFlip(surface, width, height, frameX, srcY);
   
	
   }


   surfaces.push_back( surf );
   frames.push_back( new Frame(name, surf) );
  }
  SDL_FreeSurface(surface);
  enemySurfaces[name] = surfaces;
  enemyFrames[name] = frames;
  return frames;
}



