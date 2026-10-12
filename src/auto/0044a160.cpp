#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __stdcall, sub_4496C0, 0x004496C0, (void* a1, int a2));

static int __fastcall S_00448a80_UpdateEntryState(
    S_00448a80* self, void* /*edx*/, int entryAddress)
{
    auto* entry = reinterpret_cast<void*>(entryAddress);
    const int savedState = CSF_AT(int, entry, 0x1C);

    bool active = false;
    if (CSF_AT(uint8_t, entry, 0x00) == 0)
    {
        void* owner = CSF_AT(void*, entry, 0x04);
        active = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, owner, 0)[0x80 / 4])(
                owner, nullptr, 0, entryAddress);
    }

    CSF_AT(uint8_t, entry, 0x00) = active;
    CSF_AT(int, entry, 0x1C) = 0;
    if (!active)
    {
        CSF_AT(uint8_t, entry, 0x15) = 0;
        CSF_AT(uint8_t, entry, 0x16) = 0;
        sub_4496C0(self, entryAddress);
    }
    CSF_AT(int, entry, 0x1C) = savedState;

    if (CSF_AT(uint8_t, entry, 0x00) == 0)
    {
        void* owner = CSF_AT(void*, entry, 0x04);
        if (reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, owner, 0)[0x7C / 4])(
                    owner, nullptr, entryAddress))
        {
            CSF_AT(uint8_t, entry, 0x00) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, entry, 0x00) = 1;
    return 1;
}

HOOK(0x0044A160, S_00448a80_UpdateEntryState, "auto");
