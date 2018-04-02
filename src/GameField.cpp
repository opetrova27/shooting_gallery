#include "stdafx.h"
#include "Settings.h"
#include "GameField.h"

#define BUBBLE_SIZE 40 /* Размер мишени */
#define BULLET_COUNT_MAX 3 /* Максимальное количество одновременно летящих снарядов на поле*/
#define IS_BETWEEN(x, y, z) ( x >= y && x <= z )

GameField::GameField(const std::string& name, rapidxml::xml_node<>* elem)
  : Widget(name)
  , _field(512, 384, 700, 600)
  , _objectNewId(0) {
  rapidxml::xml_node<>* position = elem->first_node("position");
  if (position) {
    _field.x = atoi(position->first_attribute("x")->value());
    _field.y = atoi(position->first_attribute("y")->value());
  }
  _field.x -= _field.width / 2;
  _field.y -= _field.height / 2;
}

void GameField::Draw() {
  /* Фон игрового поля */
  Render::device.SetTexturing(false);
  Render::BeginColor(Color(128, 128, 128, 200));
  IRect fieldRect = _field;
  fieldRect.x -= static_cast<int>(BUBBLE_SIZE / 2);
  fieldRect.y -= static_cast<int>(BUBBLE_SIZE / 2);
  fieldRect.width += BUBBLE_SIZE;
  fieldRect.height += BUBBLE_SIZE;
  Render::DrawRect(fieldRect);
  Render::EndColor();
  Render::device.SetTexturing(true);
  
  /* Элементы игрового поля */
  for each (BubblePtr bubble in _bubbles) {
    bubble->Draw();
  }
  for each (BulletPtr bullet in _bullets) {
    bullet->Draw();
  }
  _effCont.Draw();
  _gun->Draw();
}

void GameField::Update(float dt) {
  _effCont.Update(dt);
  if (_gun) {
    _gun->Update(dt);
  }
  for each (BubblePtr bubble in _bubbles) {
    bubble->Update(dt);
  }
  for each (BulletPtr bullet in _bullets) {
    bullet->Update(dt);
  }
  CheckBullets();
}

void GameField::AcceptMessage(const Message& message) {
  const std::string& publisher = message.getPublisher();
  const std::string& data = message.getData();
  if (publisher == "Layer") {
    if (data == "LayerDeinit") {
      Deinit();
    } else if (data == "LayerInit") {
      Init();
    }
  }
}

bool GameField::MouseDown(const IPoint &mouse_pos) {
  if (!IsMouseOverField()) {
    return false;
  }
  if (Core::mainInput.GetMouseLeftButton()) {
    return _gun->MouseDown(mouse_pos);
  }
  return false;
}

void GameField::MouseMove(const IPoint &mouse_pos) {
  if (!IsMouseOverField()) {
    return;
  }
  _gun->MouseMove(mouse_pos);
}

void GameField::MouseUp(const IPoint &mouse_pos) {
  if (!IsMouseOverField()) {
    return;
  }
  /* Стреляем, если был MouseDown и пушка набрала дополнительную силу выстрела */
  if (_gun->GetCurrentStrength() > 0) {
    Shoot(mouse_pos, _gun->GetCurrentStrength());
    _gun->MouseUp(mouse_pos);
  }
}

void GameField::Init() {
  const short bubbleCount = Settings::GetInstance()->GetBubblesCount();
  for (short i = 0; i < bubbleCount; ++i) {
    BubblePtr bubble(new Bubble(GetNewObjectId(), _field));
    _bubbles.push_back(bubble);
  }
  /* Максимальное увеличение скорости снаряда - в 3 раза больше указанной в настройках */
  _gun.reset(new Gun(IPoint(_field.x + _field.width / 2, _field.y),
                     Settings::GetInstance()->GetBulletSpeed() * 3));
}

