#ifndef __AppDelegate_H__
#define __AppDelegate_H__

#pragma once

class AppDelegate : public Core::EngineAppDelegate {
 public:
  AppDelegate();

  virtual void OnAppLaunched() override;
  virtual void OnAppTerminated() override;

  virtual void GameContentSize(int deviceWidth, int deviceHeight, int &width,
                               int &height) override;
  virtual void ScreenMode(DeviceMode &mode) override;

  virtual void RegisterTypes() override;

  virtual void LoadResources() override;
  virtual void OnResourceLoaded() override;

  virtual void OnPostDraw() override;
};

#endif // __AppDelegate_H__
