#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void, __fastcall, sub_4D0C10, 0x004D0C10, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, sub_4D0C50, 0x004D0C50, (void* self, void* edx, float a2));
CSF_FUNC(char, __fastcall, Autonomo_DispatchAction, 0x004D1160, (void* self, void* edx, int a2));

// Returns true when the linked owner's child reports it is not yet ready.
static bool QueryLinkedChildReady(C_0083b770* self)
{
    void* owner = reinterpret_cast<void*>(self->field_0014);
    void* child = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x4C));
    void** childVtbl = CSF_AT(void**, child, 0);
    return ((uint8_t(__fastcall*)(void*, void*))childVtbl[0x4C / 4])(child, nullptr) != 0;
}

static char __fastcall C_0083b770_UpdateAndDispatch(C_0083b770* self, void* /*edx*/, float a2)
{
    // Autonomo_DispatchAction receives the raw float bit pattern (SLODWORD(a2)).
    int32_t a2Bits;
    std::memcpy(&a2Bits, &a2, sizeof(a2Bits));

    char result = static_cast<char>(self->field_0038);
    if (result)
    {
        uint32_t phase = self->field_0044;
        if (phase)
        {
            uint32_t phaseIndex = phase - 1;
            if (phaseIndex == 0)
            {
                sub_4D0C10(self, nullptr, a2);
                return Autonomo_DispatchAction(self, nullptr, a2Bits);
            }
            if (phaseIndex == 1)
            {
                sub_4D0C50(self, nullptr, a2);
                return Autonomo_DispatchAction(self, nullptr, a2Bits);
            }
        }
        else if (!QueryLinkedChildReady(self))
        {
            self->field_0044 = 1;
        }
        return Autonomo_DispatchAction(self, nullptr, a2Bits);
    }
    return result;
}

HOOK(0x004D0DF0, C_0083b770_UpdateAndDispatch, "auto");
