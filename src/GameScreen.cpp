#include "stdafx.h"
#include "GameScreen.h"

GameScreen::GameScreen(const std::string& name, rapidxml::xml_node<>* elem)
  : Widget(name) {
  rapidxml::xml_node<>* background = elem->first_node("background");
  rapidxml::xml_attribute<>* texture = background->first_attribute("name");
  if (texture) {
    _backgroundName = texture->value();
  }

  rapidxml::xml_attribute<>* track = background->first_attribute("track");
  if (track) {
    _trackName = track->value();
  }
}

void GameScreen::Draw() {
  Render::device.PushMatrix();
  Render::device.MatrixTranslate(0, 0, 0);
  _background->Draw();
  Render::device.PopMatrix();
}

void GameScreen::Update(float dt) {
}

void GameScreen::AcceptMessage(const Message& message) {
  const std::string& publisher = message.getPublisher();
  const std::string& data = message.getData();
  if (publisher == "Layer") {
    if (data == "LayerDeinit") {
    } else if (data == "LayerInit") {
      UpdateEnv();
    }
  }
  if (publisher == "GameOver") {
    _trackName = "track_" + std::string(data == "win" ? "victory" : "gameover");
    _backgroundName = "background_" + std::string(data == "win" ? "victory" :
                      "gameover");
    UpdateEnv();
  }
}

void GameScreen::UpdateEnv() {
  _background = Core::resourceManager.Get<Render::Texture>(_backgroundName);
  if (!_trackName.empty()) {
    MM::manager.ChangeTrack(_trackName, .2f);
    Log::Info("[GameScreen] Play <" + _trackName + "> track");
  }
}