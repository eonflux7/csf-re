#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Maps a logical index (0..5) to the device-side value passed to the vtable setter.
// Stored on the stack in the original; values are 2,1,3,4,5,6.
static int DeviceValueForIndex(int index, const int (&table)[6]) {
    return table[index];
}

// Sets the effect's mode through the vtable slot at +0xD0. The setter reports success
// as a bool; the index is only recorded when it succeeds. The original returns the
// setter's result (eax) either way.
static int __fastcall C_00863ba8_TrySetEffectMode(C_00863ba8* self, void* /*edx*/, int a2) {
    int result = static_cast<int>(self->field_0018);
    if (result != a2) {
        const int table[6] = {2, 1, 3, 4, 5, 6};
        int deviceValue = DeviceValueForIndex(a2, table);

        bool accepted = ((bool(__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[0xD0 / 4])(
            self, nullptr, deviceValue);
        result = static_cast<int>(accepted);
        if (accepted) {
            self->field_0018 = static_cast<uint32_t>(a2);
        }
    }
    return result;
}

HOOK(0x00710850, C_00863ba8_TrySetEffectMode, "auto");
