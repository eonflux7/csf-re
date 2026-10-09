#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));

static int __fastcall CCharWeaponFsm_FindMatchingHandle(
    CCharWeaponFsm* self, void* /*edx*/, int requiredValue, int excludedValue)
{
    const uint32_t begin = self->field_0018;
    const uint32_t count = begin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->field_001c - begin) >> 2)
        : 0;

    for (uint32_t index = 0; index < count; ++index)
    {
        void* handle = reinterpret_cast<void**>(self->field_0018)[index];
        void* value = reinterpret_cast<void*>(
            Handle_GetValuePointer(handle, nullptr));

        if (CSF_AT(int, value, 0x74) != excludedValue)
        {
            handle = reinterpret_cast<void**>(self->field_0018)[index];
            value = reinterpret_cast<void*>(
                Handle_GetValuePointer(handle, nullptr));

            if (CSF_AT(int, value, 0xDC) == requiredValue)
            {
                const int matchedHandle = CSF_AT(
                    int, reinterpret_cast<void*>(self->field_0018), index * 4);
                if (matchedHandle != 0)
                    return matchedHandle;
            }
        }
    }

    return 0;
}

HOOK(0x00481200, CCharWeaponFsm_FindMatchingHandle, "auto");
