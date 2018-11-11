#include "renderer.h"

using namespace std;

Renderer::Renderer(){ //, SDL_GLContext* c) {
  bool success = true;

  window = SDL_CreateWindow("Terrarium", 0,0, 1920,1080, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  if (window == NULL) {
    cerr << "SDL Failed to create a window: " << SDL_GetError() << endl;
    success = false;
  }

  context = SDL_GL_CreateContext(window);
  if (context == NULL) {
    cerr << "SDL failed to create an OpenGL context: " << SDL_GetError() << endl;
    success = false;
  }

  if (!success)
    exit(-1);
//  std::cout << glGetString(GL_VERSION) << std::endl;
  pixlight = CreateShaderProg((char*)"pixlight.vert", (char*)"pixlight.frag");
  //int depth_size = 0;
  //SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &depth_size);
  //cout << depth_size << endl;
}

Renderer::~Renderer(){
  for (Renderable* object : render_objects)
    delete object;
}

void Renderer::addObject(Renderable* object) {
  render_objects.push_back(object);
}

//what is this? a renderer for ants

void Renderer::reshape(int Width, int Height) {
  //std::cout << "reshape function\n";
  setW(Width);
  setH(Height);

  // new aspect ratio
  float w2h = (Height > 0) ? (double) Width/Height : 1;
  // set viewport to match new window size
  glViewport(0,0, Width,Height);

  //adjust projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  int fov = 60;
  int nearPlane = 1.0;
  int farPlane = 100;
  gluPerspective(fov, w2h, nearPlane, farPlane);

  // switch back to modelview matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void Renderer::physics() {
  const Uint8* state = SDL_GetKeyboardState(NULL);
  keyStateUpdate(state);

  // adjust eye position
  setTh(getTh() + getDth());
  setPh(getPh() + getDph());
  float z = getZoom();
  setZoom(z < 2.0 ? 2.0 : z + getDzoom());
  for (Renderable* object : render_objects){
    object->animate();
  }
}

void Renderer::display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //glClearDepth(1.0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_NORMALIZE);

  //glMatrixMode(GL_PROJECTION);
  //glLoadIdentity();
  //reshape(w,h);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // eye position
  float ex = Sin(-getTh())*Cos(getPh())*getZoom();
  float ey = Cos(-getTh())*Cos(getPh())*getZoom();
  float ez = Sin(getPh())*getZoom();
  gluLookAt(ex,ey,ez, 0,0,0, 0,0,Cos(getPh()));

  // light source
  glEnable(GL_LIGHTING);
  //glShadeModel(GL_FLAT);
  float white[4]   = {1.0,1.0,1.0,1.0};
  float pos[4]     = {0.0, 0.0, 8.0, 1.0};
  float ambient[4] = {0.12, 0.15, 0.16, 1.0};
  float diffuse[4] = {0.65, 0.65, 0.60, 1.0};
  float specular[4]= {0.7, 0.7, 0.9, 1.0};
  float shininess  = 64;

  //glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);

  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
  glLightfv(GL_LIGHT0, GL_POSITION, pos);

  glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
  glMaterialfv(GL_FRONT, GL_SPECULAR, white);

  // render objects
  glUseProgram(pixlight);

  for (Renderable* object : render_objects) {
    //std::cout << "rendering an object\n";
    object->render();
  }

  // draw light source
  glUseProgram(0);
  glDisable(GL_LIGHTING);
  glColor3f(1.0,1.0,1.0);
  ball(pos[0],pos[1],pos[2], 0.25);

  // flush to GPU and swap buffers
  //std::cout << "display sanity check\n" << ex << " " << ey << " " << ez << std::endl;
  glFlush();
  SDL_GL_SwapWindow(window);
}

void Renderer::keyStateUpdate(const Uint8* state) {
  if (state[SDL_SCANCODE_LEFT])
    setDth(-0.5);
  else if (state[SDL_SCANCODE_RIGHT])
    setDth(0.5);
  else
    setDth(0.0);

  if (state[SDL_SCANCODE_UP])
    setDph(0.5);
  else if (state[SDL_SCANCODE_DOWN])
    setDph(-0.5);
  else
    setDph(0.0);

  if (state[SDL_SCANCODE_Z])
    setDzoom(-0.1);
  else if (state[SDL_SCANCODE_X])
    setDzoom(0.1);
  else
    setDzoom(0.0);
}

int Renderer::handleEvents() {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        return 1;
        break;
      case SDL_KEYDOWN:
        switch(event.key.keysym.scancode) {
          case SDL_SCANCODE_Q:
            return 1;
            break;
          case SDL_SCANCODE_SPACE:
            Pause();
            break;
          default:
            break;
        }
      case SDL_WINDOWEVENT:
        if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
          reshape(event.window.data1, event.window.data2);
        break;
    }
  }
  return 0;
}

// All of these getters and setters
int Renderer::getW() {
  return w;
}
void Renderer::setW(int width) {
  w = width;
}

int Renderer::getH() {
  return h;
}
void Renderer::setH(int height) {
  h = height;
}

float Renderer::getZoom() {
  return zoom;
}
void Renderer::setZoom(float z) {
  zoom = z;
}

float Renderer::getDzoom() {
  return dzoom;
}
void Renderer::setDzoom(float dz) {
  dzoom = dz;
}

float Renderer::getTh() {
  return th;
}
void Renderer::setTh(float t) {
  th = t;
}

float Renderer::getPh() {
  return ph;
}
void Renderer::setPh(float p) {
  ph = p;
}

float Renderer::getDth() {
  return dth;
}
void Renderer::setDth(float t) {
  dth = t;
}

float Renderer::getDph() {
  return dph;
}
void Renderer::setDph(float p) {
  dph = p;
}

void Renderer::Pause() {
  pause = !pause;
}
