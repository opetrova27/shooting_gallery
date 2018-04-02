#pragma once
#include "FlyObject.h"

class Bullet : public FlyObject {
 public:
  Bullet(const short id, ParticleEffectPtr eff, const float speed,
         const IPoint& startPoint, const IPoint& vectorPoint, const IRect& field);
  ~Bullet();
  void Update(float dt);
  bool IsInFly() const;

 private:
  void Init(const IPoint& vectorPoint);

 private:
  bool _inFly;
  ParticleEffectPtr _eff;
};
