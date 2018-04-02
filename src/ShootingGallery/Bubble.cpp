#include "../stdafx.h"
#include <stdlib.h>     /* rand */
#include <math.h>       /* sqrt, pow  */
#include "Bubble.h"

/* Скорость мишений рандомная от 80 до 280 */
Bubble::Bubble(const short id, const IRect& field) :
  FlyObject(id, field, 80 + rand() % 200, "bubble")  {
  Init();
}

Bubble::~Bubble() {
}

void Bubble::Update(float dt) {
  FlyObject::Update(dt);

  const float frame = math::clamp(0.0f, 1.0f, _timer / _splineTime);
  if (frame == 1.0f) {
    const IPoint vectorPoint = _startPoint + IPoint((_endPoint.x -
                               _startPoint.x) * 2 * 1, 0);
    _startPoint = _endPoint;
    CalculateNewSpline(vectorPoint);
  }
}

void Bubble::Init() {
  IRect centerField(_field);
  centerField.Inflate(-1 * (_field.width < _field.height ? _field.width / 4 :
                            _field.height / 4));
  _startPoint = GetRandPointInField(IRect(centerField));
  const IPoint vectorPoint = GetRandPointInField(IRect(_field));
  CalculateNewSpline(vectorPoint);
}

void Bubble::CalculateNewSpline(const IPoint& vectorPoint) {
  _timer = 0.f;
  _spline.Clear();
  _spline.addKey(0.0f, _startPoint);
  _endPoint = GetEndPointInDirection(_startPoint, vectorPoint);
  const float distance = sqrtf(pow(_endPoint.x - _startPoint.x,
                                   2.0f) + pow(_endPoint.y - _startPoint.y, 2.0f));
  _splineTime = distance / _speed;
  _spline.addKey(1.0f, _endPoint);
  _spline.CalculateGradient();
}
