#pragma once
#include "ShootingGallery/bubble.h"
#include "ShootingGallery/bullet.h"
#include "ShootingGallery/gun.h"

typedef std::tr1::shared_ptr<Gun> GunPtr;
typedef std::tr1::shared_ptr<Bullet> BulletPtr;
typedef std::tr1::shared_ptr<Bubble> BubblePtr;

class GameField : public GUI::Widget {
 public:
  GameField(const std::string& name, rapidxml::xml_node<>* elem);

  void Draw() override;
  void Update(float dt) override;

  void AcceptMessage(const Message& message) override;

  bool MouseDown(const IPoint& mouse_pos) override;
  void MouseMove(const IPoint& mouse_pos) override;
  void MouseUp(const IPoint& mouse_pos) override;

 private:
  void Init();
  void Deinit();
  void CheckBullets();
  void Shoot(const IPoint &mouse_pos, const int gunStrength);
  bool IsMouseOverField() const;
  int GetNewObjectId();

 private:
  IRect _field;
  EffectsContainer _effCont;

  int _objectNewId;
  GunPtr _gun;
  std::vector<BubblePtr> _bubbles;
  std::vector<BulletPtr> _bullets;
};
