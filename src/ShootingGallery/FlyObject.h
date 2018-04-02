#pragma once

/* базовый класс летящего объекта */
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
  const IRect _field; /* Размеры зоны полета */
  const float _speed; /* Скорость полета */

  float _timer;       /* Счетчик времени для анимации по сплайну */
  TimedSpline<FPoint> _spline; /* Сплайн полета */
  IPoint _startPoint; /* Начальная точка сплайна */
  IPoint _endPoint;   /* Конечная точка сплайна */
  float _splineTime;  /* Длительность полета по сплайну */

 private:
  Render::Texture* _back;
  const int _id;      /* Идентификатор летящего объекта */
};
