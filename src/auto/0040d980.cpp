#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, AllocateSubobjectHandle, 0x0040A430, (int type));
CSF_FUNC(int, __fastcall, sub_40D140, 0x0040D140, (void* self, void* edx, int a2, int a3));

// Stores the request id in field_0004, allocates a type-14 subobject handle into field_0030,
// asks the handle (vtable slot 2) whether it accepts a2, and ANDs that flag with the
// result of the owner's own check (sub_40D140).
static char __fastcall C_0082dcf0_InitSubobjectAndCheck(C_0082dcf0* self, void* /*edx*/, int a2, int a3)
{
    self->field_0004 = static_cast<uint32_t>(a2);

    uint32_t* subobjectHandle = AllocateSubobjectHandle(14);
    self->field_0030 = reinterpret_cast<uint32_t>(subobjectHandle);

    char handleAccepts =
        ((char(__fastcall*)(void*, void*, int))(CSF_AT(void**, subobjectHandle, 0)[2]))(
            subobjectHandle, nullptr, a2) & 1;

    return static_cast<char>(handleAccepts & sub_40D140(self, nullptr, a2, a3));
}

HOOK(0x0040D980, C_0082dcf0_InitSubobjectAndCheck, "auto");
