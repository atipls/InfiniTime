#pragma once
#include <FreeRTOS.h>
#include <task.h>

namespace Pinetime {
    namespace System {
        class SystemMonitor {
          public:
            void Process();

          private:
            mutable TickType_t lastTick = 0;
        };
    }
}
