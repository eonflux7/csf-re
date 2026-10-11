#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, AddObjectReference, 0x006923B0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0, (void* self, void* edx));

static int __fastcall C_008670f0_SetObjectReference(
    C_008670f0* self, void* /*edx*/, int reference)
{
    const uint32_t oldReference = self->field_0014;
    int result = 0;

    if (oldReference != static_cast<uint32_t>(reference))
    {
        self->field_0010 = 1;
        if (oldReference)
            result = ReleaseObjectReference(
                reinterpret_cast<void*>(oldReference), nullptr);

        self->field_0014 = static_cast<uint32_t>(reference);
        self->field_00d8 = 1;

        if (reference)
        {
            AddObjectReference(reinterpret_cast<void*>(reference), nullptr);
            return 0;
        }
    }

    return result;
}

HOOK(0x007329E0, C_008670f0_SetObjectReference, "auto");
