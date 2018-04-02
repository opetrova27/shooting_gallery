#include "stdafx.h"
#include "AppDelegate.h"
#include "ButtonNavigate.h"
#include "ButtonSounds.h"
#include "GameRuntimeInfo.h"
#include "GameResults.h"
#include "GameScreen.h"
#include "GameField.h"
#include "Settings.h"

enum {
  WINDOW_WIDTH = 1024,
  WINDOW_HEIGHT = 768
};

AppDelegate::AppDelegate() {
}

void AppDelegate::OnAppLaunched() {
  Settings::SetInstance(new Settings());
}

void AppDelegate::OnAppTerminated() {
  delete Settings::GetInstance();
  Settings::SetInstance(NULL);
}

void AppDelegate::GameContentSize(int deviceWidth, int deviceHeight, int &width,
                                  int &height) {
  //
  // Здесь задаётся размер игрового экрана в логических единицах.
  //
  // Это значит, что и в оконном, и в полноэкранном режиме размер экрана
  // всегда будет равен WINDOW_WIDTH x WINDOW_HEIGHT "пикселей".
  //
  width = WINDOW_WIDTH;
  height = WINDOW_HEIGHT;
}

void AppDelegate::ScreenMode(DeviceMode &mode) {
  mode = DeviceMode::Windowed;
}

void AppDelegate::RegisterTypes() {
  //
  // Чтобы можно было декларативно использовать виджет в xml по имени,
  // его необходимо зарегистрировать таким образом.
  //
  REGISTER_WIDGET_XML(GameScreen, "GameScreen");
  REGISTER_WIDGET_XML(GameRuntimeInfo, "GameRuntimeInfo");
  REGISTER_WIDGET_XML(GameResults, "GameResults");
  REGISTER_WIDGET_XML(ButtonNavigate, "ButtonNavigate");
  REGISTER_WIDGET_XML(GameField, "GameField");
  REGISTER_WIDGET_XML(ButtonSounds, "ButtonSounds");
}

void AppDelegate::LoadResources() {
  //
  // Обычно в этом методе выполняется скрипт, в котором определяется,
  // какие ресурсы нужно загрузить и какой игровой слой положить на экран.
  //
  Core::LuaExecuteStartupScript("start.lua");
}

void AppDelegate::OnResourceLoaded() {
  if (Core::appInstance->AllResourcesLoaded()) {
    Core::appInstance->SetLoaded();
  }
}

void AppDelegate::OnPostDraw() {
  if (!Render::isFontLoaded("arial"))
  { return; }

  //
  // Перед вызовом метода Render::PrintString() нужно привязать нужный шрифт
  //
  Render::BindFont("arial");

  int dy = Render::getFontHeight();
  int x = Render::device.Width() - 5;
  int y = Render::device.Height() - 20;

  Render::PrintString(x, y,
                      std::string("FPS: ") + utils::lexical_cast(Core::appInstance->GetCurrentFps()),
                      1.0f, RightAlign, BottomAlign);
  Render::PrintString(x, y -= dy,
                      std::string("Video: ") + utils::lexical_cast(Render::device.GetVideoMemUsage() /
                          1024) + std::string("K"), 1.0f, RightAlign, BottomAlign);
  Render::PrintString(x, y -= dy,
                      std::string("Audio: ") + utils::lexical_cast(
                        Core::resourceManager.GetMemoryInUse<MM::AudioResource>() / 1024) +
                      std::string("K"), 1.0f, RightAlign, BottomAlign);
  Render::PrintString(x, y -= dy,
                      std::string("Animations: ") + utils::lexical_cast((
                            Core::resourceManager.GetMemoryInUse<Render::StreamingAnimation>() +
                            Core::resourceManager.GetMemoryInUse<Render::Animation>()) / 1024) +
                      std::string("K"), 1.0f, RightAlign, BottomAlign);
  Render::PrintString(x, y -= dy,
                      std::string("Textures: ") + utils::lexical_cast(
                        Core::resourceManager.GetMemoryInUse<Render::Texture>() / 1024) +
                      std::string("K"), 1.0f, RightAlign, BottomAlign);
  Render::PrintString(x, y -= dy,
                      std::string("Particles: ") + utils::lexical_cast(
                        Core::resourceManager.GetMemoryInUse<ParticleEffect>() / 1024) +
                      std::string("K"), 1.0f, RightAlign, BottomAlign);
  Render::PrintString(x, y -= dy,
                      std::string("Models: ") + utils::lexical_cast(
                        Core::resourceManager.GetMemoryInUse<Render::ModelAnimation>() / 1024) +
                      std::string("K"), 1.0f, RightAlign, BottomAlign);
}
