#pragma once

#include <array>
#include <cstdint>
#include <lvgl/lvgl.h>

#include "displayapp/screens/ScreenList.h"
#include "components/settings/Settings.h"
#include "displayapp/screens/Screen.h"
#include "displayapp/screens/Symbols.h"
#include "displayapp/screens/CheckboxList.h"
#include "displayapp/screens/WatchFaceWin95.h"

namespace Pinetime {

  namespace Applications {
    namespace Screens {

      class SettingWatchFace : public Screen {
      public:
        SettingWatchFace(DisplayApp* app, Pinetime::Controllers::Settings& settingsController, Pinetime::Controllers::FS& filesystem);
        ~SettingWatchFace() override;

        bool OnTouchEvent(TouchEvents event) override;

      private:
        DisplayApp* app;
        auto CreateScreenList() const;
        std::unique_ptr<Screen> CreateScreen(unsigned int screenNum) const;

        Controllers::Settings& settingsController;
        Pinetime::Controllers::FS& filesystem;

        static constexpr const char* title = "Watch face";
        static constexpr const char* symbol = Symbols::home;

        static constexpr int settingsPerScreen = 4;

        // Increment this when more space is needed
        static constexpr int nScreens = 1;

        std::array<Screens::CheckboxList::Item, settingsPerScreen * nScreens> watchfaces {{
           {"Digital face", true},
           {"Terminal", true},
           {"Windows 95", Applications::Screens::WatchFaceWin95::IsAvailable(filesystem)},
           {"", false},
        }};
        ScreenList<nScreens> screens;
      };
    }
  }
}
