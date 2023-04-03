#pragma once

#include <lvgl/src/lv_core/lv_obj.h>
#include <chrono>
#include <cstdint>
#include <memory>
#include "displayapp/screens/Screen.h"
#include "components/datetime/DateTimeController.h"
#include "utility/DirtyValue.h"

namespace Pinetime {
    namespace Controllers {
        class Settings;
        class Battery;
        class Ble;
        class NotificationManager;
        class HeartRateController;
        class MotionController;
    }

    namespace Applications {
        namespace Screens {

            class WatchFaceWin95 : public Screen {
              public:
                WatchFaceWin95(Controllers::DateTime& dateTimeController, const Controllers::Battery& batteryController,
                               const Controllers::Ble& bleController, Controllers::NotificationManager& notificationManager,
                               Controllers::Settings& settingsController, Controllers::HeartRateController& heartRateController,
                               Controllers::MotionController& motionController, Controllers::FS& filesystem);
                ~WatchFaceWin95() override;

                void Refresh() override;

                static bool IsAvailable(Pinetime::Controllers::FS& filesystem);

              private:
                uint8_t displayedHour = -1;
                uint8_t displayedMinute = -1;
                uint8_t displayedSecond = -1;

                uint16_t currentYear = 1970;
                Pinetime::Controllers::DateTime::Months currentMonth = Pinetime::Controllers::DateTime::Months::Unknown;
                Pinetime::Controllers::DateTime::Days currentDayOfWeek = Pinetime::Controllers::DateTime::Days::Unknown;
                uint8_t currentDay = 0;

                Utility::DirtyValue<int> batteryPercentRemaining {};
                Utility::DirtyValue<bool> powerPresent {};
                Utility::DirtyValue<bool> bleState {};
                Utility::DirtyValue<bool> bleRadioEnabled {};
                Utility::DirtyValue<std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>> currentDateTime {};
                Utility::DirtyValue<bool> motionSensorOk {};
                Utility::DirtyValue<uint32_t> stepCount {};
                Utility::DirtyValue<uint8_t> heartbeat {};
                Utility::DirtyValue<bool> heartbeatRunning {};
                Utility::DirtyValue<bool> notificationState {};

                lv_font_t* font_win95_clock {};
                lv_font_t* font_win95_normal {};
                lv_font_t* font_win95_small {};

                lv_obj_t* image_gradient {};

                lv_obj_t* label_date {};
                lv_obj_t* label_time {};

                lv_obj_t* label_battery {};
                lv_obj_t* label_steps {};
                lv_obj_t* label_heartrate {};
                lv_obj_t* label_notification {};
                lv_obj_t* label_bluetooth {};

                Controllers::DateTime& dateTimeController;
                const Controllers::Battery& batteryController;
                const Controllers::Ble& bleController;
                Controllers::NotificationManager& notificationManager;
                Controllers::Settings& settingsController;
                Controllers::HeartRateController& heartRateController;
                Controllers::MotionController& motionController;

                lv_task_t* taskRefresh;
            };
        }
    }
}