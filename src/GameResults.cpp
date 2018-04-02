#include "stdafx.h"
#include "Settings.h"
#include "GameResults.h"

#define MINUTE_LENGHT 60
#define WINDOW_WIDTH 1024
#define ELEMENT_HEIGHT 70
#define ELEMENT_WIDTH 2*ELEMENT_HEIGHT

GameResults::GameResults(const std::string& name, rapidxml::xml_node<>* elem)
  : Widget(name)
  , _pos(512, 384)
  , _shootsCount(0)
  , _scoreCurrent(0)
  , _scoreMax(0)
  , _timeCurrent(.0f)
  , _timeStart(.0f) {
  rapidxml::xml_node<>* position = elem->first_node("position");
  if (position) {
    _pos = IPoint(atoi(position->first_attribute("x")->value()),
                  atoi(position->first_attribute("y")->value()));
  }
}

void GameResults::Init(const float time, const int scoreCurrent,
                       const int scoreMax, const int shootsCount) {
  _timeStart = _timeCurrent = time;
  _scoreCurrent = scoreCurrent;
  _scoreMax = scoreMax;
  _shootsCount = shootsCount;
  Log::Info("[GameResults] <" + name + "> Init: time=" + std::to_string(
              _timeCurrent) +
            " scoreCurrent=" + std::to_string(_scoreCurrent) + " scoreMax=" +
            std::to_string(_scoreMax));
}

void GameResults::Deinit() {
  _timeCurrent = .0f;
  _scoreCurrent = _scoreMax = _shootsCount = 0;
}

void GameResults::Draw() {
  /* Результат матча */
  if (!Render::isFontLoaded("courier_large")) {
    Log::Error("[GameResults] courier_large font is not loaded");
    return;
  }
  Render::BindFont("courier_large");
  Render::PrintString(_pos.x, _pos.y + 3 * ELEMENT_HEIGHT,
                      (_scoreCurrent == _scoreMax) ? "WIN" : "FAIL", 1.f, CenterAlign, CenterAlign);

  /* Подробности матча */
  if (!Render::isFontLoaded("courier_small")) {
    Log::Error("[GameResults] courier_small font is not loaded");
    return;
  }
  Render::device.SetTexturing(false);
  Render::BeginColor(Color(128, 128, 128, 128));
  Render::DrawRect(_pos.x - ELEMENT_WIDTH,
                   _pos.y - ELEMENT_HEIGHT,
                   ELEMENT_WIDTH * 2, ELEMENT_HEIGHT * 2);
  Render::EndColor();
  Render::device.SetTexturing(true);

  Render::BindFont("courier_small");
  /* Потраченное время */
  const short seconds = (static_cast<int>(_timeCurrent)) % MINUTE_LENGHT;
  const short minutes = static_cast<int>(_timeCurrent / MINUTE_LENGHT);
  const std::string spentTimeText = "Spent time " +
                                    utils::lexical_cast(
                                      minutes) + ":" +
                                    (seconds < 10 ? "0" : "") + utils::lexical_cast(seconds);
  const IPoint spentTimePos = IPoint(_pos.x,
                                     _pos.y - static_cast<int>(ELEMENT_HEIGHT / 2));
  Render::PrintString(spentTimePos, spentTimeText, 1.f, CenterAlign, CenterAlign);
  /* Точность попадания */
  const std::string accuracyText = "Accuracy " + utils::lexical_cast(
                                     (_shootsCount != 0 ? static_cast<int>(100 * _scoreCurrent /  _shootsCount) : 0))
                                   + " percent";
  Render::PrintString(_pos, accuracyText, 1.f, CenterAlign, CenterAlign);
  /* Убито монстров */
  const std::string bubblesCollectedText = "Monster killed " +
      utils::lexical_cast(
        _scoreCurrent) + "/" + utils::lexical_cast(_scoreMax);
  const IPoint bubblesCollectedPos = IPoint(_pos.x,
                                     _pos.y + static_cast<int>(ELEMENT_HEIGHT / 2));
  Render::PrintString(bubblesCollectedPos, bubblesCollectedText, 1.f, CenterAlign,
                      CenterAlign);
}

void GameResults::AcceptMessage(const Message& message) {
  const std::string& publisher = message.getPublisher();
  const std::string& data = message.getData();
  if (publisher == "Layer") {
    if (data == "LayerDeinit") {
      Deinit();
    }
  } else if (publisher == "GameRuntimeInfo") {
    const int idxFirstDelimeter = data.find_first_of("|");
    const float ftime = std::stof(data.substr(0,
                                  data.length() - idxFirstDelimeter));
    const int idxSecondDelimeter = data.find_first_of("|", idxFirstDelimeter + 1);
    const int iscoreCurrent = std::stoi(data.substr(idxFirstDelimeter + 1,
                                        data.length() - idxSecondDelimeter));
    const int idxThirdDelimeter = data.find_first_of("|", idxSecondDelimeter + 1);
    const int iscoreMax = std::stoi(data.substr(idxSecondDelimeter + 1,
                                    data.length() - idxThirdDelimeter));
    const int ishootsCount = std::stoi(data.substr(idxThirdDelimeter + 1,
                                       data.length()));
    Init(ftime, iscoreCurrent, iscoreMax, ishootsCount);
  }
}
