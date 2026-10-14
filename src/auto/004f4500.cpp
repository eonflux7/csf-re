#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, HudStringShow_NotifyChildSlotsAndTail, 0x004F4010,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, HudStringShow_CheckRectangle, 0x004F41A0,
         (void* self, void* edx, uint32_t* a2));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, Text_ConvertToDisplayEncoding, 0x00692580,
         (int text, int flag));
CSF_FUNC(int, __cdecl, MenuMessageBox_TestEntryAgainstFlag, 0x00695500,
         (uint16_t* entry, char* flagByte));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

CSF_GLOBAL(float, g_ProgressScale, 0x0082E03C);
CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static char __fastcall HudStringShow_UpdateDisplay(void* self, void* /*edx*/)
{
    int result = CSF_AT(int, self, 0x24);
    if (result ||
        (result = CSF_AT(int, self, 0x28)) ||
        (result = CSF_AT(int, self, 0x2C)) ||
        (result = CSF_AT(int, self, 0x30))) {
        result = CSF_AT(uint8_t, self, 0x34);
        if (result)
            result = HudStringShow_NotifyChildSlotsAndTail(self, nullptr);
    }

    if (CSF_AT(int, self, 0x48) == -1082130432)
        return static_cast<char>(result);

    if (!CSF_AT(int, self, 0x24) &&
        !(result = CSF_AT(int, self, 0x28)) &&
        !(result = CSF_AT(int, self, 0x2C)) &&
        !(result = CSF_AT(int, self, 0x30))) {
        result = CSF_AT(uint8_t, self, 0x35);
        if (result)
            return static_cast<char>(result);
    }

    result = CSF_AT(int, self, 0x20);
    if (!result)
        return static_cast<char>(result);

    const int text = CSF_AT(uint32_t, self, 0x1C) < 0x10u
        ? reinterpret_cast<int>(static_cast<uint8_t*>(self) + 8)
        : CSF_AT(int, self, 8);
    const int displayText = Text_ConvertToDisplayEncoding(text, 12);
    if (!displayText)
        return static_cast<char>(displayText);

    result = MenuMessageBox_TestEntryAgainstFlag(
        reinterpret_cast<uint16_t*>(displayText),
        reinterpret_cast<char*>(&g_byte_82E65C));
    if (!result)
        return static_cast<char>(result);

    void* display = CSF_AT(void*, self, 0x20);
    // 004F45B3 pushes only the display string; the earlier push ebx saves ebx.
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, display, 0)[0x40 / 4])(
            display, nullptr, displayText);

    alignas(8) uint8_t rectangleResultBytes[0x8];
    alignas(8) uint8_t sizeBytes[0x8];
    alignas(8) uint8_t positionBytes[0xC];
    auto* rectangleResult = reinterpret_cast<uint32_t*>(rectangleResultBytes);
    auto* size = reinterpret_cast<int*>(sizeBytes);
    auto* position = reinterpret_cast<int*>(positionBytes);

    rectangleResult[0] = 0;
    rectangleResult[1] = 0;
    size[0] = 0;
    size[1] = 0;

    int* requestedSize;
    if (static_cast<uint8_t>(HudStringShow_CheckRectangle(
            static_cast<uint8_t*>(self) + 0x24, nullptr, rectangleResult))) {
        void* screen = LookupObjectById(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr);
        const int screenWidth =
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, screen, 0)[0x20 / 4])(screen, nullptr);

        screen = LookupObjectById(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr);
        position[0] = 0;
        const int screenHeight =
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, screen, 0)[0x24 / 4])(screen, nullptr);
        position[2] = static_cast<int>(
            static_cast<uint32_t>(screenHeight) * 40u);
        position[1] = static_cast<int>(
            static_cast<int64_t>(static_cast<long double>(position[2]) * g_ProgressScale));

        display = CSF_AT(void*, self, 0x20);
        reinterpret_cast<void(__fastcall*)(void*, void*, int*)>(
            CSF_AT(void**, display, 0)[0x50 / 4])(
                display, nullptr, position);

        display = CSF_AT(void*, self, 0x20);
        reinterpret_cast<void(__fastcall*)(void*, void*, int*)>(
            CSF_AT(void**, display, 0)[0x5C / 4])(
                display, nullptr, size);

        size[0] = screenWidth;
        requestedSize = size;
    } else {
        size[0] = CSF_AT(int, self, 0x24);
        size[1] = CSF_AT(int, self, 0x28);

        display = CSF_AT(void*, self, 0x20);
        reinterpret_cast<void(__fastcall*)(void*, void*, int*)>(
            CSF_AT(void**, display, 0)[0x50 / 4])(
                display, nullptr, size);

        const int left = CSF_AT(int, self, 0x24);
        const int right = CSF_AT(int, self, 0x2C);
        const int top = CSF_AT(int, self, 0x28);
        const int bottom = CSF_AT(int, self, 0x30);
        position[0] = static_cast<int>(
            static_cast<uint32_t>(right) - static_cast<uint32_t>(left));
        position[1] = static_cast<int>(
            static_cast<uint32_t>(bottom) - static_cast<uint32_t>(top));
        requestedSize = position;
    }

    display = CSF_AT(void*, self, 0x20);
    reinterpret_cast<void(__fastcall*)(void*, void*, int*)>(
        CSF_AT(void**, display, 0)[0x58 / 4])(
            display, nullptr, requestedSize);

    display = CSF_AT(void*, self, 0x20);
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, display, 0)[0x68 / 4])(display, nullptr, 1);

    display = CSF_AT(void*, self, 0x20);
    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, display, 0)[0x98 / 4])(
            display, nullptr, static_cast<uint8_t*>(self) + 0x4C);

    display = CSF_AT(void*, self, 0x20);
    return reinterpret_cast<char(__fastcall*)(void*, void*)>(
        CSF_AT(void**, display, 0)[0x08 / 4])(display, nullptr);
}

HOOK(0x004F4500, HudStringShow_UpdateDisplay, "auto");
