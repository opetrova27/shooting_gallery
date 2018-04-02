#include "stdafx.h"
#include "ButtonNavigate.h"

#define IS_BETWEEN(x, y, z) ( x >= y && x <= z )

ButtonNavigate::ButtonNavigate(const std::string& name,
                               rapidxml::xml_node<>* elem)
  : Widget(name)
  , _pos(0, 0)
  , _layer("")
  , _text("UNKNOWN") {
  _text = elem->first_attribute("text")->value();
  _layer = elem->first_attribute("layer")->value();
  rapidxml::xml_node<>* position = elem->first_node("position");
  if (position) {
    _pos = IPoint(atoi(position->first_attribute("x")->value()),
                  atoi(position->first_attribute("y")->value()));
  }
  Init();
}

void ButtonNavigate::Init() {
  _buttonBack = Core::resourceManager.Get<Render::Texture>("button_long");
  _buttonMouseOver =
    Core::resourceManager.Get<Render::Texture>("button_long_mouseover");
}

bool ButtonNavigate::IsMouseOver() const {
  IPoint mouse_pos = Core::mainInput.GetMousePos();
  const float halfWidth = _buttonBack->Width() / 2;
  const float halfHeight = _buttonBack->Height() / 2;
  return IS_BETWEEN(mouse_pos.x, _pos.x - halfWidth,  _pos.x + halfWidth) &&
         IS_BETWEEN(mouse_pos.y, _pos.y - halfHeight, _pos.y + halfHeight);
}

void ButtonNavigate::Draw() {
  Render::device.PushMatrix();
  Render::device.MatrixTranslate((float)_pos.x - _buttonBack->Width() / 2,
                                 (float)_pos.y - _buttonBack->Height() / 2, 0);
  _buttonBack->Draw();
  if (IsMouseOver()) {
    _buttonMouseOver->Draw();
  }
  Render::device.PopMatrix();

  if (Render::isFontLoaded("arial_blue")) {
    Render::BindFont("arial_blue");
    Render::PrintString(_pos, _text, 1.0f, CenterAlign, CenterAlign);
  } else {
    Log::Error("[ButtonNavigate] <" + name + "> Nice is not loaded");
  }
}

bool ButtonNavigate::MouseDown(const IPoint &mouse_pos) {
  //TODO: check click by texture
  if (!IsMouseOver()) {
    return false;
  }

  if (Core::mainInput.GetMouseLeftButton()) {
    ChangeLayer();
  }
  return false;
}

void ButtonNavigate::AcceptMessage(const Message& message) {
  const std::string& publisher = message.getPublisher();
  const std::string& data = message.getData();
  if (publisher == "GameOver") {
    ChangeLayer();
  }
}

void ButtonNavigate::ChangeLayer() {
  Core::guiManager.getLayer(_layer)->AcceptMessage(Message("ChangeLayer",
      _layer));
}
