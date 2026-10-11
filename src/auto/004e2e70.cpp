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

CSF_FUNC(void*, __fastcall, C_0082dd78_DispatchReturnedObject, 0x41E0C0, (C_0082dd78* self, void* edx));

CSF_GLOBAL(float, g_GravityScale, 0x82E070);
CSF_GLOBAL(float, g_WheelTravelMultiplier, 0x82E24C);

// Vector triple returned by the first virtual call; fields mirror the original's [ebp-0xC..-0x4] block.
struct WheelTravelVector {
    uint32_t x;
    float y;
    float z;
};

// Runs once per object (guarded by the flag at +0x48): reads the owner's dispatch object,
// scales the returned vector, and hands it to the dispatch object's virtual slot 11.
static char __fastcall C_0083e248_SyncWheelTravelOnce(C_0083e248* self, void* /*edx*/)
{
    uint8_t result = self->field_0048;
    if (result == 0) {
        uint32_t ownerRaw = self->field_000c;
        result = static_cast<uint8_t>(ownerRaw);
        uint8_t* owner = reinterpret_cast<uint8_t*>(ownerRaw);

        C_0082dd78* dispatch = reinterpret_cast<C_0082dd78*>(CSF_AT(uint32_t, owner, 0x4C));
        if (dispatch) {
            void* firstObject = C_0082dd78_DispatchReturnedObject(dispatch, nullptr);
            void* vectorPtr = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, firstObject, 0)[12])(firstObject, nullptr);

            WheelTravelVector vec;
            vec.x = CSF_AT(uint32_t, vectorPtr, 0);
            vec.y = CSF_AT(float, vectorPtr, 4) * g_GravityScale;
            vec.z = CSF_AT(float, vectorPtr, 8) * g_WheelTravelMultiplier;

            void* secondObject = C_0082dd78_DispatchReturnedObject(dispatch, nullptr);
            result = ((char (__fastcall*)(void*, void*, void*))CSF_AT(void**, secondObject, 0)[11])(secondObject, nullptr, &vec);

            self->field_0048 = 1;
        }
    }
    return static_cast<char>(result);
}

HOOK(0x4E2E70, C_0083e248_SyncWheelTravelOnce, "auto");
