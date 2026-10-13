#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x006917E0, __thiscall (ecx = this) with one stack argument, ret 4.
CSF_FUNC(unsigned int*, __fastcall, C_0085e5c4_LookupEntryByKey, 0x006917E0, (void* self, void* edx, unsigned int a2));

// 0x00691B30: returns 1 when the lookup finds an entry whose first dword equals the key, otherwise 0.
static int __fastcall C_0085e5c4_HasKey(C_0085e5c4* self, void* /*edx*/, unsigned int key)
{
    unsigned int* entry = C_0085e5c4_LookupEntryByKey(self, nullptr, key);
    return entry != nullptr && *entry == key;
}

HOOK(0x00691B30, C_0085e5c4_HasKey, "auto");
