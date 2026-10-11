#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original allocator: __cdecl, one stack arg (size), returns object pointer.
CSF_FUNC(uint32_t*, __cdecl, sub_40A430, 0x0040A430, (int a1));

// Original helper: __thiscall, ret 8. Bound as __fastcall with a dummy edx.
CSF_FUNC(char, __fastcall, sub_41E050, 0x0041E050, (void* self, void* edx, int a2, int a3));

// Original: __thiscall (ecx = this), ret 8.
// Calls the base helper first, then allocates a 12-byte object, stores it in
// field_0010, and invokes its vtable slot 8 with a2. Returns the helper's char result.
static char __fastcall C_0082df70_InitAndAttachHelper(C_0082df70* self, void* /*edx*/, int a2, int a3)
{
    char helperResult = sub_41E050(self, nullptr, a2, a3);

    uint32_t* allocated = sub_40A430(12);
    self->field_0010 = reinterpret_cast<uint32_t>(allocated);

    // Object type is unknown here: call vtable slot 8 (index 2) through the raw vtable.
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, allocated, 0)[8 / 4])(allocated, nullptr, a2);

    return helperResult;
}

HOOK(0x411670, C_0082df70_InitAndAttachHelper, "auto");
