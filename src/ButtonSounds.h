#pragma once

class ButtonSounds : public GUI::Widget {
 public:
  ButtonSounds(const std::string& name, rapidxml::xml_node<>* elem);

  void Draw() override;
  bool MouseDown(const IPoint& mouse_pos) override;

  void AcceptMessage(const Message& message) override;

 private:
  void Init();
  bool IsMouseOver() const;
  void ActualizeIcon();

 private:
  Render::Texture* _buttonBack;
  Render::Texture* _buttonMouseOver;
  Render::Texture* _buttonIcon;
  IPoint _pos;
};
