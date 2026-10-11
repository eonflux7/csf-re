#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

using DeferredCallback = int (__cdecl*)(uint32_t, int);

CSF_FUNC(bool, __fastcall, GfxScene_InvokeCallback, 0x6BCA60,
         (void* self, void* edx, DeferredCallback callback, int argumentBlock));

static bool __fastcall C_00860bb8_QueueDeferredCallback(
    C_00860bb8* self, void* /*edx*/, int value, char flag)
{
    void* target = reinterpret_cast<void*>(self->field_0008);
    if (!target)
        return false;

    alignas(8) uint8_t argumentBytes[0x8] = {};
    CSF_AT(int, argumentBytes, 0x0) = value;
    CSF_AT(char, argumentBytes, 0x4) = flag;

    return GfxScene_InvokeCallback(
        target, nullptr,
        reinterpret_cast<DeferredCallback>(CSF_ADDR_FN(0x6CD720)),
        static_cast<int>(reinterpret_cast<uintptr_t>(argumentBytes)));
}

HOOK(0x6CD7A0, C_00860bb8_QueueDeferredCallback, "auto");
