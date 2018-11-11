#include "stdGL.h"
#include <vector>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <SDL.h>
#include <SDL_opengl.h>
#include "map.h"
#include "ant.h"
#include "renderer.h"

using namespace std;

//////// SDL Init Function ////////

bool init()
{
  bool success = true;

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
  {
    cerr << "SDL failed to initialize: " << SDL_GetError() << endl;
    success = false;
  }

  return success;
}

///////////////////////////////////

int main(int argc, char *argv[])
{
  //Initialize
  if (init() != true)
  {
    cerr << "Shutting Down\n";
    return 1;
  }

  srand (static_cast <unsigned> (time(0)));

  Renderer* R = new Renderer();
  R->reshape(1920,1080);

  Map* M;
  M = new Map(48,48);
  R->addObject(M);
  Ant* A;

  for (int i = 0; i < 1000; i++) {
      float r1 = 1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/((M->getH()-3))));
      float r2 = 1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/((M->getW()-3))));
      A = new Ant(r1, r2, M);
      R->addObject(A);
      M->addAnt(A);
  }


  //Timing
  int r = 0;
  int dr = 0;
  int oldr = 0;
  //int Pause = 0;
  int frames = 0;

  int startuptime = SDL_GetTicks();
  oldr = startuptime;

  ////////Main Loop////////
  bool quit = false;
  while (!quit)
  {
    //cout << "handling events\n";
    quit = R->handleEvents();

    //// PHYSICS TIMING ////
    r = SDL_GetTicks();
    dr += r - oldr;
    while (dr >= 16)
    {
      //cout << "physics step\n";
      R->physics();
      dr -= 16;
    }
    oldr = r;
    R->display();
    frames += 1;
    //quit = true;
  }

  cout << "Shutting Down\n";
  cout << "average framerate: " << 1000*(float)frames/(r - startuptime) << endl;

  delete R;
  //delete M;
  //delete A;

  SDL_Quit();

  return 0;
}
