#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, Registry_OpenKey, 0x0069D0F0,
         (void* self, void* edx));

// The first two parameters reproduce the registers prepared at the
// indirect call. The remaining parameters retain the Win32 stack ABI.
using RegSetValueExAFunction = int32_t (__fastcall*)(
    const void* ecx, const void* edx,
    void* key, const char* valueName, uint32_t reserved,
    uint32_t type, const uint8_t* data, uint32_t size);
using RegCloseKeyFunction = int32_t (__fastcall*)(
    void* ecx, void* edx, void* key);

CSF_GLOBAL(RegSetValueExAFunction, g_RegSetValueExA, 0x0082D008);
CSF_GLOBAL(RegCloseKeyFunction, g_RegCloseKey, 0x0082D000);

static char __fastcall Registry_WriteValue(
    void* self, void* /*edx*/, const char* valueName,
    uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3,
    uint32_t d4, uint32_t d5, uint32_t d6, uint32_t d7,
    uint32_t d8, uint32_t d9, uint32_t d10, uint32_t d11,
    uint32_t d12, uint32_t d13, uint32_t d14, uint32_t d15,
    uint32_t d16, uint32_t d17, uint32_t d18, uint32_t d19,
    uint32_t d20, uint32_t d21, uint32_t d22, uint32_t d23,
    uint32_t d24, uint32_t d25, uint32_t d26, uint32_t d27,
    uint32_t d28, uint32_t d29, uint32_t d30, uint32_t d31,
    uint32_t d32, uint32_t d33, uint32_t d34, uint32_t d35,
    uint32_t d36, uint32_t d37, uint32_t d38, uint32_t d39,
    uint32_t d40, uint32_t d41, uint32_t d42, uint32_t d43,
    uint32_t d44, uint32_t d45, uint32_t d46, uint32_t d47,
    uint32_t d48, uint32_t d49, uint32_t d50, uint32_t d51,
    uint32_t d52, uint32_t d53, uint32_t d54, uint32_t d55,
    uint32_t d56, uint32_t d57, uint32_t d58, uint32_t d59,
    uint32_t d60, uint32_t d61, uint32_t d62, uint32_t d63,
    uint32_t d64, uint32_t d65, uint32_t d66)
{
    (void)CSF_VTBL(0x0069D0F0);

    auto* registry = static_cast<S_0069d0f0*>(self);
    if (!registry->field_0000)
    {
        const char opened = Registry_OpenKey(registry, nullptr);
        if (!opened)
            return opened;
    }

    uint32_t record[] = {
        d0, d1, d2, d3, d4, d5, d6, d7,
        d8, d9, d10, d11, d12, d13, d14, d15,
        d16, d17, d18, d19, d20, d21, d22, d23,
        d24, d25, d26, d27, d28, d29, d30, d31,
        d32, d33, d34, d35, d36, d37, d38, d39,
        d40, d41, d42, d43, d44, d45, d46, d47,
        d48, d49, d50, d51, d52, d53, d54, d55,
        d56, d57, d58, d59, d60, d61, d62, d63,
        d64, d65, d66
    };

    const auto* data = reinterpret_cast<const uint8_t*>(&record[1]);
    char succeeded = 0;
    switch (record[65])
    {
    case 1:
        record[66] = static_cast<uint32_t>(
            std::strlen(reinterpret_cast<const char*>(data)) + 1);
        succeeded = g_RegSetValueExA(
            data, valueName,
            reinterpret_cast<void*>(registry->field_0004),
            valueName, 0, 1, data, record[66]) == 0;
        break;

    case 3:
        succeeded = g_RegSetValueExA(
            data, valueName,
            reinterpret_cast<void*>(registry->field_0004),
            valueName, 0, 3, data, record[66]) == 0;
        break;

    case 4:
    {
        void* key = reinterpret_cast<void*>(registry->field_0004);
        record[66] = 4;
        succeeded = g_RegSetValueExA(
            valueName, key, key, valueName, 0, 4,
            reinterpret_cast<const uint8_t*>(&record[0]), 4) == 0;
        break;
    }
    }

    if (registry->field_0000)
    {
        void* key = reinterpret_cast<void*>(registry->field_0004);
        g_RegCloseKey(key, nullptr, key);
        registry->field_0004 = 0;
        registry->field_0000 = 0;
    }
    return succeeded;
}

HOOK(0x0069D290, Registry_WriteValue, "auto");
