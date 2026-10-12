#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, ReturnZero, 0x004B2A30, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BB4CC, 0x006C8B80, ());
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(void, __stdcall, NetSession_FormatStationEntry, 0x0073E0A0, (char* buffer, char* stationIndex));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

using GraphBaselines = uint32_t[5];
using GraphScales = float[5];

CSF_GLOBAL(GraphBaselines, g_GraphBaselines, 0x008444B0);
CSF_GLOBAL(GraphScales, g_GraphScales, 0x008445C4);
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_SoundActivityThreshold, 0x0082E114);

static char __fastcall MpGameModes_ApplyGlobalObjectChange(void* self, void* /*edx*/)
{
    // The station formatter writes its status byte at entry offset 490,
    // inside the adjoining memory-text buffer.
    alignas(8) uint8_t frameBytes[0x23C];
    char* stationEntry = reinterpret_cast<char*>(frameBytes);
    char* memoryText = reinterpret_cast<char*>(frameBytes + 0xE8);
    char* text = reinterpret_cast<char*>(frameBytes + 0x1EC);
    uint32_t* startPoint = reinterpret_cast<uint32_t*>(frameBytes + 0x20C);
    uint32_t* endPoint = reinterpret_cast<uint32_t*>(frameBytes + 0x214);
    auto* bytes = reinterpret_cast<uint8_t*>(self);

    if (CSF_AT(char, self, 0xC))
    {
        void* object = CSF_AT(void*, self, 0xFB4);
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[2])(object, nullptr);

        uint8_t* items = bytes + 0xFD0;
        for (int remaining = 5; remaining; --remaining, items += 4)
        {
            object = CSF_AT(void*, items, -0x14);
            reinterpret_cast<void(__fastcall*)(void*, void*)>(
                CSF_AT(void**, object, 0)[2])(object, nullptr);

            object = CSF_AT(void*, items, 0);
            reinterpret_cast<void(__fastcall*)(void*, void*)>(
                CSF_AT(void**, object, 0)[2])(object, nullptr);
        }

        items = bytes + 0x1010;
        for (int remaining = 11; remaining; --remaining, items += 4)
        {
            object = CSF_AT(void*, items, -0x2C);
            reinterpret_cast<void(__fastcall*)(void*, void*)>(
                CSF_AT(void**, object, 0)[2])(object, nullptr);

            object = CSF_AT(void*, items, 0);
            reinterpret_cast<void(__fastcall*)(void*, void*)>(
                CSF_AT(void**, object, 0)[2])(object, nullptr);
        }

        uint8_t* graphItems = bytes + 0x7E4;
        int sampleOffset = 1;
        int x = 62;
        do
        {
            int currentIndex = CSF_AT(int, self, 0x108C) + sampleOffset;
            int previousIndex = currentIndex - 1;
            int nextIndex = currentIndex + 1;
            if (previousIndex >= 100)
                previousIndex -= 100;
            if (currentIndex >= 100)
                currentIndex -= 100;
            if (nextIndex >= 100)
                nextIndex -= 100;

            uint8_t* previous = bytes + 16 + 20 * previousIndex;
            uint8_t* current = bytes + 16 + 20 * currentIndex;
            uint8_t* next = bytes + 16 + 20 * nextIndex;
            auto* color = reinterpret_cast<uint8_t*>(CSF_ADDR_DATA(0x0089A430));
            int series = 0;
            do
            {
                uint32_t nextValue = CSF_AT(uint32_t, next, 0);
                uint32_t currentValue = CSF_AT(uint32_t, current, 0);
                int32_t nextChange = static_cast<int32_t>(nextValue - currentValue);
                int32_t previousChange = static_cast<int32_t>(
                    currentValue - CSF_AT(uint32_t, previous, 0));

                startPoint[0] = x - 6;
                startPoint[1] = g_GraphBaselines[series] -
                    static_cast<uint32_t>(static_cast<int64_t>(
                        static_cast<double>(previousChange) * g_GraphScales[series]));

                object = CSF_AT(void*, graphItems, 0);
                reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t*)>(
                    CSF_AT(void**, object, 0)[7])(object, nullptr, startPoint);

                endPoint[0] = x;
                endPoint[1] = g_GraphBaselines[series] -
                    static_cast<uint32_t>(static_cast<int64_t>(
                        static_cast<double>(nextChange) * g_GraphScales[series]));

                object = CSF_AT(void*, graphItems, 0);
                reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t*)>(
                    CSF_AT(void**, object, 0)[9])(object, nullptr, endPoint);

                object = CSF_AT(void*, graphItems, 0);
                reinterpret_cast<void(__fastcall*)(void*, void*, uint8_t*)>(
                    CSF_AT(void**, object, 0)[11])(object, nullptr, color);

                object = CSF_AT(void*, graphItems, 0);
                reinterpret_cast<void(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, object, 0)[2])(object, nullptr);

                next += 4;
                current += 4;
                previous += 4;
                color += 16;
                ++series;
                graphItems += 4;
            }
            while (reinterpret_cast<intptr_t>(color) <
                   static_cast<intptr_t>(CSF_ADDR_DATA(0x0089A480)));

            ++sampleOffset;
            x += 6;
        }
        while (x < 650);

        items = bytes + 0x1064;
        for (int series = 0; series < 5; ++series, items += 4)
        {
            object = CSF_AT(void*, items, -0x28);
            reinterpret_cast<void(__fastcall*)(void*, void*)>(
                CSF_AT(void**, object, 0)[2])(object, nullptr);

            int index = CSF_AT(int, self, 0x108C);
            int latestIndex = index - 1;
            if (latestIndex < 0)
                latestIndex += 100;
            int olderIndex = index - 6;
            if (olderIndex < 0)
                olderIndex += 100;

            uint8_t* total = bytes + 16 + 4 * (5 * latestIndex + series);
            _sprintf(text, CSF_STR(0x0084461C, "Tot: %d"),
                     CSF_AT(uint32_t, total, 0));

            object = CSF_AT(void*, items, 0);
            reinterpret_cast<void(__fastcall*)(void*, void*, char*)>(
                CSF_AT(void**, object, 0)[15])(object, nullptr, text);
            object = CSF_AT(void*, items, 0);
            reinterpret_cast<void(__fastcall*)(void*, void*)>(
                CSF_AT(void**, object, 0)[2])(object, nullptr);

            uint32_t average = CSF_AT(uint32_t, total, 0) /
                               CSF_AT(uint32_t, self, 0x1090);
            _sprintf(text, CSF_STR(0x00844614, "Med: %d"), average);

            object = CSF_AT(void*, items, 0x14);
            reinterpret_cast<void(__fastcall*)(void*, void*, char*)>(
                CSF_AT(void**, object, 0)[15])(object, nullptr, text);
            object = CSF_AT(void*, items, 0x14);
            reinterpret_cast<void(__fastcall*)(void*, void*)>(
                CSF_AT(void**, object, 0)[2])(object, nullptr);

            uint32_t change = CSF_AT(uint32_t, total, 0) -
                CSF_AT(uint32_t, bytes + 16 + 4 * (5 * olderIndex + series), 0);
            // Natural unsigned conversion replaces the original bias idiom.
            uint32_t rate = static_cast<uint32_t>(static_cast<int64_t>(
                static_cast<double>(change) * g_SoundActivityThreshold));
            _sprintf(text, CSF_STR(0x0084460C, "M5s: %d"), rate);

            object = CSF_AT(void*, items, -0x14);
            reinterpret_cast<void(__fastcall*)(void*, void*, char*)>(
                CSF_AT(void**, object, 0)[15])(object, nullptr, text);
            object = CSF_AT(void*, items, -0x14);
            reinterpret_cast<void(__fastcall*)(void*, void*)>(
                CSF_AT(void**, object, 0)[2])(object, nullptr);
        }
    }

    if (CSF_AT(char, self, 0xD))
    {
        uint32_t stationIndex = 0;
        bool finished = false;
        do
        {
            stationEntry[490] = 0;
            stationEntry[180] = 0;
            stationEntry[190] = 0;
            stationEntry[170] = 0;
            stationEntry[160] = 0;
            stationEntry[150] = 0;
            stationEntry[100] = 0;
            stationEntry[50] = 0;
            stationEntry[0] = 0;

            GetGlobalDword_73C020();
            NetSession_FormatStationEntry(
                stationEntry, reinterpret_cast<char*>(stationIndex));
            if (stationEntry[490])
                GetGlobalValue_8BB4CC();
            else
                finished = true;
            ++stationIndex;
        }
        while (!finished);
    }

    char result = CSF_AT(char, self, 0xE);
    if (result)
    {
        GetGlobalDword_73C020();
        int usedMemory = ReturnZero();
        _sprintf(memoryText, CSF_STR(0x008445F8, "Memoria usada: %d"), usedMemory);
        return static_cast<char>(GetGlobalValue_8BB4CC());
    }
    return result;
}

HOOK(0x00550150, MpGameModes_ApplyGlobalObjectChange, "auto");
