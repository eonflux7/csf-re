#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Cheats_GetCheatName, 0x0042A1B0,
         (int index, void* output));
CSF_FUNC(void*, __cdecl, Cheats_FormatCheatValue, 0x0042A3B0,
         (unsigned int value, unsigned int uCode, void* output));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static void* __cdecl Cheats_AddCheatMenuRow(
    int index, void* entries, void* menu)
{
    void* entry = static_cast<uint8_t*>(entries) +
                  20 * static_cast<uint32_t>(index);
    if (CSF_AT(uint32_t, entry, 0) == UINT32_MAX)
        return entry;

    const int row =
        ((int(__fastcall*)(void*, void*))
             CSF_AT(void**, menu, 0)[0x274 / 4])(menu, nullptr);

    alignas(8) uint8_t stringBytes[0x38];
    void* nameStorage = stringBytes;
    void* valueStorage = stringBytes + 0x1C;

    CSF_AT(uint32_t, nameStorage, 0x14) = 0;
    CSF_AT(uint16_t, nameStorage, 0x04) = 0;
    CSF_AT(uint32_t, valueStorage, 0x14) = 0;
    CSF_AT(uint16_t, valueStorage, 0x04) = 0;
    CSF_AT(uint32_t, nameStorage, 0x18) = 7;
    CSF_AT(uint32_t, valueStorage, 0x18) = 7;

    auto* name = static_cast<S_004f21d0*>(
        Cheats_GetCheatName(index, nameStorage));
    uint16_t* nameText;
    if (name->field_0018 < 8)
        nameText = &name->field_0004;
    else
        nameText = *reinterpret_cast<uint16_t**>(&name->field_0004);

    ((void(__fastcall*)(void*, void*, int, int, uint16_t*, int))
         CSF_AT(void**, menu, 0)[0x294 / 4])(
        menu, nullptr, row, 0, nameText, index);

    Cheats_FormatCheatValue(
        CSF_AT(uint32_t, entry, 0),
        CSF_AT(uint32_t, entry, 4),
        valueStorage);

    uint16_t* valueText =
        CSF_AT(uint32_t, valueStorage, 0x18) < 8
            ? reinterpret_cast<uint16_t*>(
                  static_cast<uint8_t*>(valueStorage) + 4)
            : CSF_AT(uint16_t*, valueStorage, 4);

    ((void(__fastcall*)(void*, void*, int, int, uint16_t*, int))
         CSF_AT(void**, menu, 0)[0x294 / 4])(
        menu, nullptr, row, 1, valueText, index);

    if (CSF_AT(uint32_t, valueStorage, 0x18) >= 8) {
        void* block = CSF_AT(void*, valueStorage, 4);
        if (block)
            Mem_FreeDebug(block);
    }

    const uint32_t nameCapacity = CSF_AT(uint32_t, nameStorage, 0x18);
    CSF_AT(uint32_t, valueStorage, 0x18) = 7;
    CSF_AT(uint32_t, valueStorage, 0x14) = 0;
    CSF_AT(uint16_t, valueStorage, 4) = 0;

    if (nameCapacity >= 8) {
        void* block = CSF_AT(void*, nameStorage, 4);
        if (block)
            return reinterpret_cast<void*>(
                static_cast<uintptr_t>(Mem_FreeDebug(block)));
        return block;
    }

    return reinterpret_cast<void*>(static_cast<uintptr_t>(nameCapacity));
}

HOOK(0x0042AD00, Cheats_AddCheatMenuRow, "auto");