void GameField::Deinit() {
  _bubbles.clear();
  _gun.reset();
  _bullets.clear();
  _effCont.KillAllEffects();
}

bool GameField::IsMouseOverField() const {
  IPoint mouse_pos = Core::mainInput.GetMousePos();
  return IS_BETWEEN(mouse_pos.x, _field.x, _field.x + _field.width) &&
         IS_BETWEEN(mouse_pos.y, _field.y, _field.y + _field.height);
}

int GameField::GetNewObjectId() {
  if (++_objectNewId == INT_MAX) {
    _objectNewId = 0;
  }
  return _objectNewId;
}

void GameField::Shoot(const IPoint &mouse_pos, const int gunStrength) {
  /* Пусть будет снарядов на поле будет не больше указанного количества */
  if (_bullets.size() < BULLET_COUNT_MAX) {
    Log::Info("[GameField] GunStrength = " + std::to_string(gunStrength));
    BulletPtr bullet(new Bullet(GetNewObjectId(),
                                _effCont.AddEffect("bullet_tail"),
                                Settings::GetInstance()->GetBulletSpeed() + gunStrength,
                                IPoint(_field.x + _field.width / 2, _field.y),
                                mouse_pos, _field));
    _bullets.push_back(bullet);
    /*  Сообщаем виджету текущих результатов, что был произведен выстрел */
    Core::guiManager.getLayer("LayerGame")->getWidget("InfoRuntime")->AcceptMessage(
      Message("GameField", "Shoot"));
    const int sample = MM::manager.PlaySample("fireball");
    if (sample == -1) {
      Log::Error("[GameField] Cannot play fireball sound");
    }
  } else {
    /* Иначе пушка перегревается */
    ParticleEffectPtr eff = _effCont.AddEffect("no_bullets");
    eff->SetPos(_gun->GetPos());
    eff->Reset();
    const int sample = MM::manager.PlaySample("empty");
    if (sample == -1) {
      Log::Error("[GameField] Cannot play empty sound");
    }
  }
}

void GameField::CheckBullets() {
  std::vector<int> objectIdToDelete;
  int hittedBubbleCount = 0;
  for each (BulletPtr bullet in _bullets) {
    /* Удаляем попавшие в стенки снаряды */
    if (!bullet->IsInFly()) {
      objectIdToDelete.push_back(bullet->Id());
      continue;
    }
    /* Проверяем пересечение снарядов с мишенями */
    for each (BubblePtr bubble in _bubbles) {
      if (bullet->IsIntersectedWithRect(bubble->GetRect().Inflated(
                                          -1 * BUBBLE_SIZE / 10))) {
        objectIdToDelete.push_back(bubble->Id());
        objectIdToDelete.push_back(bullet->Id());
        ++hittedBubbleCount;
        ParticleEffectPtr eff = _effCont.AddEffect("hit");
        eff->SetPos(bubble->GetPos());
        eff->Reset();
        const int sample = MM::manager.PlaySample("explosion");
        if (sample == -1) {
          Log::Error("[GameField] Cannot play explosion sound");
        }
      }
    }
  }
  for each (int id in objectIdToDelete) {
    auto itbubble = std::find_if(_bubbles.begin(), _bubbles.end(),
    [&id](const BubblePtr & obj) {return obj->Id() == id; });
    if (itbubble != _bubbles.end()) {
      _bubbles.erase(itbubble);
    }
    auto itbullet = std::find_if(_bullets.begin(), _bullets.end(),
    [&id](const BulletPtr & obj) {return obj->Id() == id; });
    if (itbullet != _bullets.end()) {
      _bullets.erase(itbullet);
    }
  }
  /* Сообщаем виджету отображения результатов о сбитых мишенях */
  for (short i = 0; i < hittedBubbleCount; ++i) {
    Core::guiManager.getLayer("LayerGame")->getWidget("InfoRuntime")->AcceptMessage(
      Message("GameField", "BurstBubble"));
  }
}
