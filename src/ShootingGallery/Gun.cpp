#include "../stdafx.h"
#include <math.h>       /* sqrt, pow  */
#include "Gun.h"

Gun::Gun(const IPoint& pos, const float strengthMax)
  : _pos(pos)
  , _angle(.0f)
  , _gunStrength(.0f)
  , _gunStrengthMax(strengthMax) {
  Init();
}

Gun::~Gun() {
}

void Gun::Draw() {
  Render::device.PushMatrix();
  Render::device.MatrixTranslate(_pos);
  Render::device.MatrixScale(1.f + 0.4f * _gunStrength / _gunStrengthMax);
  Render::device.MatrixRotate(math::Vector3(0, 0, 1), _angle);
  Render::device.MatrixTranslate(-_back->Width() / 2, -_back->Height() / 2, 0);
  _back->Draw();
  Render::device.PopMatrix();

  Render::device.PushMatrix();
  Render::device.MatrixTranslate(_pos.x - _overlayer->Width() / 2,
                                 _pos.y - _back->Height(), 0);
  _overlayer->Draw();
  Render::device.PopMatrix();
}

void Gun::Update(float dt) {
  if (_gunStrength > .0f && _gunStrength <= _gunStrengthMax) {
    _gunStrength += 10;
  }
}

bool Gun::MouseDown(const IPoint &mouse_pos) {
  _gunStrength += 1.f;
  return false;
}

void Gun::MouseMove(const IPoint &mouse_pos) {
  _angle = GetAngleBetweenTwoLines(mouse_pos, _pos, IPoint(0, 0), IPoint(1024,
                                   0));
}

void Gun::MouseUp(const IPoint &mouse_pos) {
  _gunStrength = .0f;
}

int Gun::GetCurrentStrength() const {
  return static_cast<int>(_gunStrength);
}

IPoint Gun::GetPos() const {
  return _pos;
}

void Gun::Init() {
  _back = Core::resourceManager.Get<Render::Texture>("gun");
  _overlayer = Core::resourceManager.Get<Render::Texture>("gunover");
}

float Gun::GetAngleBetweenTwoLines(const IPoint& line1point1,
                                   const IPoint& line1point2,
                                   const IPoint& line2point1,
                                   const IPoint& line2point2) const {
  const short x1 = line1point1.x;
  const short y1 = line1point1.y;
  const short x2 = line1point2.x;
  const short y2 = line1point2.y;
  const short x3 = line2point1.x;
  const short y3 = line2point1.y;
  const short x4 = line2point2.x;
  const short y4 = line2point2.y;
  /*
  (y1 - y2)*x + (x2 - x1) * y + (x1*y2 - x2*y1) = 0
  (y3 - y4)*x + (x4 - x3) * y + (x3*y4 - x4*y3) = 0
  */
  const float anglecos = ((y1 - y2) * (y3 - y4) + (x2 - x1) * (x4 - x3)) /
                         ( sqrt( pow(y1 - y2, 2.0f) + pow(x2 - x1, 2.0f)) *
                           sqrt( pow(y3 - y4, 2.0f) + pow(x4 - x3, 2.0f)) );
  return 90.f - std::acos(anglecos) * 180.f / math::PI;
}