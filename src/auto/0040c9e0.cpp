#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(char, __fastcall, C_0082e520_PrepareInputForBinding, 0x0041E050,
         (C_0082e520* self, void* /*edx*/, int a2, int a3));
CSF_FUNC(uint32_t*, __cdecl, AllocateSubobjectHandle, 0x0040A430, (int type));
CSF_FUNC(bool, __fastcall, IsRecordSlotActive, 0x00407F40, (void* self, void* /*edx*/, int index));
CSF_FUNC(int, __fastcall, C_00833908_GetTargetName, 0x0048FE90, (C_00833908* self, void* /*edx*/));

// Binds an input to a newly allocated subobject handle and reports whether
// the record slot is active. Returns 1 if both the input prep and the handle
// check succeeded, otherwise 0. Pops 8 bytes of arguments (a2, a3).
static char __fastcall C_0082db60_BindInputSubobject(C_0082db60* self, void* /*edx*/, int a2, int a3)
{
    char prepareResult = C_0082e520_PrepareInputForBinding(
        reinterpret_cast<C_0082e520*>(self), nullptr, a2, a3);

    uint32_t* subobjectHandle = AllocateSubobjectHandle(5);
    self->field_008c = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(subobjectHandle));

    // Virtual call on the handle object: vtable slot 8 / 4 = 2.
    using HandleMethod = char(__fastcall*)(void* self, void* edx, int a2);
    char handleResult = reinterpret_cast<HandleMethod>(CSF_AT(void**, subobjectHandle, 0)[2])(
        subobjectHandle, nullptr, a2);

    char combined = handleResult & prepareResult;

    if (IsRecordSlotActive(reinterpret_cast<void*>(self->field_008c), nullptr, 0))
        return combined & 1;

    C_00833908_GetTargetName(reinterpret_cast<C_00833908*>(self->field_0004), nullptr);
    return 0;
}

HOOK(0x0040C9E0, C_0082db60_BindInputSubobject, "auto");
