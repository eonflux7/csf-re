#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_4A4F10, 0x004A4F10, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, IsField1BCNonNegative, 0x00492B10, (void* self, void*));
CSF_FUNC(int, __fastcall, sub_4A3180, 0x004A3180, (void* self, void* edx, int a2));
CSF_FUNC(void**, __fastcall, sub_4A4C70, 0x004A4C70, (void* self, void* edx, void* arg0));
CSF_FUNC(int, __fastcall, sub_4A4A40, 0x004A4A40, (void* self, void* edx, void* a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

static int __fastcall C_00834598_DispatchModeCommand(C_00834598* self, void* /*edx*/, int a2)
{
    int lookupIndex = sub_4A4F10(self, nullptr, a2);

    if (lookupIndex == -1)
    {
        // No direct entry: fall back depending on the flags word at +0x08.
        if (!IsField1BCNonNegative(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0008)), nullptr))
            return sub_4A3180(self, nullptr, a2);

        void** resolved = sub_4A4C70(self, nullptr, reinterpret_cast<void*>(static_cast<uintptr_t>(a2)));
        if (resolved == reinterpret_cast<void**>(static_cast<intptr_t>(-1)))
            return sub_4A3180(self, nullptr, a2);

        return sub_4A4A40(self, nullptr, resolved);
    }

    // Direct entry: forward through the game object's slot-2 virtual method.
    uint8_t* game = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetValue8B0554()));
    void* target = CSF_AT(void*, game, 0x1FC4);

    uint8_t* table = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_007c));
    uint32_t entryValue = CSF_AT(uint32_t, table + lookupIndex * 8, 4);

    return ((int(__fastcall*)(void*, void*, int))(CSF_AT(void**, target, 0)[2]))(
        target, nullptr, static_cast<int>(entryValue));
}

HOOK(0x004A5250, C_00834598_DispatchModeCommand, "auto");
