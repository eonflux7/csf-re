#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S_004dca10_Grow, 0x004DCA10,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SerializeElement, 0x004DCE80,
         (void* self, void* edx, void* a2));

static const auto kGrowthVtable = CSF_VTBL(0x004DCA10);

static int __stdcall S_004dca10_SerializeElements(S_004dca10* self, int a2)
{
    void* context = reinterpret_cast<void*>(a2);
    const bool writing = CSF_AT(uint8_t, context, 0x14) != 0;
    const uint8_t existingElements = CSF_AT(uint8_t, context, 0x15);

    alignas(8) uint8_t countBytes[0x4];
    uint32_t& count = *reinterpret_cast<uint32_t*>(countBytes);
    count = writing ? self->field_0008 : 0;

    bool failed = false;
    if (CSF_AT(uint8_t, context, 0x0) == 0)
    {
        void* serializer = CSF_AT(void*, context, 0x4);
        failed = !reinterpret_cast<bool(__fastcall*)(
            void*, void*, uint32_t*, int)>(
                CSF_AT(void**, serializer, 0x0)[0x78 / 4])(
                    serializer, nullptr, &count, a2);
    }
    CSF_AT(uint8_t, context, 0x0) = failed;
    if (failed)
    {
        CSF_AT(uint8_t, context, 0x0) = 1;
        return 1;
    }

    if (writing || existingElements != 0)
    {
        uint32_t index = 0;
        uint32_t offset = 0;
        while (index < count)
        {
            void* element = reinterpret_cast<void*>(self->field_0000 + offset);
            const bool canSerialize = CSF_AT(uint8_t, context, 0x0) == 0;
            CSF_AT(uint32_t, context, 0x1C) = 0;
            if (canSerialize)
            {
                CSF_AT(uint8_t, context, 0x15) = existingElements;
                CSF_AT(uint8_t, context, 0x16) = 0;
                SerializeElement(element, nullptr, context);
            }
            ++index;
            offset += 28;
        }
    }
    else
    {
        const uint32_t capacity = self->field_0004;
        const uint32_t requestedCount = count;
        self->field_0008 = 0;
        if (requestedCount > capacity)
            S_004dca10_Grow(self, nullptr,
                           static_cast<int>(requestedCount - capacity));

        uint32_t index = 0;
        uint32_t offset = 0;
        while (index < count)
        {
            if (static_cast<uint32_t>(self->field_0008) >=
                static_cast<uint32_t>(self->field_0004))
            {
                S_004dca10_Grow(self, nullptr,
                               static_cast<int>(self->field_000c));
            }

            const uint32_t insertionIndex = self->field_0008;
            void* newElement = reinterpret_cast<void*>(
                self->field_0000 + 28 * insertionIndex);
            self->field_0008 = insertionIndex + 1;
            CSF_AT(uint32_t, newElement, 0x0) = 0;
            CSF_AT(int32_t, newElement, 0x8) = -1;
            CSF_AT(int32_t, newElement, 0xC) = -1;
            CSF_AT(uint32_t, newElement, 0x10) = 0;
            CSF_AT(uint32_t, newElement, 0x14) = 0;
            CSF_AT(uint32_t, newElement, 0x18) = 0;

            void* element = reinterpret_cast<void*>(self->field_0000 + offset);
            const bool canSerialize = CSF_AT(uint8_t, context, 0x0) == 0;
            CSF_AT(uint32_t, context, 0x1C) = 0;
            if (canSerialize)
            {
                CSF_AT(uint8_t, context, 0x15) = 0;
                CSF_AT(uint8_t, context, 0x16) = 0;
                SerializeElement(element, nullptr, context);
            }
            ++index;
            offset += 28;
        }
    }

    if (CSF_AT(uint8_t, context, 0x0) == 0)
    {
        void* serializer = CSF_AT(void*, context, 0x4);
        if (reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, serializer, 0x0)[0x7C / 4])(
                    serializer, nullptr, a2))
        {
            CSF_AT(uint8_t, context, 0x0) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, context, 0x0) = 1;
    return 1;
}

HOOK(0x004DD870, S_004dca10_SerializeElements, "auto");
