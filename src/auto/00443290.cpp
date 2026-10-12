#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callees (bound by address, signatures from the packet).
CSF_FUNC(void*, __stdcall, DynamicObj_ReleaseTimedObject, 0x00442BB0, (int a1));
CSF_FUNC(int, __stdcall, DynamicObj_CheckLifetimeExpired, 0x00442C40, (int a1));
CSF_FUNC(int, __stdcall, DynamicObj_HandleZeroCounter, 0x00442CE0, (int a1));

// Advances the timer of every dynamic object in the list by elapsed time and
// dispatches the per-object checks. Returns the final list count.
static uint32_t __fastcall DynamicObj_AdvanceAllObjectTimers(S_00442f90* self, void* /*edx*/, float elapsed)
{
    uint32_t result = self->field_0014;
    for (uint32_t i = 0; i < result; ++i)
    {
        uint32_t objAddr = reinterpret_cast<uint32_t*>(self->field_000c)[i];
        uint8_t* obj = reinterpret_cast<uint8_t*>(objAddr);

        uint32_t flagsAtEntry = CSF_AT(uint32_t, obj, 0xC);
        if ((flagsAtEntry & 2) == 0)
        {
            float newTime = elapsed + CSF_AT(float, obj, 0x10);
            CSF_AT(float, obj, 0x10) = newTime;

            if ((flagsAtEntry & 1) == 0 && newTime >= CSF_AT(float, obj, 0x14))
                DynamicObj_ReleaseTimedObject(objAddr);

            if ((CSF_AT(uint32_t, obj, 0xC) & 1) != 0 && CSF_AT(uint32_t, obj, 0x4) == 0)
                DynamicObj_HandleZeroCounter(objAddr);

            if ((CSF_AT(uint32_t, obj, 0xC) & 4) == 0 &&
                CSF_AT(float, obj, 0x10) >= CSF_AT(float, obj, 0x18))
                DynamicObj_CheckLifetimeExpired(objAddr);
        }
        result = self->field_0014;
    }
    return result;
}

HOOK(0x00443290, DynamicObj_AdvanceAllObjectTimers, "auto");
