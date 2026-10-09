#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());

// Scales a volume/pitch-style value by the object's gain (field_0034) and forwards it to the
// sound object returned by the global accessor. Slot 0x30 is used for objects of type 5,
// slot 0x2C for everything else.
static int __fastcall C_00840ff0_ApplyScaledVolume(C_00840ff0* self, void* /*edx*/, int /*a2*/, float a3)
{
    int globalValue = GetGlobalValue_70DDE0();
    void* globalObj = reinterpret_cast<void*>(static_cast<uintptr_t>(globalValue));

    // vtbl slot 0x70 / 4 = 28
    int result = ((int(__fastcall*)(void*, void*))CSF_AT(void**, globalObj, 0)[0x70 / 4])(globalObj, nullptr);

    if (result) {
        float scaledValue = a3 * self->field_0034;
        void* soundObj = reinterpret_cast<void*>(static_cast<uintptr_t>(result));

        if (self->field_0004 == 5) {
            // vtbl slot 0x30 / 4 = 12
            return ((int(__fastcall*)(void*, void*, float))CSF_AT(void**, soundObj, 0)[0x30 / 4])(soundObj, nullptr, scaledValue);
        } else {
            // vtbl slot 0x2C / 4 = 11
            return ((int(__fastcall*)(void*, void*, float))CSF_AT(void**, soundObj, 0)[0x2C / 4])(soundObj, nullptr, scaledValue);
        }
    }
    return result;
}

HOOK(0x00526570, C_00840ff0_ApplyScaledVolume, "auto");
