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

// Refreshes the cached 0x64-byte snapshot in this object when it is inactive,
// or when the source record has a nonzero flag at +0x4C and the current flag at +0x1D8 is clear.
// Returns the value returned by the sub-object's slot-15 method, or the flag value that was tested.
static int __fastcall C_0082e128_CaptureSnapshot(C_0082e128* self, void* /*edx*/,
                                                 int /*a2*/, uint32_t* sourceRecord, int /*a4*/)
{
    int result = 0;

    if (self->field_01f4 == 0 ||
        ((result = static_cast<int>(sourceRecord[19])) != 0 &&
         (result = static_cast<int>(self->field_01d8)) == 0))
    {
        // Fetch the owned sub-object at +0x04, then ask it (slot 10) for its state object.
        void* owner = reinterpret_cast<void*>(self->field_0004);
        void* stateObject = reinterpret_cast<void*>(
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(CSF_AT(void**, owner, 0)[10])(owner, nullptr));

        // Slot 15 on the state object with argument 0.
        result = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, stateObject, 0)[15])(stateObject, nullptr, 0);

        // Copy 0x64 bytes (25 dwords) of the source record into the snapshot area.
        std::memcpy(&self->field_018c, sourceRecord, 0x64);

        self->field_01f4 = 1;
        self->field_01f8 = 0.2f;
    }

    return result;
}

HOOK(0x00417EA0, C_0082e128_CaptureSnapshot, "auto");
