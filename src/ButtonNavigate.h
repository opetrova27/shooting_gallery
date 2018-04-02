#pragma once

class ButtonNavigate : public GUI::Widget {
 public:
  ButtonNavigate(const std::string& name, rapidxml::xml_node<>* elem);

  void Draw() override;
  void AcceptMessage(const Message& message) override;
  bool MouseDown(const IPoint& mouse_pos) override;

 private:
  void Init();
  bool IsMouseOver() const;
  void ChangeLayer();

 private:
  Render::Texture* _buttonBack;
  Render::Texture* _buttonMouseOver;

  IPoint _pos;
  std::string _text;
  std::string _layer;
};
