#include <cstdint>
#include "hook.h"

// 0x0050D7C0 (__thiscall, ret 4)
// Compares the byte flag at this+0x88 against the low byte of the argument.
// If they differ, it stores the new byte and, when the child object pointer
// at this+0x10 is non-null, notifies that child through virtual slot 6
// (offset 0x18) passing the full 32-bit argument. Returns the argument when
// no virtual call is made, otherwise returns the virtual call's result.
static int __fastcall MenuCommon_SetFlagAndNotifyChild(void* self, void* /*edx*/, int value)
{
    uint8_t* base = static_cast<uint8_t*>(self);

    if (base[0x88] != static_cast<uint8_t>(value))
    {
        base[0x88] = static_cast<uint8_t>(value);

        void* child = *reinterpret_cast<void**>(base + 0x10);
        if (child)
        {
            void* vtable = *reinterpret_cast<void**>(child);
            auto notify = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                reinterpret_cast<void**>(vtable)[6]);
            return notify(child, nullptr, value);
        }
    }

    return value;
}

HOOK(0x50D7C0, MenuCommon_SetFlagAndNotifyChild, "auto");
