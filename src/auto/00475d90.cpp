#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_008316d8_DispatchFireAction(
    C_008316d8* self, void* /*edx*/, int action)
{
    const int mode =
        reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0x48 / 4])(self, nullptr);

    switch (mode)
    {
    case 1:
    case 3:
        return self->vSlotC0(action);
    case 2:
        return self->vSlotC4(action);
    default:
        return static_cast<int>(static_cast<std::uint32_t>(mode) - 3u);
    }
}

HOOK(0x00475D90, C_008316d8_DispatchFireAction, "auto");
