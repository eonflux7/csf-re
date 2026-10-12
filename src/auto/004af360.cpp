#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(char, __fastcall, SetMenuMissionActiveFlag, 0x004951B0, (void* self, void* , char activeFlag));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Returns the handle-table slot address used as the handle for this object's
// mission entry: +0x628 normally, or +0x610 when the index at +0x630 is negative.
static uint8_t* SelectMissionHandleSlot(void* self)
{
    uint8_t* base = static_cast<uint8_t*>(self);
    if (CSF_AT(int, self, 0x630) < 0)
        return base + 0x610;
    return base + 0x628;
}

static char __fastcall TEstatico_UpdateMissionActiveFlag(void* self, void* /*edx*/)
{
    int v1 = CSF_AT(uint8_t, self, 0x684);
    if ((uint8_t)v1 != 0 && CSF_AT(int, self, 0x608) == 7)
    {
        uint8_t* slot = SelectMissionHandleSlot(self);
        v1 = (int)(uintptr_t)slot;
        if (CSF_AT(int, slot, 8) >= 0)
        {
            uint8_t* handleSlot = SelectMissionHandleSlot(self);
            void* resolved = reinterpret_cast<void*>(
                static_cast<uintptr_t>(CHandleTable_Resolve(g_pCurrentScene, nullptr,
                                                            static_cast<int>(reinterpret_cast<uintptr_t>(handleSlot)))));
            v1 = (int)(uintptr_t)resolved;
            if (resolved != nullptr && CSF_AT(int, resolved, 0x50) == 0)
            {
                v1 = SetMenuMissionActiveFlag(resolved, nullptr, 1);
            }
        }
    }
    return static_cast<char>(v1);
}

HOOK(0x004AF360, TEstatico_UpdateMissionActiveFlag, "auto");
