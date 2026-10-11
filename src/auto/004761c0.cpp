#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_008316d8_ApplyDefaultTransform(
    C_008316d8* self, void* /*edx*/)
{
    int entry = self->vResolveEntryLookupValue();
    if (!entry)
        return entry;

    void* object = reinterpret_cast<void*>(entry);
    void* target =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x68 / 4])(object, nullptr);

    alignas(4) std::uint8_t transform[0x20]{};
    CSF_AT(float, transform, 0x10) = 1.0f;
    CSF_AT(float, transform, 0x14) = 1.0f;
    CSF_AT(float, transform, 0x18) = 1.0f;
    CSF_AT(std::uint8_t, transform, 0x1D) = 1;

    return reinterpret_cast<int (__fastcall*)(
        void*, void*, int, int, void*)>(
        CSF_AT(void**, target, 0)[0x14 / 4])(
            target, nullptr, 0, 0, transform);
}

HOOK(0x004761C0, C_008316d8_ApplyDefaultTransform, "auto");
