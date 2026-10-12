#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __stdcall TypeRegistry_SerializeIndirectValue(
    uint32_t* value, void* context)
{
    if (CSF_AT(uint8_t, context, 0x14))
    {
        uint32_t indirectValue = *value;
        const bool inactive = CSF_AT(uint8_t, context, 0x00) == 0;
        CSF_AT(uint32_t, context, 0x1C) = 0;
        if (inactive)
        {
            void* serializer = CSF_AT(void*, context, 0x04);
            CSF_AT(uint8_t, context, 0x15) = 0;
            CSF_AT(uint8_t, context, 0x16) = 0;
            const bool failed =
                (reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, void*)>(
                    CSF_AT(void**, serializer, 0)[0x48 / 4]))(
                    serializer, nullptr, &indirectValue, context) == -1;
            CSF_AT(uint8_t, context, 0x00) = failed;
            return -static_cast<int>(failed);
        }
    }
    else
    {
        const bool inactive = CSF_AT(uint8_t, context, 0x00) == 0;
        uint32_t result = 0;
        CSF_AT(uint32_t, context, 0x1C) = 0;
        if (inactive)
        {
            void* serializer = CSF_AT(void*, context, 0x04);
            CSF_AT(uint8_t, context, 0x15) = 0;
            CSF_AT(uint8_t, context, 0x16) = 0;
            CSF_AT(uint8_t, context, 0x00) =
                (reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, void*)>(
                    CSF_AT(void**, serializer, 0)[0x48 / 4]))(
                    serializer, nullptr, &result, context) == -1;
        }
        *value = result;
    }
    return -static_cast<int>(CSF_AT(uint8_t, context, 0x00) != 0);
}

HOOK(0x0054C750, TypeRegistry_SerializeIndirectValue, "auto");
