#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __fastcall, C_008316d8_ResetStateFromSource, 0x00475B30, (C_008316d8* self, void*));
static CSF_FUNC(int, __fastcall, WeaponFirearms_UpdateSourceState, 0x00478480, (void* self, void* edx));

static int __fastcall C_008317c8_ResetAndClearSource(C_008317c8* self, void* /*edx*/)
{
    C_008316d8_ResetStateFromSource(self, nullptr);
    int result = self->vSlotE0();

    void* source = reinterpret_cast<void*>(self->field_0070);
    if (source)
    {
        result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, source, 0)[0xA8 / 4])(source, nullptr);
        if (result)
        {
            self->vSlotE0();
            result = WeaponFirearms_UpdateSourceState(self, nullptr);
        }
        self->field_0070 = 0;
        self->field_0074 = 0;
    }
    return result;
}

HOOK(0x004793D0, C_008317c8_ResetAndClearSource, "auto");
