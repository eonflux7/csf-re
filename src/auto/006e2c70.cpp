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

// Teardown for C_00860104 base: if the flag at +0x04 is set, release the owned
// sub-object at +0x3C via its slot 0 (argument 1), clear the pointer, then clear the flag.
// The owned object's real type is unknown, so the slot-0 call goes through its vtable.
static char __fastcall C_00860104_BaseTeardown(C_00860120* self, void* /*edx*/)
{
    char result = static_cast<char>(self->field_0004);
    if (result)
    {
        void* ownedObject = reinterpret_cast<void*>(self->field_003c);
        if (ownedObject)
        {
            result = ((char(__fastcall*)(void*, void*, int))CSF_AT(void**, ownedObject, 0)[0])(ownedObject, nullptr, 1);
            self->field_003c = 0;
        }
        self->field_0004 = 0;
    }
    return result;
}

HOOK(0x6E2C70, C_00860104_BaseTeardown, "auto");
