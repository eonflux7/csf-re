#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// sub_6510B0 (__fastcall, one stack arg, ret 4)
CSF_FUNC(char, __fastcall, sub_6510B0, 0x006510B0, (void* self, void* edx, int a2));

// __thiscall at 0x73CAE0 -> __fastcall shim.
// Loads the object at +0xD0; if it is non-null and its +0xC word equals 1,
// calls sub_6510B0(obj, 4) and returns its result. Otherwise returns 0
// (the original leaves eax undefined on that path).
static int __fastcall NetSession_ProcessLinkedObjectIfReady(C_0085e760** self, void* /*edx*/)
{
    C_0085e760* linkedObject = CSF_AT(C_0085e760*, self, 0xD0);
    int result = 0;
    if (linkedObject)
    {
        if (CSF_AT(int, linkedObject, 0xC) == 1)
        {
            return sub_6510B0(linkedObject, nullptr, 4);
        }
    }
    return result;
}

HOOK(0x0073CAE0, NetSession_ProcessLinkedObjectIfReady, "auto");
