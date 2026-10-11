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

// Forwarder: returns the object the dispatcher hands back (__fastcall, no extra args).
CSF_FUNC(void*, __fastcall, C_0082dd78_DispatchReturnedObject, 0x41E0C0, (C_0082dd78* self, void* edx));

// Scale for the Y component (fmul dword ptr [0x82dc7c], ~3.3333333).
CSF_GLOBAL(float, g_DirectionYScale, 0x82DC7C);
// Scale for the Z component (fmul dword ptr [0x82dce8]).
CSF_GLOBAL(float, g_WheelRecordScale, 0x82DCE8);

static char __fastcall C_0083e248_SendScaledWheelDirection(C_0083e248* self, void* /*edx*/)
{
    // Result starts as the low byte of the flag (0 on the early-out path).
    char result = static_cast<char>(self->field_0048);
    if (self->field_0048 != 0)
    {
        // Owner pointer stored at +0x0C; the dispatcher lives at owner+0x4C.
        uint32_t ownerWord = self->field_000c;
        result = static_cast<char>(ownerWord);

        C_0082dd78* dispatcher = reinterpret_cast<C_0082dd78*>(
            CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(ownerWord)), 0x4C));
        if (dispatcher != nullptr)
        {
            // First dispatch: fetch the vector source and call vtable slot 0x30/4 = 12 to get its data.
            void* sourceObj = C_0082dd78_DispatchReturnedObject(dispatcher, nullptr);
            void* vec = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, sourceObj, 0)[12])(sourceObj, nullptr);

            // Build the 12-byte record { int x; float y*scale; float z*scale; } on the stack.
            alignas(4) uint8_t directionBytes[12];
            CSF_AT(int32_t, directionBytes, 0) = CSF_AT(int32_t, vec, 0);
            float y = CSF_AT(float, vec, 4);
            float z = CSF_AT(float, vec, 8);
            CSF_AT(float, directionBytes, 4) = y * g_DirectionYScale;
            CSF_AT(float, directionBytes, 8) = z * g_WheelRecordScale;

            // Second dispatch: call vtable slot 0x2C/4 = 11 with the record pointer.
            void* sinkObj = C_0082dd78_DispatchReturnedObject(dispatcher, nullptr);
            char sendResult = reinterpret_cast<char (__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, sinkObj, 0)[11])(sinkObj, nullptr, directionBytes);
            result = sendResult;

            // Clear the pending flag.
            self->field_0048 = 0;
        }
    }
    return result;
}

HOOK(0x4E2EE0, C_0083e248_SendScaledWheelDirection, "auto");
