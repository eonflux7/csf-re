#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, sub_4DEA50, 0x004DEA50, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(char, __fastcall, UnregisterFromGlobalList, 0x006B1A30, (void* self, void* edx, int a2));

// Body of the C_0083e124 destructor: restores vtables, releases the
// sub-object (if flagged), frees the two heap blocks, and returns the
// result of the last Mem_Free (or the field_0020 value when it was zero).
static int __fastcall C_0083e124_Destroy(C_0083e124* self, void* /*edx*/)
{
    *(void***)self = CSF_VTBL(0x0083E124);

    void* globalObj = (void*)(uintptr_t)GetGlobalInt();
    UnregisterFromGlobalList(globalObj, nullptr, (int)(uintptr_t)self);

    if (self->field_0004)
    {
        sub_4DEA50(self, nullptr);
        self->field_0004 = 0;
    }

    if (self->field_0030)
    {
        Mem_Free((void*)(uintptr_t)self->field_0030);
        self->field_0030 = 0;
    }

    int result = (int)self->field_0020;
    if (result)
    {
        result = Mem_Free((void*)(uintptr_t)self->field_0020);
        self->field_0020 = 0;
    }

    *(void***)self = CSF_VTBL(0x0083E118);
    return result;
}

HOOK(0x4DEE30, C_0083e124_Destroy, "auto");
