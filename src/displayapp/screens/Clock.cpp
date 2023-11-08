#include "displayapp/screens/Clock.h"

#include <lvgl/lvgl.h>
#include "components/battery/BatteryController.h"
#include "components/motion/MotionController.h"
#include "components/ble/BleController.h"
#include "components/settings/Settings.h"
#include "displayapp/DisplayApp.h"
#include "displayapp/screens/WatchFaceDigital.h"

using namespace Pinetime::Applications::Screens;
using namespace Pinetime::Applications;

Clock::Clock(Controllers::DateTime& dateTimeController,
             const Controllers::Battery& batteryController,
             const Controllers::Ble& bleController,
             Controllers::Settings& settingsController,
             Controllers::HeartRateController& heartRateController,
             Controllers::MotionController& motionController,
             Controllers::FS& filesystem)
  : dateTimeController {dateTimeController},
    batteryController {batteryController},
    bleController {bleController},
    settingsController {settingsController},
    heartRateController {heartRateController},
    motionController {motionController},
    filesystem {filesystem},
    screen {[this, &settingsController]() {
      return WatchFaceDigitalScreen();
    }()} {
  settingsController.SetAppMenu(0);
}

Clock::~Clock() {
  lv_obj_clean(lv_scr_act());
}

bool Clock::OnTouchEvent(Pinetime::Applications::TouchEvents event) {
  return screen->OnTouchEvent(event);
}

bool Clock::OnButtonPushed() {
  return screen->OnButtonPushed();
}

std::unique_ptr<Screen> Clock::WatchFaceDigitalScreen() {
  return std::make_unique<Screens::WatchFaceDigital>(dateTimeController,
                                                     batteryController,
                                                     bleController,
                                                     settingsController,
                                                     heartRateController,
                                                     motionController);
}

