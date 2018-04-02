#include "../stdafx.h"
#include "Bullet.h"

Bullet::Bullet(const short id, ParticleEffectPtr eff, const float speed,
               const IPoint& startPoint, const IPoint& vectorPoint, const IRect& field)
  : FlyObject(id, field, speed, "bullet")
  , _eff(eff)
  , _inFly(true) {
  _startPoint = startPoint;
  Init(vectorPoint);
}

Bullet::~Bullet() {
//  Log::Info("[Bullet] destroy id=" + std::to_string(Id()));
  _eff->Finish();
  _eff = NULL;
}

void Bullet::Update(float dt) {
  FlyObject::Update(dt);
  _inFly = _inFly && 1.0f != math::clamp(0.0f, 1.0f, _timer / _splineTime);
  _eff->SetPos(GetPos());
}

bool Bullet::IsInFly() const {
  return _inFly;
}

void Bullet::Init(const IPoint& vectorPoint) {
  _timer = 0.f;
  _spline.Clear();
  _spline.addKey(0.0f, _startPoint);
  _endPoint = GetEndPointInDirection(_startPoint, vectorPoint);
  const float distance = sqrtf(pow(_endPoint.x - _startPoint.x,
                                   2.0f) + pow(_endPoint.y - _startPoint.y, 2.0f));
  _splineTime = distance / _speed;
  _spline.addKey(1.0f, _endPoint);
  _spline.CalculateGradient();

  _eff->SetPos(GetPos());
  _eff->Reset();
}
