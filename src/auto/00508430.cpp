#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees (__fastcall, one stack arg `char` popped by ret 4)
CSF_FUNC(unsigned int, __fastcall, CreditsDestroyFlagObject_508360, 0x00508360, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CreditsReleaseSlot34_507DE0, 0x00507DE0, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, CreditsReleaseSlot44_507EA0, 0x00507EA0, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, CreditsReleaseSlot54_508180, 0x00508180, (void* self, void* edx, char a2));

static unsigned int __fastcall S_00508360_DestroyEmbeddedMembers(S_00508360* self, void* /*edx*/)
{
    // The original references this address as a vtable; no store is performed
    // (the struct has no vtable field and the original writes nothing here).
    (void)CSF_VTBL(0x00508360);

    unsigned int result = 0;

    if (self->field_0000)
    {
        result = CreditsDestroyFlagObject_508360(self, nullptr);
        self->field_0000 = 0;
    }

    uint32_t slot54 = self->field_0054;
    if (slot54)
    {
        result = static_cast<unsigned int>(
            CreditsReleaseSlot54_508180(reinterpret_cast<void*>(slot54), nullptr, 3));
        self->field_0054 = 0;
    }

    uint32_t slot44 = self->field_0044;
    if (slot44)
    {
        result = static_cast<unsigned int>(
            CreditsReleaseSlot44_507EA0(reinterpret_cast<void*>(slot44), nullptr, 3));
        self->field_0044 = 0;
    }

    uint32_t slot34 = self->field_0034;
    if (slot34)
    {
        result = static_cast<unsigned int>(
            CreditsReleaseSlot34_507DE0(reinterpret_cast<void*>(slot34), nullptr, 3));
        self->field_0034 = 0;
    }

    return result;
}

HOOK(0x00508430, S_00508360_DestroyEmbeddedMembers, "auto");
