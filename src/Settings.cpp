#include "stdafx.h"
#include "Settings.h"

#define FILENAME "input.txt"

Settings* _instance = NULL;

Settings::Settings()
  : _countTarget(0)
  , _bulletSpeed(0)
  , _gameTime(0) {
  Log::Info("[Settings] created");
  ReLoad();
  _soundVolume = MM::manager.GetMusicVolume();
  SetSoundVolume(_soundVolume);
}

Settings::~Settings() {
  Log::Info("[Settings] destroyed");
}

Settings* Settings::GetInstance() {
  return _instance;
}

void Settings::SetInstance(Settings* instance) {
  _instance = instance;
}

void Settings::ReLoad() {
  IO::InputStreamPtr stream = Core::fileSystem.OpenRead(FILENAME);
  std::vector<uint8_t> data;
  bool res = stream->ReadAllBytes(data);
  if (!res) {
    Log::Error("[Settings] <" + std::string(FILENAME) + ">  not found");
    return;
  }
  const std::string settings(data.begin(), data.end());

  const size_t idxStartCount = settings.find("CountTarget=") + 12;
  const size_t idxEndCount = settings.find("\r\n", idxStartCount);
  _countTarget = std::stoi(settings.substr(idxStartCount,
                           settings.length() - idxEndCount));

  const size_t idxStartSpeed = settings.find("Speed=", idxStartCount) + 6;
  const size_t idxEndSpeed = settings.find("\r\n", idxStartSpeed);
  _bulletSpeed = std::stof(settings.substr(idxStartSpeed,
                           settings.length() - idxEndSpeed));

  const size_t idxStartTime = settings.find("Time=", idxStartSpeed) + 5;
  const size_t idxEndTime = settings.find("\r\n", idxStartTime);
  _gameTime = std::stof(settings.substr(idxStartTime,
                                        settings.length() - idxEndTime));
}

int Settings::GetBubblesCount() const {
  return _countTarget;
}

int Settings::GetBulletSpeed() const {
  return _bulletSpeed;
}

int Settings::GetGameTime() const {
  return _gameTime;
}

float Settings::GetSoundVolume() const {
  return _soundVolume;
}

void Settings::SetSoundVolume(const float value) {
  _soundVolume = value;
  MM::manager.SetSoundVolume(_soundVolume);
  MM::manager.SetMusicVolume(_soundVolume);
}