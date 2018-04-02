#include "stdafx.h"
#include "Settings.h"
#include "ButtonSounds.h"

#define IS_BETWEEN(x, y, z) ( x >= y && x <= z )

ButtonSounds::ButtonSounds(const std::string& name,
                           rapidxml::xml_node<>* elem)
  : Widget(name)
  , _pos(0, 0) {
  rapidxml::xml_node<>* position = elem->first_node("position");
  if (position) {
    _pos = IPoint(atoi(position->first_attribute("x")->value()),
                  atoi(position->first_attribute("y")->value()));
  }
  Init();
}

void ButtonSounds::Init() {
  _buttonBack = Core::resourceManager.Get<Render::Texture>("button_short");
  _buttonMouseOver =
    Core::resourceManager.Get<Render::Texture>("button_short_mouseover");
  ActualizeIcon();
}

bool ButtonSounds::IsMouseOver() const {
  IPoint mouse_pos = Core::mainInput.GetMousePos();
  const float halfWidth = _buttonBack->Width() / 2;
  const float halfHeight = _buttonBack->Height() / 2;
  return IS_BETWEEN(mouse_pos.x, _pos.x - halfWidth,  _pos.x + halfWidth) &&
         IS_BETWEEN(mouse_pos.y, _pos.y - halfHeight, _pos.y + halfHeight);
}

void ButtonSounds::ActualizeIcon() {
  _buttonIcon =
    Core::resourceManager.Get<Render::Texture>
    (Settings::GetInstance()->GetSoundVolume() > 0 ? "sounds_enabled" :
     "sounds_disabled");
}

void ButtonSounds::Draw() {
  Render::device.PushMatrix();
  Render::device.MatrixTranslate((float)_pos.x - _buttonBack->Width() / 2,
                                 (float)_pos.y - _buttonBack->Height() / 2, 0);
  _buttonBack->Draw();
  if (IsMouseOver()) {
    _buttonMouseOver->Draw();
  }
  Render::device.PopMatrix();
  Render::device.PushMatrix();
  Render::device.MatrixTranslate((float)_pos.x - _buttonIcon->Width() / 2,
                                 (float)_pos.y - _buttonIcon->Height() / 2, 0);
  _buttonIcon->Draw();
  Render::device.PopMatrix();
}

bool ButtonSounds::MouseDown(const IPoint & mouse_pos) {
  if (!IsMouseOver()) {
    return false;
  }

  if (Core::mainInput.GetMouseLeftButton()) {
    const float volume = Settings::GetInstance()->GetSoundVolume();
    Settings::GetInstance()->SetSoundVolume(volume > 0 ? 0.f : 100.f);
    ActualizeIcon();
  }
  return false;
}

void ButtonSounds::AcceptMessage(const Message& message) {
  const std::string& publisher = message.getPublisher();
  const std::string& data = message.getData();
  if (publisher == "Layer" && data == "LayerInit") {
    ActualizeIcon();
  }
}