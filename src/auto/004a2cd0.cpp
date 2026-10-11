#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Sends a 3-dword message {a2, value, kind} to the listener sub-object at (*(self+8))+4,
// via its vtable slot 2 (__thiscall, one stack argument).
static void SlotEvents_Notify(uint8_t* self, int a2, int value, int kind)
{
    uint32_t msg[3];
    msg[0] = static_cast<uint32_t>(a2);
    msg[1] = static_cast<uint32_t>(value);
    msg[2] = static_cast<uint32_t>(kind);

    uint8_t* target = CSF_AT(uint8_t*, self, 8) + 4;
    void** vtbl = CSF_AT(void**, target, 0);
    ((void(__fastcall*)(void*, void*, void*))vtbl[2])(target, nullptr, msg);
}

static int __fastcall SlotEvents_SetSlot(void* selfPtr, void* /*edx*/, int a2, int a3, int a4)
{
    uint8_t* self = static_cast<uint8_t*>(selfPtr);

    if (a3 != 0)
    {
        if (a3 > 0 && a3 <= 2)
        {
            int cur = CSF_AT(int, self, 0xC);
            if (cur != 0)
            {
                SlotEvents_Notify(self, a2, cur, a3);
                CSF_AT(int, self, 0xC) = 0;
                CSF_AT(int, self, 0x10) = 0;
            }
        }
    }
    else
    {
        int cur = CSF_AT(int, self, 0x1C);
        if (cur != 0)
        {
            SlotEvents_Notify(self, a2, cur, 0);
            CSF_AT(int, self, 0x1C) = 0;
            CSF_AT(int, self, 0x20) = 0;
        }
        cur = CSF_AT(int, self, 0x2C);
        if (cur != 0)
        {
            SlotEvents_Notify(self, a2, cur, 0);
            CSF_AT(int, self, 0x2C) = 0;
            CSF_AT(int, self, 0x30) = 0;
        }
    }

    int off = a3 * 16 + 0xC;
    int prev = CSF_AT(int, self, off);
    SlotEvents_Notify(self, a2, prev, a3);
    CSF_AT(int, self, off) = a2;
    CSF_AT(int, self, off + 4) = a4;
    return a4;
}

HOOK(0x004A2CD0, SlotEvents_SetSlot, "auto");
