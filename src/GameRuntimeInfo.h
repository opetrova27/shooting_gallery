#pragma once

class GameRuntimeInfo : public GUI::Widget {
 public:
  GameRuntimeInfo(const std::string& name, rapidxml::xml_node<>* elem);

  void Draw() override;
  void Update(float dt) override;

  void AcceptMessage(const Message& message) override;

 private:
  void Init(const float time, const int scoreCurrent, const int scoreMax,
            const int ishootsCount);
  void Deinit();
  void SendInfo() const;
  void SendGameOver(const std::string& sResult) const;

 private:
  IPoint _pos;
  float _timerWinDelay;
  bool _active;

  /*  Отображаемое актуальное инфо */
  float _timeCurrent;
  float _timeStart;
  int _scoreCurrent;
  int _scoreMax;
  int _shootsCount;
};
