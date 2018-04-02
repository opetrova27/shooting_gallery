#pragma once

class Gun {
 public:
  Gun(const IPoint& pos, const float strengthMax);
  ~Gun();

  void Draw();
  void Update(float dt);

  bool MouseDown(const IPoint& mouse_pos);
  void MouseMove(const IPoint& mouse_pos);
  void MouseUp(const IPoint& mouse_pos);

  int GetCurrentStrength() const;
  IPoint GetPos() const;

 private:
  void Init();
  float GetAngleBetweenTwoLines(const IPoint& line1point1,
                                const IPoint& line1point2,
                                const IPoint& line2point1,
                                const IPoint& line2point2) const;

 private:
  Render::Texture* _back;
  Render::Texture* _overlayer;
  const IPoint _pos;
  float _angle;
  float _gunStrength;
  const float _gunStrengthMax;
};
