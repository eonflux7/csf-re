#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original allocator: __cdecl, one stack arg (type), returns object pointer.
CSF_FUNC(uint32_t*, __cdecl, AllocateSubobjectHandle, 0x0040A430, (int type));

// Original helper: __thiscall, ret 8. Bound as __fastcall with a dummy edx.
CSF_FUNC(char, __fastcall, C_0082e520_PrepareInputForBinding, 0x0041E050, (C_0082e520* self, void* /*edx*/, int a2, int a3));

// Original: __thiscall (ecx = this), ret 8.
// Calls the base helper first, then allocates a subobject handle (type 7), stores it in
// field_0010, and invokes its vtable slot 8 with a2. Returns the helper's char result.
static char __fastcall C_0082dc10_BindSubobjectHandle(C_0082dc10* self, void* /*edx*/, int a2, int a3)
{
    char prepareResult = C_0082e520_PrepareInputForBinding(reinterpret_cast<C_0082e520*>(self), nullptr, a2, a3);

    uint32_t* subobjectHandle = AllocateSubobjectHandle(7);
    self->field_0010 = reinterpret_cast<uint32_t>(subobjectHandle);

    // Handle's type is unknown here: call vtable slot 8 (index 2) through the raw vtable.
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, subobjectHandle, 0)[8 / 4])(subobjectHandle, nullptr, a2);

    return prepareResult;
}

HOOK(0x40D090, C_0082dc10_BindSubobjectHandle, "auto");
