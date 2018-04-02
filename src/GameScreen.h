#pragma once

class GameScreen : public GUI::Widget {
 public:
  GameScreen(const std::string& name, rapidxml::xml_node<>* elem);

  void Draw() override;
  void Update(float dt) override;

  void AcceptMessage(const Message& message) override;

 private:
  void UpdateEnv();

 private:
  Render::Texture* _background;
  std::string _trackName;
  std::string _backgroundName;
};
