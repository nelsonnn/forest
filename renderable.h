// renderable interface
#ifndef RENDERABLE_H
#define RENDERABLE_H

class Renderable {
public:
  virtual void animate() = 0;
  virtual void render() = 0;
};

#endif
