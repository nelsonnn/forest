#ifndef RENDERER_H
#define RENDERER_H

#include "stdGL.h"
#include "shader.h"
#include "renderable.h"
#include "objects.h"
#include <iostream>
#include <SDL.h>
#include <SDL_opengl.h>
#include <vector>

class Renderer {
private:
  // pause state
  bool pause = false;

  // window size
  int w = 1920;
  int h = 1080;

  // eye postion
  float zoom = 30.0;
  float dzoom = 0;
  float th =  0;
  float ph = 45;
  float dth = 0;
  float dph = 0;

  // Shader
  int pixlight = 0;

  // window object
  SDL_Window* window = NULL;
  SDL_GLContext context;

  // objects to render
  std::vector<Renderable*> render_objects;

public:
  // Constructor/Destructor
  Renderer();//, SDL_GLContext* context);
  ~Renderer();

  // update and render
  void physics();
  void display();

  // manage internal variables
  void reshape(int width, int height);
  void addObject(Renderable* object);

  //void removeNullObjects();
  void keyStateUpdate(const Uint8* state);
  int handleEvents();
  // generic gets and sets
  int getW();
  void setW(int width);
  int getH();
  void setH(int height);
  float getZoom();
  void setZoom(float z);
  float getDzoom();
  void setDzoom(float dz);
  float getTh();
  void setTh(float t);
  float getPh();
  void setPh(float p);
  float getDth();
  void setDth(float dt);
  float getDph();
  void setDph(float dp);
  void Pause();
};

#endif
