#include <lvgl/lvgl.h>
#include "displayapp/screens/WatchFaceWin95.h"
#include "displayapp/screens/BatteryIcon.h"
#include "displayapp/screens/NotificationIcon.h"
#include "displayapp/screens/Symbols.h"
#include "components/battery/BatteryController.h"
#include "components/ble/BleController.h"
#include "components/ble/NotificationManager.h"
#include "components/heartrate/HeartRateController.h"
#include "components/motion/MotionController.h"
#include "components/settings/Settings.h"

#include "displayapp/icons/win95/gradient.c"

using namespace Pinetime::Applications::Screens;

WatchFaceWin95::WatchFaceWin95(Controllers::DateTime& dateTimeController, const Controllers::Battery& batteryController,
                               const Controllers::Ble& bleController, Controllers::NotificationManager& notificationManager,
                               Controllers::Settings& settingsController, Controllers::HeartRateController& heartRateController,
                               Controllers::MotionController& motionController, Controllers::FS& filesystem)
  : currentDateTime {{}},
    dateTimeController {dateTimeController},
    batteryController {batteryController},
    bleController {bleController},
    notificationManager {notificationManager},
    settingsController {settingsController},
    heartRateController {heartRateController},
    motionController {motionController} {

    lfs_file f = {};
    if (filesystem.FileOpen(&f, "/fonts/lv_font_win95_clock.bin", LFS_O_RDONLY) >= 0) {
        filesystem.FileClose(&f);
        font_win95_clock = lv_font_load("F:/fonts/lv_font_win95_clock.bin");
    } else
        return;

    if (filesystem.FileOpen(&f, "/fonts/lv_font_win95_normal.bin", LFS_O_RDONLY) >= 0) {
        filesystem.FileClose(&f);
        font_win95_normal = lv_font_load("F:/fonts/lv_font_win95_normal.bin");
    } else
        return;

    if (filesystem.FileOpen(&f, "/fonts/lv_font_win95_small.bin", LFS_O_RDONLY) >= 0) {
        filesystem.FileClose(&f);
        font_win95_small = lv_font_load("F:/fonts/lv_font_win95_small.bin");
    } else
        return;

    lv_obj_t* background = lv_obj_create(lv_scr_act(), nullptr);
    lv_obj_set_style_local_bg_color(background, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(184, 184, 184));
    lv_obj_set_style_local_radius(background, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_size(background, 240, 240);
    lv_obj_align(background, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);

    image_gradient = lv_img_create(lv_scr_act(), nullptr);
    lv_img_set_src(image_gradient, &win95gradient);
    lv_obj_align(image_gradient, lv_scr_act(), LV_ALIGN_IN_TOP_MID, 0, 2);

    label_time = lv_label_create(lv_scr_act(), nullptr);
    lv_obj_align(label_time, lv_scr_act(), LV_ALIGN_CENTER, 0, -60);
    lv_obj_set_style_local_text_font(label_time, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, font_win95_clock);
    lv_obj_set_style_local_text_color(label_time, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    label_date = lv_label_create(lv_scr_act(), nullptr);
    lv_obj_align(label_date, lv_scr_act(), LV_ALIGN_CENTER, 0, -10);
    lv_obj_set_style_local_text_font(label_date, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, font_win95_normal);
    lv_obj_set_style_local_text_color(label_date, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    label_battery = lv_label_create(lv_scr_act(), nullptr);
    lv_obj_align(label_battery, lv_scr_act(), LV_ALIGN_IN_BOTTOM_LEFT, 10, -10);
    lv_obj_set_style_local_text_font(label_battery, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, font_win95_small);
    lv_obj_set_style_local_text_color(label_battery, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    label_steps = lv_label_create(lv_scr_act(), nullptr);
    lv_obj_align(label_steps, lv_scr_act(), LV_ALIGN_IN_BOTTOM_RIGHT, -10, -10);
    lv_obj_set_style_local_text_font(label_steps, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, font_win95_small);
    lv_obj_set_style_local_text_color(label_steps, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    label_heartrate = lv_label_create(lv_scr_act(), nullptr);
    lv_obj_align(label_heartrate, lv_scr_act(), LV_ALIGN_IN_BOTTOM_MID, 15, -10);
    lv_obj_set_style_local_text_font(label_heartrate, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, font_win95_small);
    lv_obj_set_style_local_text_color(label_heartrate, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    label_notification = lv_label_create(lv_scr_act(), nullptr);
    lv_obj_align(label_notification, lv_scr_act(), LV_ALIGN_IN_BOTTOM_LEFT, 70, -10);
    lv_obj_set_style_local_text_font(label_notification, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, font_win95_small);
    lv_obj_set_style_local_text_color(label_notification, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    label_bluetooth = lv_label_create(lv_scr_act(), nullptr);
    lv_obj_align(label_bluetooth, lv_scr_act(), LV_ALIGN_IN_BOTTOM_LEFT, 50, -10);
    lv_obj_set_style_local_text_font(label_bluetooth, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, font_win95_small);
    lv_obj_set_style_local_text_color(label_bluetooth, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    taskRefresh = lv_task_create(RefreshTaskCallback, LV_DISP_DEF_REFR_PERIOD, LV_TASK_PRIO_MID, this);
    Refresh();
}

WatchFaceWin95::~WatchFaceWin95() {
    lv_task_del(taskRefresh);

    if (font_win95_clock) lv_font_free(font_win95_clock);
    if (font_win95_normal) lv_font_free(font_win95_normal);
    if (font_win95_small) lv_font_free(font_win95_small);

    font_win95_clock = nullptr;
    font_win95_normal = nullptr;
    font_win95_small = nullptr;

    lv_obj_clean(lv_scr_act());
}

void WatchFaceWin95::Refresh() {

    currentDateTime = dateTimeController.CurrentDateTime();

    if (currentDateTime.IsUpdated()) {
        auto hour = dateTimeController.Hours();
        auto minute = dateTimeController.Minutes();
        auto second = dateTimeController.Seconds();
        auto year = dateTimeController.Year();
        auto month = dateTimeController.Month();
        auto dayOfWeek = dateTimeController.DayOfWeek();
        auto day = dateTimeController.Day();

        if (displayedHour != hour || displayedMinute != minute || displayedSecond != second) {
            displayedHour = hour;
            displayedMinute = minute;
            displayedSecond = second;

            if (settingsController.GetClockType() == Controllers::Settings::ClockType::H12) {
                char ampmChar[3] = "AM";
                if (hour == 0) {
                    hour = 12;
                } else if (hour == 12) {
                    ampmChar[0] = 'P';
                } else if (hour > 12) {
                    hour = hour - 12;
                    ampmChar[0] = 'P';
                }
                lv_label_set_text_fmt(label_time, "%02d:%02d:%02d %s", hour, minute, second, ampmChar);
            } else {
                lv_label_set_text_fmt(label_time, "%02d:%02d:%02d", hour, minute, second);
            }

            lv_obj_realign(label_time);
        }

        if ((year != currentYear) || (month != currentMonth) || (dayOfWeek != currentDayOfWeek) || (day != currentDay)) {
            lv_label_set_text_fmt(label_date, "%04d. %02d. %02d", short(year), char(month), char(day));
            lv_obj_realign(label_date);

            currentYear = year;
            currentMonth = month;
            currentDayOfWeek = dayOfWeek;
            currentDay = day;
        }
    }

    powerPresent = batteryController.IsPowerPresent();
    batteryPercentRemaining = batteryController.PercentRemaining();
    if (batteryPercentRemaining.IsUpdated() || powerPresent.IsUpdated()) {
        lv_label_set_text_fmt(label_battery, "%d%%", batteryPercentRemaining.Get());
        if (batteryController.IsPowerPresent()) {
            lv_label_ins_text(label_battery, LV_LABEL_POS_LAST, " ++");
        }

        lv_obj_realign(label_battery);
    }

    stepCount = motionController.NbSteps();
    motionSensorOk = motionController.IsSensorOk();
    if (stepCount.IsUpdated() || motionSensorOk.IsUpdated()) {
        lv_label_set_text_fmt(label_steps, "%d steps", stepCount.Get());
        lv_obj_realign(label_steps);
    }

    heartbeat = heartRateController.HeartRate();
    heartbeatRunning = heartRateController.State() != Controllers::HeartRateController::States::Stopped;
    if (heartbeat.IsUpdated() || heartbeatRunning.IsUpdated()) {
        if (heartbeatRunning.Get()) {
            lv_label_set_text_fmt(label_heartrate, "%d bpm", heartbeat.Get());
        } else {
            lv_label_set_text_static(label_heartrate, "");
        }

        lv_obj_realign(label_heartrate);
    }

    notificationState = notificationManager.AreNewNotificationsAvailable();
    if (notificationState.IsUpdated()) {
        if (notificationState.Get()) {
            lv_label_set_text_fmt(label_notification, "%d msgs", notificationManager.NbNotifications());
        } else {
            lv_label_set_text_static(label_notification, "");
        }

        lv_obj_realign(label_notification);
    }

    bleState = bleController.IsConnected();
    bleRadioEnabled = bleController.IsRadioEnabled();
    if (bleState.IsUpdated() || bleRadioEnabled.IsUpdated()) {
        if (!bleRadioEnabled.Get()) {
            lv_label_set_text_static(label_bluetooth, "");
        } else {
            lv_label_set_text_static(label_bluetooth, "BT");

            lv_obj_set_style_local_text_color(label_bluetooth,
                                              LV_LABEL_PART_MAIN,
                                              LV_STATE_DEFAULT,
                                              bleState.Get() ? LV_COLOR_BLUE : LV_COLOR_BLACK);
        }

        lv_obj_realign(label_bluetooth);
    }
}

bool WatchFaceWin95::IsAvailable(Pinetime::Controllers::FS& filesystem) {
    lfs_file file = {};

    if (filesystem.FileOpen(&file, "/fonts/lv_font_win95_clock.bin", LFS_O_RDONLY) < 0) {
        return false;
    }
    filesystem.FileClose(&file);

    if (filesystem.FileOpen(&file, "/fonts/lv_font_win95_normal.bin", LFS_O_RDONLY) < 0) {
        return false;
    }

    filesystem.FileClose(&file);
    return true;
}
