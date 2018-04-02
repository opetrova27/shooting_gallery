#pragma once

class GameResults : public GUI::Widget {
 public:
  GameResults(const std::string& name, rapidxml::xml_node<>* elem);

  void Draw() override;
  void AcceptMessage(const Message& message) override;

 private:
  void Init(const float time, const int scoreCurrent, const int scoreMax,
            const int ishootsCount);
  void Deinit();

 private:
  IPoint _pos;
  bool inited;

  /* Отображаемое инфо */
  float _timeCurrent;
  float _timeStart;
  int _scoreCurrent;
  int _scoreMax;
  int _shootsCount;
  std::string _gameResult;
};
