#pragma once
#include <cstdint>

namespace Pinetime {
  namespace Applications {
    namespace Display {
      enum class Messages : uint8_t {
        GoToSleep,
        GoToRunning,
        UpdateDateTime,
        UpdateBleConnection,
        TouchEvent,
        ButtonPushed,
        ButtonLongPressed,
        ButtonLongerPressed,
        TimerDone,
        BleFirmwareUpdateStarted,
        DimScreen,
        RestoreBrightness,
        ShowPairingKey,
        AlarmTriggered,
        BleRadioEnableToggle,
        OnChargingEvent,
      };
    }
  }
}
