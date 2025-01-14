#pragma once

#include <lvgl/src/lv_core/lv_obj.h>
#include <chrono>
#include <cstdint>
#include <memory>
#include <components/heartrate/HeartRateController.h>
#include "displayapp/screens/Screen.h"
#include "components/datetime/DateTimeController.h"

namespace Pinetime {
  namespace Controllers {
    class Settings;
    class Battery;
    class Ble;
    class MotionController;
  }

  namespace Applications {
    namespace Screens {
      class Clock : public Screen {
      public:
        Clock(Controllers::DateTime& dateTimeController,
              const Controllers::Battery& batteryController,
              const Controllers::Ble& bleController,
              Controllers::Settings& settingsController,
              Controllers::HeartRateController& heartRateController,
              Controllers::MotionController& motionController,
              Controllers::FS& filesystem);
        ~Clock() override;

        bool OnTouchEvent(TouchEvents event) override;
        bool OnButtonPushed() override;

      private:
        Controllers::DateTime& dateTimeController;
        const Controllers::Battery& batteryController;
        const Controllers::Ble& bleController;
        Controllers::Settings& settingsController;
        Controllers::HeartRateController& heartRateController;
        Controllers::MotionController& motionController;
        Controllers::FS& filesystem;

        std::unique_ptr<Screen> screen;
        std::unique_ptr<Screen> WatchFaceDigitalScreen();
      };
    }
  }
}
