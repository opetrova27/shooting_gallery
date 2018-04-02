#include "../stdafx.h"
#include <stdlib.h>     /* rand */
#include <math.h>       /* sqrt, pow  */
#include "FlyObject.h"

FlyObject::FlyObject(const short id, const IRect& field, const float speed,
                     const std::string& textureName)
  : _splineTime(.0f)
  , _speed(speed)
  , _field(field)
  , _id(id)
  , _timer(0)
  , _startPoint(0, 0)
  , _endPoint(0, 0) {
  _back = Core::resourceManager.Get<Render::Texture>(textureName);
}

FlyObject::~FlyObject() {
}

void FlyObject::Draw() {
  const float frame = math::clamp(0.0f, 1.0f, _timer / _splineTime);
  const FPoint currentPosition = _spline.getGlobalFrame(frame);
  Render::device.PushMatrix();
  Render::device.MatrixTranslate(currentPosition.x - static_cast<int>
                                 (_back->Width() / 2),
                                 currentPosition.y - static_cast<int>(_back->Height() / 2), 0);
  _back->Draw();
  Render::device.PopMatrix();
}

void FlyObject::Update(float dt) {
  _timer += dt * 2;
}

bool FlyObject::IsIntersectedWithRect(const IRect& rect) const {
  return GetRect().Intersects(rect);
}

IRect FlyObject::GetRect() const {
  const float frame = math::clamp(0.0f, 1.0f, _timer / _splineTime);
  FPoint currentPosition = _spline.getGlobalFrame(frame);
  return IRect(currentPosition.Rounded(), _back->Width(),
               _back->Height());
}

IPoint FlyObject::GetPos() const {
  const float frame = math::clamp(0.0f, 1.0f, _timer / _splineTime);
  return _spline.getGlobalFrame(frame).Rounded();
}

int FlyObject::Id() const {
  return _id;
}

IPoint FlyObject::GetIntersectionPointOfTwoLines(const IPoint& line1point1,
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

  const int denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

  if (denominator != 0) {
    const int x = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) *
                   (x3 * y4 - y3 * x4)) / denominator;
    const int y = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) *
                   (x3 * y4 - y3 * x4)) / denominator;
    return IPoint(x, y);
  } else {
    return IPoint(INT_MAX, INT_MAX);
  }
}

IPoint FlyObject::GetRandPointInField(const IRect& rect) const {
  return IPoint (rect.LeftBottom().x + rand() % (rect.RightBottom().x -
                 rect.LeftBottom().x),
                 rect.LeftBottom().y + rand() % (rect.LeftTop().y - rect.LeftBottom().y));
}

IPoint FlyObject::GetEndPointInDirection(const IPoint& startPoint,
    const IPoint& vectorPoint) const {
  const IPoint IntersectionBot = GetIntersectionPointOfTwoLines(startPoint,
                                 vectorPoint, _field.LeftBottom(), _field.RightBottom());
  const IPoint IntersectionTop = GetIntersectionPointOfTwoLines(startPoint,
                                 vectorPoint, _field.LeftTop(), _field.RightTop());
  const IPoint IntersectionLeft = GetIntersectionPointOfTwoLines(startPoint,
                                  vectorPoint, _field.LeftBottom(), _field.LeftTop());
  const IPoint IntersectionRight = GetIntersectionPointOfTwoLines(startPoint,
                                   vectorPoint, _field.RightTop(), _field.RightBottom());

  std::vector<IPoint> possibleNextPoints;
  if (IntersectionTop != startPoint && _field.Contains(IntersectionTop)) {
    possibleNextPoints.push_back(IntersectionTop);
  }
  if (IntersectionBot != startPoint && _field.Contains(IntersectionBot)) {
    possibleNextPoints.push_back(IntersectionBot);
  }
  if (IntersectionRight != startPoint && _field.Contains(IntersectionRight)) {
    possibleNextPoints.push_back(IntersectionRight);
  }
  if (IntersectionLeft != startPoint && _field.Contains(IntersectionLeft)) {
    possibleNextPoints.push_back(IntersectionLeft);
  }
  return possibleNextPoints[rand() % possibleNextPoints.size()];
}
