#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __fastcall, C_00864508_RemoveObjectAtIndex, 0x0071ABC0,
         (C_00864508* self, void* edx, int index));
CSF_FUNC(void*, __cdecl, CopyArrayBytes, 0x007AEE60,
         (void* destination, const void* source, std::size_t size));

static uint32_t GetSignedStrideCount(uint32_t end, uint32_t begin,
                                     unsigned shift)
{
    return static_cast<uint32_t>(static_cast<int32_t>(end - begin) >> shift);
}

static uint32_t* __fastcall C_00864508_RemoveObjectsNotInList(
    C_00864508* self, void* /*edx*/, void* retainedObjects, char removeByIndex)
{
    if (removeByIndex)
    {
        const uint32_t begin = self->field_0034;
        const uint32_t count = begin
            ? GetSignedStrideCount(self->field_0038, begin, 3)
            : 0;

        for (uint32_t index = 0; index < count; ++index)
        {
            const uint32_t objectIndex =
                CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0034),
                       index * 8);
            if (objectIndex != UINT32_MAX)
            {
                C_00864508_RemoveObjectAtIndex(
                    self, nullptr, static_cast<int32_t>(objectIndex));
            }
        }

        const uint32_t allocation = self->field_0034;
        if (allocation)
            Mem_FreeDebug(reinterpret_cast<void*>(allocation));

        self->field_0034 = 0;
        self->field_0038 = 0;
        self->field_003c = 0;
    }

    const uint32_t initialBegin = self->field_0048;
    const uint32_t initialCount = initialBegin
        ? GetSignedStrideCount(self->field_004c, initialBegin, 2)
        : 0;

    for (uint32_t index = initialCount - 1;
         static_cast<int32_t>(index) >= 0; --index)
    {
        const uint32_t entry = self->field_0048 + index * 4;
        if (CSF_AT(uint32_t, reinterpret_cast<void*>(entry), 0) == 0)
        {
            const uint32_t next = entry + 4;
            const uint32_t bytes =
                GetSignedStrideCount(self->field_004c, next, 2) << 2;
            CopyArrayBytes(reinterpret_cast<void*>(entry),
                           reinterpret_cast<void*>(next), bytes);
            self->field_004c = self->field_004c - 4;
        }
    }

    uint32_t result = self->field_0048;
    const uint32_t objectCount = result
        ? GetSignedStrideCount(self->field_004c, result, 2)
        : 0;

    for (uint32_t index = 0; index < objectCount; ++index)
    {
        const uint32_t entry = self->field_0048 + index * 4;
        bool retained = false;

        if (retainedObjects)
        {
            uint32_t candidate = CSF_AT(uint32_t, retainedObjects, 0x04);
            const uint32_t retainedCount = candidate
                ? GetSignedStrideCount(
                      CSF_AT(uint32_t, retainedObjects, 0x08), candidate, 2)
                : 0;

            for (uint32_t retainedIndex = 0;
                 retainedIndex < retainedCount; ++retainedIndex)
            {
                if (CSF_AT(uint32_t, reinterpret_cast<void*>(candidate), 0) ==
                    CSF_AT(uint32_t, reinterpret_cast<void*>(entry), 0))
                {
                    retained = true;
                    break;
                }
                candidate += 4;
            }
        }

        if (!retained)
        {
            if (removeByIndex)
            {
                C_00864508_RemoveObjectAtIndex(
                    self, nullptr, static_cast<int32_t>(index));
            }
            else
            {
                const uint32_t object =
                    CSF_AT(uint32_t, reinterpret_cast<void*>(entry), 0);
                const auto removeObject =
                    reinterpret_cast<void (__fastcall*)(
                        C_00864508*, void*, uint32_t, int)>(
                            CSF_AT(void**, self, 0)[0x60 / 4]);
                removeObject(self, nullptr, object, 1);
            }
        }

        result = objectCount;
    }

    for (uint32_t index = objectCount - 1;
         static_cast<int32_t>(index) >= 0; --index)
    {
        result = self->field_0048 + index * 4;
        if (CSF_AT(uint32_t, reinterpret_cast<void*>(result), 0) == 0)
        {
            const uint32_t next = result + 4;
            const uint32_t bytes =
                GetSignedStrideCount(self->field_004c, next, 2) << 2;
            CopyArrayBytes(reinterpret_cast<void*>(result),
                           reinterpret_cast<void*>(next), bytes);
            result = self->field_004c - 4;
            self->field_004c = result;
        }
    }

    return reinterpret_cast<uint32_t*>(result);
}

HOOK(0x0071B120, C_00864508_RemoveObjectsNotInList, "auto");
