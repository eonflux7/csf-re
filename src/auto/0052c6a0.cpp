#include <cstdint>
#include "hook.h"

// 0x52C6A0 — menu input dispatch thunk.
// The `this` pointer addresses an embedded sub-object located 4 bytes into a
// larger object: the DWORD at [this-4] is that sub-object's vtable pointer.
// When the event id (arg2) equals 5, control is forwarded to the sub-object's
// virtual method at vtable byte offset 0x4C (slot 19), with the sub-object
// (this-4) as the callee's `this`. Otherwise nothing is done and EAX keeps
// whatever value it had (the original never writes it back).
static int __fastcall MenuProgressInput_DispatchEvent5(void* self, void* /*edx*/, int eventId, int /*param*/)
{
    if (eventId == 5)
    {
        void* subObject = reinterpret_cast<uint8_t*>(self) - 4;
        void** vtbl = *reinterpret_cast<void***>(subObject);
        return (reinterpret_cast<int(__fastcall*)(void*, void*)>(vtbl[0x4C / 4]))(subObject, nullptr);
    }

    // Original leaves EAX untouched on this path (undefined result).
    int undefinedResult;
    return undefinedResult;
}

HOOK(0x52C6A0, MenuProgressInput_DispatchEvent5, "auto");
