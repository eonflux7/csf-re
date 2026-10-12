#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_0040c070_ClearElements, 0x0040B930,
         (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, S_0040c070_ReserveElements, 0x0040BB80,
         (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __fastcall, S_0040c070_SerializeElement, 0x0040C110,
         (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, S_0040c070_AppendElement, 0x0040C2A0,
         (void* self, void* edx, uint32_t* a3));

static int __stdcall S_0040c070_SerializeElements(S_0040c070* self, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);
    const uint8_t writing = CSF_AT(uint8_t, context, 0x14);
    const uint8_t updateExisting = CSF_AT(uint8_t, context, 0x15);

    alignas(8) uint8_t countBytes[0x4];
    uint32_t& count = *reinterpret_cast<uint32_t*>(countBytes);
    count = 0;

    if (writing)
    {
        const uint32_t begin = self->field_0004;
        count = begin
            ? static_cast<int32_t>(self->field_0008 - begin) / 28
            : 0;
    }

    bool failed = false;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* serializer = CSF_AT(void*, context, 0x4);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, uint32_t*, int)>(
            CSF_AT(void**, serializer, 0)[120 / 4])(
                serializer, nullptr, &count, contextAddress);
    }
    CSF_AT(uint8_t, context, 0) = failed;

    if (!failed)
    {
        if (writing)
        {
            uint32_t limit = count;
            uint32_t index = 0;
            uint32_t offset = 0;
            while (index < limit)
            {
                void* element = reinterpret_cast<void*>(self->field_0004 + offset);
                const bool canProcess = CSF_AT(uint8_t, context, 0) == 0;
                CSF_AT(uint32_t, context, 0x1C) = 0;
                if (canProcess)
                {
                    CSF_AT(uint8_t, context, 0x15) = updateExisting;
                    CSF_AT(uint8_t, context, 0x16) = 0;
                    S_0040c070_SerializeElement(element, nullptr, contextAddress);
                    limit = count;
                }
                ++index;
                offset += 28;
            }
        }
        else if (updateExisting)
        {
            uint32_t index = 0;
            uint32_t offset = 0;
            while (index < count)
            {
                void* element = reinterpret_cast<void*>(self->field_0004 + offset);
                const bool canProcess = CSF_AT(uint8_t, context, 0) == 0;
                CSF_AT(uint32_t, context, 0x1C) = 0;
                if (canProcess)
                {
                    CSF_AT(uint8_t, context, 0x15) = updateExisting;
                    CSF_AT(uint8_t, context, 0x16) = 0;
                    S_0040c070_SerializeElement(element, nullptr, contextAddress);
                }
                ++index;
                offset += 28;
            }
        }
        else
        {
            S_0040c070_ClearElements(self, nullptr);
            S_0040c070_ReserveElements(self, nullptr, count);

            if (count)
            {
                alignas(8) uint8_t elementBytes[0x1C];
                uint32_t* defaultElement = reinterpret_cast<uint32_t*>(elementBytes);
                defaultElement[6] = 10;
                std::memset(elementBytes, 0, 24);

                uint32_t index = 0;
                uint32_t offset = 0;
                do
                {
                    S_0040c070_AppendElement(self, nullptr, defaultElement);
                    void* element = reinterpret_cast<void*>(self->field_0004 + offset);
                    const bool canProcess = CSF_AT(uint8_t, context, 0) == 0;
                    CSF_AT(uint32_t, context, 0x1C) = 0;
                    if (canProcess)
                    {
                        CSF_AT(uint8_t, context, 0x15) = 0;
                        CSF_AT(uint8_t, context, 0x16) = 0;
                        S_0040c070_SerializeElement(element, nullptr, contextAddress);
                    }
                    offset += 28;
                    ++index;
                } while (index < count);
            }
        }

        if (CSF_AT(uint8_t, context, 0) == 0)
        {
            void* serializer = CSF_AT(void*, context, 0x4);
            if (reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, serializer, 0)[124 / 4])(
                        serializer, nullptr, contextAddress))
            {
                CSF_AT(uint8_t, context, 0) = 0;
                return 0;
            }
        }
    }

    CSF_AT(uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x0040C3C0, S_0040c070_SerializeElements, "auto");
