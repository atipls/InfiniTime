#pragma once
#include <cstdint>

namespace Pinetime {
    namespace PinMap {
        static constexpr uint8_t Charging = 12;
        static constexpr uint8_t Cst816sReset = 10;
        static constexpr uint8_t Button = 13;

        static constexpr uint8_t ButtonEnable = 15;
        static constexpr uint8_t Cst816sIrq = 28;
        static constexpr uint8_t PowerPresent = 19;
        static constexpr uint8_t Bma421Irq = 8;

        static constexpr uint8_t Motor = 16;

        static constexpr uint8_t LcdBacklightLow = 14;
        static constexpr uint8_t LcdBacklightMedium = 22;
        static constexpr uint8_t LcdBacklightHigh = 23;

        static constexpr uint8_t SpiSck = 2;
        static constexpr uint8_t SpiMosi = 3;
        static constexpr uint8_t SpiMiso = 4;

        static constexpr uint8_t SpiFlashCsn = 5;
        static constexpr uint8_t SpiLcdCsn = 25;
        static constexpr uint8_t LcdDataCommand = 18;

        static constexpr uint8_t TwiScl = 7;
        static constexpr uint8_t TwiSda = 6;
    }
}
