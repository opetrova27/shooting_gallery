#include "stdafx.h"
#include "Settings.h"
#include "GameRuntimeInfo.h"

#define MINUTE_LENGHT 60
#define WINDOW_WIDTH 1024
#define ELEMENT_HEIGHT 70
#define ELEMENT_WIDTH 2*ELEMENT_HEIGHT

GameRuntimeInfo::GameRuntimeInfo(const std::string& name,
                                 rapidxml::xml_node<>* elem)
  : Widget(name)
  , _pos(512, 384)
  , _shootsCount(0)
  , _scoreCurrent(0)
  , _scoreMax(0)
  , _timerWinDelay(.0f)
  , _timeCurrent(.0f)
  , _timeStart(.0f)
  , _active(false) {
  rapidxml::xml_node<>* position = elem->first_node("position");
  if (position) {
    _pos = IPoint(atoi(position->first_attribute("x")->value()),
                  atoi(position->first_attribute("y")->value()));
  }
}

void GameRuntimeInfo::Init(const float time, const int scoreCurrent,
                           const int scoreMax, const int shootsCount) {
  _timeStart = _timeCurrent = Settings::GetInstance()->GetGameTime();
  _scoreMax = Settings::GetInstance()->GetBubblesCount();
  _scoreCurrent = _shootsCount = 0;
  Log::Info("[GameRuntimeInfo] Init: time=" + std::to_string( _timeCurrent) +
            " scoreCurrent=" + std::to_string(_scoreCurrent) + " scoreMax=" +
            std::to_string(_scoreMax));
  _active = true;
}

void GameRuntimeInfo::Deinit() {
  _active = false;
  _timeCurrent = _timerWinDelay = .0f;
  _scoreCurrent = _scoreMax = _shootsCount = 0;
}

void GameRuntimeInfo::Draw() {
  if (!Render::isFontLoaded("courier_medium")) {
    Log::Error("[GameRuntimeInfo] courier_medium font is not loaded");
    return;
  }
  Render::BindFont("courier_medium");
  const short seconds = (static_cast<int>(_timeCurrent)) % MINUTE_LENGHT;
  const short minutes = static_cast<int>(_timeCurrent / MINUTE_LENGHT);
  Render::PrintString(_pos.x - ELEMENT_WIDTH,
                      _pos.y - static_cast<int>(ELEMENT_HEIGHT / 2),
                      utils::lexical_cast(minutes) + ":" +
                      (seconds < 10 ? "0" : "") +
                      utils::lexical_cast(seconds), 1.f, CenterAlign, CenterAlign);
  Render::PrintString(_pos.x + ELEMENT_WIDTH,
                      _pos.y - static_cast<int>(ELEMENT_HEIGHT / 2),
                      utils::lexical_cast(_scoreCurrent) + "/" + utils::lexical_cast(_scoreMax), 1.f,
                      CenterAlign, CenterAlign);

  if (!Render::isFontLoaded("courier_small")) {
    Log::Error("[GameRuntimeInfo] courier_small font is not loaded");
    return;
  }

  Render::device.SetTexturing(false);
  Render::BeginColor(Color(128, 128, 128, 128));
  Render::DrawRect(WINDOW_WIDTH - ELEMENT_WIDTH, 0, ELEMENT_WIDTH,
                   ELEMENT_HEIGHT);
  Render::EndColor();
  Render::device.SetTexturing(true);

  Render::BindFont("courier_small");
  IPoint mouse_pos = Core::mainInput.GetMousePos();
  Render::PrintString(WINDOW_WIDTH - ELEMENT_WIDTH / 2, ELEMENT_HEIGHT / 2,
                      utils::lexical_cast(mouse_pos.x) + ", " + utils::lexical_cast(mouse_pos.y), 1.f,
                      CenterAlign, CenterAlign);
}

void GameRuntimeInfo::Update(float dt) {
  /* Ожидаем некоторое время после выигрыша, чтобы досмотреть последний эффект попадания */
  if (_timerWinDelay > .0f) {
    _timerWinDelay -= dt;
  } else if (_timerWinDelay < .0f) {
    _timerWinDelay = .0f;
    SendGameOver("win");
  }
  if (!_active) {
    return;
  }
  if (_scoreCurrent == _scoreMax) {
    /* Сбили все мишени */
    _active = false;
    _timerWinDelay = 1.0f;
    return;
  }
  _timeCurrent -= dt;
  if (_timeCurrent < 0) {
    /* Время истекло */
    _active = false;
    SendGameOver("lose");
    return;
  }
}

void GameRuntimeInfo::AcceptMessage(const Message& message) {
  const std::string& publisher = message.getPublisher();
  const std::string& data = message.getData();
  if (publisher == "Layer") {
    if (data == "LayerDeinit") {
      SendInfo();
      Deinit();
    } else if (data == "LayerInit") {
      Init(.0f, 0, 0, 0);
    }
  } else if (publisher == "GameField" && data == "BurstBubble") {
    ++_scoreCurrent;
  } else if (publisher == "GameField" && data == "Shoot") {
    ++_shootsCount;
  }
}

void GameRuntimeInfo::SendInfo() const {
  const std::string info = std::to_string(_timeStart - _timeCurrent) + "|" +
                           std::to_string(_scoreCurrent) + "|" +
                           std::to_string(_scoreMax) + "|" +
                           std::to_string(_shootsCount);
  /* Сообщаем подробности игры виджету отображения результирующей информации */
  Core::guiManager.getLayer("LayerResult")->getWidget("InfoResult")->AcceptMessage(
    Message("GameRuntimeInfo", info));
  /* Сообщаем итоги игры экрану отображения результатов */
  Core::guiManager.getLayer("LayerResult")->getWidget("ResultScreen")->AcceptMessage(
    Message("GameOverResult", _scoreCurrent == _scoreMax ? "win" : "fail"));
}

void GameRuntimeInfo::SendGameOver(const std::string& sResult) const {
  /* Сообщаем итоги игры экрану отображения результатов */
  Core::guiManager.getLayer("LayerGame")->getWidget("ButtonStop")->AcceptMessage(
    Message("GameOver", sResult));
}
