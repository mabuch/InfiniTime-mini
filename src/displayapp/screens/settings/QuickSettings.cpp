#include "displayapp/screens/settings/QuickSettings.h"
#include "displayapp/DisplayApp.h"
#include "displayapp/screens/Symbols.h"
#include "displayapp/screens/BatteryIcon.h"
#include "components/ble/BleController.h"
#include "displayapp/InfiniTimeTheme.h"

using namespace Pinetime::Applications::Screens;

namespace {
  void ButtonEventHandler(lv_obj_t* obj, lv_event_t event) {
    auto* screen = static_cast<QuickSettings*>(obj->user_data);
    if (event == LV_EVENT_CLICKED) {
      screen->OnButtonEvent(obj);
    }
  }

  void lv_update_task(struct _lv_task_t* task) {
    auto* user_data = static_cast<QuickSettings*>(task->user_data);
    user_data->UpdateScreen();
  }
}

QuickSettings::QuickSettings(Pinetime::Applications::DisplayApp* app,
                             const Pinetime::Controllers::Battery& batteryController,
                             Controllers::DateTime& dateTimeController,
                             Controllers::BrightnessController& brightness,
                             Controllers::MotorController& motorController,
                             Pinetime::Controllers::Settings& settingsController,
                             const Controllers::Ble& bleController)
  : app {app},
    dateTimeController {dateTimeController},
    brightness {brightness},
    motorController {motorController},
    settingsController {settingsController},
    statusIcons(batteryController, bleController) {

  statusIcons.Create();

  // This is the distance (padding) between all objects on this screen.
  static constexpr uint8_t innerDistance = 10;

  // Time
  label_time = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_align(label_time, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(label_time, lv_scr_act(), LV_ALIGN_IN_TOP_LEFT, 0, 0);

  static constexpr uint8_t barHeight = 20 + innerDistance;
  static constexpr uint8_t buttonHeight = (LV_VER_RES_MAX - barHeight - innerDistance) / 2;
  static constexpr uint8_t buttonWidth = (LV_HOR_RES_MAX - innerDistance) / 2; // wide buttons
  // static constexpr uint8_t buttonWidth = buttonHeight; // square buttons
  static constexpr uint8_t buttonXOffset = (LV_HOR_RES_MAX - buttonWidth * 2 - innerDistance) / 2;

  lv_style_init(&btn_style);
  lv_style_set_radius(&btn_style, LV_STATE_DEFAULT, buttonHeight / 4);
  lv_style_set_bg_color(&btn_style, LV_STATE_DEFAULT, Colors::bgAlt);

  btn1 = lv_btn_create(lv_scr_act(), nullptr);
  btn1->user_data = this;
  lv_obj_set_event_cb(btn1, ButtonEventHandler);
  lv_obj_add_style(btn1, LV_BTN_PART_MAIN, &btn_style);
  lv_obj_set_size(btn1, buttonWidth, buttonHeight);
  lv_obj_align(btn1, nullptr, LV_ALIGN_IN_TOP_LEFT, buttonXOffset, barHeight);

  btn1_lvl = lv_label_create(btn1, nullptr);
  lv_obj_set_style_local_text_font(btn1_lvl, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_sys_48);
  lv_label_set_text_static(btn1_lvl, brightness.GetIcon());

  btn2 = lv_btn_create(lv_scr_act(), nullptr);
  btn2->user_data = this;
  lv_obj_set_event_cb(btn2, ButtonEventHandler);
  lv_obj_add_style(btn2, LV_BTN_PART_MAIN, &btn_style);
  lv_obj_set_size(btn2, buttonWidth, buttonHeight);
  lv_obj_align(btn2, nullptr, LV_ALIGN_IN_TOP_RIGHT, -buttonXOffset, barHeight);

  btn2_lvl = lv_label_create(btn2, nullptr);
  lv_obj_set_style_local_text_font(btn2_lvl, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_sys_48);
  //lv_label_set_text_static(btn2_lvl, Symbols::bigBluetooth);

  if (settingsController.GetBleRadioEnabled()) {
    lv_label_set_text_static(btn2_lvl, Symbols::bigBluetooth);
  } else {
    lv_label_set_text_static(btn2_lvl, Symbols::bigBluetoothOff);
  }

  btn4 = lv_btn_create(lv_scr_act(), nullptr);
  btn4->user_data = this;
  lv_obj_set_event_cb(btn4, ButtonEventHandler);
  lv_obj_add_style(btn4, LV_BTN_PART_MAIN, &btn_style);
  lv_obj_set_size(btn4, buttonWidth, buttonHeight);
  lv_obj_align(btn4, nullptr, LV_ALIGN_IN_BOTTOM_RIGHT, -buttonXOffset, 0);

  lv_obj_t* lbl_btn;
  lbl_btn = lv_label_create(btn4, nullptr);
  lv_obj_set_style_local_text_font(lbl_btn, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_sys_48);
  lv_label_set_text_static(lbl_btn, Symbols::settings);

  taskUpdate = lv_task_create(lv_update_task, 5000, LV_TASK_PRIO_MID, this);

  UpdateScreen();
}

QuickSettings::~QuickSettings() {
  lv_style_reset(&btn_style);
  lv_task_del(taskUpdate);
  lv_obj_clean(lv_scr_act());
  settingsController.SaveSettings();
}

void QuickSettings::UpdateScreen() {
  lv_label_set_text(label_time, dateTimeController.FormattedTime().c_str());
  statusIcons.Update();
}

void QuickSettings::OnButtonEvent(lv_obj_t* object) {
  if (object == btn2) {
    if (settingsController.GetBleRadioEnabled()) {
      settingsController.SetBleRadioEnabled(false);
      lv_label_set_text_static(btn2_lvl, Symbols::bigBluetoothOff);
    } else {
      settingsController.SetBleRadioEnabled(true);
      lv_label_set_text_static(btn2_lvl, Symbols::bigBluetooth);
    }
    app->PushMessage(Pinetime::Applications::Display::Messages::BleRadioEnableToggle);    
  } else if (object == btn1) {

    brightness.Step();
    lv_label_set_text_static(btn1_lvl, brightness.GetIcon());
    settingsController.SetBrightness(brightness.Level());

  } else if (object == btn4) {
    settingsController.SetSettingsMenu(0);
    app->StartApp(Apps::Settings, DisplayApp::FullRefreshDirections::Up);
  }
}
