#pragma once

class FlyObject {
 public:
  FlyObject(const short id, const IRect& field, const float speed,
            const std::string& textureName);
  ~FlyObject();

  void Draw();
  void Update(float dt);
  bool IsIntersectedWithRect(const IRect& rect) const;
  IRect GetRect() const;
  IPoint GetPos() const;
  int Id() const;

 protected:
  IPoint GetIntersectionPointOfTwoLines(const IPoint& line1point1,
                                        const IPoint& line1point2,
                                        const IPoint& line2point1,
                                        const IPoint& line2point2) const;
  IPoint GetRandPointInField(const IRect& field) const;
  IPoint GetEndPointInDirection(const IPoint& startPoint,
                                const IPoint& vectorPoint) const;

 protected:
  const IRect _field; /* Область движения */
  const float _speed; /* Постоянная скорость движения */

  float _timer;       /* Текущий момент сплайна */
  TimedSpline<FPoint> _spline; /* Сплайн движения объекта */
  IPoint _startPoint; /* Начальная точка движения объекта */
  IPoint _endPoint;   /* Конечная точка движения объекта */
  float _splineTime;  /* Рассчетное время движения объекта */

 private:
  Render::Texture* _back;
  const int _id;      /* Идентификатор */
};
