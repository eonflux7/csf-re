#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, S_0069d0f0_OpenKey, 0x0069D0F0,
         (void* self, void* edx));

using RegistryQueryProc = std::int32_t (__stdcall*)(
    void* key, const char* valueName, std::uint32_t* reserved,
    std::uint32_t* type, std::uint8_t* data, std::uint32_t* dataSize);
using RegistryQueryWithKeyRegisterProc = std::int32_t (__fastcall*)(
    void* ecx, void* edx,
    void* key, const char* valueName, std::uint32_t* reserved,
    std::uint32_t* type, std::uint8_t* data, std::uint32_t* dataSize);
using RegistryCloseProc = std::int32_t (__stdcall*)(void* key);

CSF_GLOBAL(RegistryQueryProc, g_RegistryQueryValue, 0x0082D014);
CSF_GLOBAL(RegistryCloseProc, g_RegistryCloseKey, 0x0082D000);

static char __fastcall S_0069d0f0_ReadValue(
    S_0069d0f0* self, void* /*edx*/,
    const char* valueName, std::uint8_t* data)
{
    if (!self->field_0000)
    {
        const char opened = S_0069d0f0_OpenKey(self, nullptr);
        if (!opened)
            return opened;
    }

    auto* valueType = &CSF_AT(std::uint32_t, data, 0x104);
    void* key = reinterpret_cast<void*>(self->field_0004);
    char success = 0;

    // Preserve the key in ECX at the first imported call while keeping
    // the original six stack arguments and callee-popped byte count.
    if (reinterpret_cast<RegistryQueryWithKeyRegisterProc>(
            g_RegistryQueryValue)(
            key, nullptr, key, valueName, nullptr,
            valueType, nullptr, nullptr) == 0)
    {
        switch (*valueType)
        {
        case 1:
        case 3:
            CSF_AT(std::uint32_t, data, 0x108) = 256;
            success = g_RegistryQueryValue(
                reinterpret_cast<void*>(self->field_0004),
                valueName, nullptr, valueType, data + 4,
                &CSF_AT(std::uint32_t, data, 0x108)) == 0;
            break;

        case 4:
            CSF_AT(std::uint32_t, data, 0x108) = 4;
            success = g_RegistryQueryValue(
                reinterpret_cast<void*>(self->field_0004),
                valueName, nullptr, valueType, data,
                &CSF_AT(std::uint32_t, data, 0x108)) == 0;
            break;
        }
    }

    if (self->field_0000)
    {
        g_RegistryCloseKey(reinterpret_cast<void*>(self->field_0004));
        self->field_0004 = 0;
        self->field_0000 = 0;
    }

    return success;
}

HOOK(0x0069D1B0, S_0069d0f0_ReadValue, "auto");
