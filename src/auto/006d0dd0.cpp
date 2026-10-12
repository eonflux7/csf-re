#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Calls the slot-0x68 query, converts its byte result to a 0/1 flag (1 when the
// query returned zero), then forwards that flag to the slot-0x64 virtual method.
static int __fastcall C_00860e68_InvokeSlot64WithNegatedSlot68(C_00860e68* self, void* /*edx*/)
{
    // The original reads the vtable pointer before the first virtual call.
    auto vtable = self->vtbl;

    uint8_t slot68Result = static_cast<uint8_t>(self->vSlot68());
    int flagArg = (slot68Result == 0) ? 1 : 0;

    using Slot64Fn = int(__fastcall*)(void*, void*, int);
    return reinterpret_cast<Slot64Fn>(vtable[0x64 / 4])(self, nullptr, flagArg);
}

HOOK(0x6D0DD0, C_00860e68_InvokeSlot64WithNegatedSlot68, "auto");
