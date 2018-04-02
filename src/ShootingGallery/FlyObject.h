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
  const IRect _field; /* ������� �������� */
  const float _speed; /* ���������� �������� �������� */

  float _timer;       /* ������� ������ ������� */
  TimedSpline<FPoint> _spline; /* ������ �������� ������� */
  IPoint _startPoint; /* ��������� ����� �������� ������� */
  IPoint _endPoint;   /* �������� ����� �������� ������� */
  float _splineTime;  /* ���������� ����� �������� ������� */

 private:
  Render::Texture* _back;
  const int _id;      /* ������������� */
};
