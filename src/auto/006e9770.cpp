#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Resolves the linked HAnim object for a given argument (sub_6E9030 in the original).
CSF_FUNC(int, __fastcall, HAnim_ResolveLinkedObject, 0x006E9030, (void* self, void* edx, int a2));

// Returns the dword at +0x1C of the linked object when this node's +0x1C link is set, otherwise 0.
static int __fastcall C_00861e98_ReadLinkedObjectValue(C_00861e98* self, void* /*edx*/, int a2)
{
    if (CSF_AT(int, self, 0x1C) != 0)
    {
        int linkedHandle = HAnim_ResolveLinkedObject(self, nullptr, a2);
        void* linkedObject = reinterpret_cast<void*>(static_cast<uintptr_t>(linkedHandle));
        return CSF_AT(int, linkedObject, 0x1C);
    }
    return 0;
}

HOOK(0x006E9770, C_00861e98_ReadLinkedObjectValue, "auto");
