#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(C_0082daf4*, __fastcall, C_0082daf4_FindOrCreateByKey, 0x004087D0, (void* self, void* /*edx*/, int key));
CSF_FUNC(int, __fastcall, ProcessMatchedEntry, 0x0040B770, (void* self, void* /*edx*/));
CSF_FUNC(float*, __fastcall, Wheel_ApplyRatio, 0x0040B550, (void* self, void* /*edx*/, int a2, float* a3));

static float* __fastcall WeaponFirearms_UpdateLinkedObject(void* self, void* /*edx*/)
{
    // Linked object pointer stored at +0x8C of the weapon.
    void* linkedObject = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x8C));

    alignas(8) float ratioValues[4];
    ratioValues[0] = -0.06666667f;
    ratioValues[1] = -0.33333334f;
    ratioValues[2] = 0.0f;
    ratioValues[3] = -1.0f;

    C_0082daf4* entry = C_0082daf4_FindOrCreateByKey(linkedObject, nullptr, 2);
    ProcessMatchedEntry(entry, nullptr);
    return Wheel_ApplyRatio(entry, nullptr, 7, ratioValues);
}

HOOK(0x0040C8A0, WeaponFirearms_UpdateLinkedObject, "auto");
