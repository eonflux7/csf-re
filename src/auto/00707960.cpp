#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __cdecl, RtQuatRotate, 0x007D9E30, (int a1, float* a2, float a3, int a4));

// Rotates the interpolator's quaternion when one is set, then dispatches the
// object's slot-35 virtual method with the second stored value. Returns that
// dispatch's result, or the quaternion pointer when no quaternion is set.
static int __fastcall C_008633a0_RotateAndRefresh(C_008633a0* self, void* /*edx*/, int a2, float a3)
{
    int result = static_cast<int>(self->field_0010);
    if (result)
    {
        RtQuatRotate(result, reinterpret_cast<float*>(a2), a3, 2);

        void* animObj = reinterpret_cast<void*>(self->field_0008);
        using SlotFn = int(__fastcall*)(void*, void*, uint32_t);
        SlotFn slot = reinterpret_cast<SlotFn>(CSF_AT(void**, animObj, 0)[0x8c / 4]);
        result = slot(animObj, nullptr, self->field_000c);

        self->field_0020 = 1;
    }
    return result;
}

HOOK(0x00707960, C_008633a0_RotateAndRefresh, "auto");
