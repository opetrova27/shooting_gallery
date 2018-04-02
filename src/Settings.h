#pragma once

class Settings {
 public:
  Settings();
  ~Settings();

  static Settings* GetInstance();
  static void SetInstance(Settings* instance);

  void ReLoad();
  int GetBubblesCount() const;
  int GetBulletSpeed() const;
  int GetGameTime() const;
  float GetSoundVolume() const;
  void SetSoundVolume(const float value);

 private:
  int _countTarget;
  int _bulletSpeed;
  int _gameTime;
  float _soundVolume;
};
