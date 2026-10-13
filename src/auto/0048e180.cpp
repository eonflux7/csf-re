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

// Object at +0x38 is an unknown polymorphic type; its vtable slots 44 (+0xB0) and 45 (+0xB4)
// are called with the object as `this`, returning a bool (test al, al).
static bool __fastcall IsTargetActive_CallVirtualSlot44(void* obj)
{
    return ((bool(__fastcall*)(void*, void*))CSF_AT(void**, obj, 0)[0xB0 / 4])(obj, nullptr);
}

static bool __fastcall IsTargetActive_CallVirtualSlot45(void* obj)
{
    return ((bool(__fastcall*)(void*, void*))CSF_AT(void**, obj, 0)[0xB4 / 4])(obj, nullptr);
}

// __thiscall, no stack args -> ret 0.
static int __fastcall IsTargetActive(void* self, void* /*edx*/)
{
    int result = 0;
    if ((CSF_AT(uint8_t, self, 0x170) & 1) != 0)
    {
        void* firstTarget = CSF_AT(void*, self, 0x38);
        if (!firstTarget)
            return 1;
        if (!IsTargetActive_CallVirtualSlot44(firstTarget))
        {
            void* secondTarget = CSF_AT(void*, self, 0x38);
            if (!secondTarget || !IsTargetActive_CallVirtualSlot45(secondTarget))
                return 1;
        }
    }
    return result;
}

HOOK(0x0048E180, IsTargetActive, "auto");
