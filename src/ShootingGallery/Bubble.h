#pragma once
#include "FlyObject.h"

class Bubble: public FlyObject {
 public:
  Bubble(const short id, const IRect& field);
  ~Bubble();
  void Update(float dt);

 private:
  void Init();
  void CalculateNewSpline(const IPoint& vectorPoint);
};
