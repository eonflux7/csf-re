#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, CoreErrorLog_CombineRegistryPath, 0x0069D000, (void* a1, void* a2, uint32_t* a3));

using RegistryCloseKeyFn = std::int32_t (__fastcall*)(
    S_0069d0f0* context, void* edx, void* key);
using RegistryCreateKeyFn = std::int32_t (__fastcall*)(
    std::uint32_t* context, void* edx,
    void* root, const char* subkey, std::uint32_t reserved,
    char* className, std::uint32_t options, std::uint32_t access,
    void* security, std::uint32_t* key, std::uint32_t* disposition);

CSF_GLOBAL(RegistryCloseKeyFn, g_RegCloseKey, 0x0082D000);
CSF_GLOBAL(RegistryCreateKeyFn, g_RegCreateKeyExA, 0x0082D00C);

static std::uint8_t __fastcall S_0069d0f0_OpenRegistryKey(
    S_0069d0f0* self, void* /*edx*/)
{
    if (self->field_0000)
    {
        g_RegCloseKey(
            self, nullptr, reinterpret_cast<void*>(self->field_0004));
        self->field_0004 = 0;
        self->field_0000 = 0;
    }

    alignas(8) std::uint8_t combinedBytes[0x1C];
    alignas(8) std::uint8_t pathBytes[0x1C];
    auto& combined = *reinterpret_cast<C_0082d410*>(combinedBytes);
    auto& path = *reinterpret_cast<C_0082d410*>(pathBytes);

    CSF_AT(std::uint32_t, pathBytes, 0x18) = 15;
    CSF_AT(std::uint32_t, pathBytes, 0x14) = 0;
    *reinterpret_cast<std::uint8_t*>(&path.field_0004) = 0;

    const int source = CoreErrorLog_CombineRegistryPath(
        &combined,
        reinterpret_cast<std::uint8_t*>(self) + 8,
        reinterpret_cast<std::uint32_t*>(
            reinterpret_cast<std::uint8_t*>(self) + 0x24));

    AssignStringSubstring(
        &path, nullptr, reinterpret_cast<const void*>(source), 0,
        static_cast<std::uint32_t>(-1));

    if (CSF_AT(std::uint32_t, combinedBytes, 0x18) >= 0x10 &&
        combined.field_0004)
    {
        Mem_FreeDebug(reinterpret_cast<void*>(combined.field_0004));
    }

    const char* subkey = reinterpret_cast<const char*>(path.field_0004);
    if (CSF_AT(std::uint32_t, pathBytes, 0x18) < 0x10)
        subkey = reinterpret_cast<const char*>(&path.field_0004);

    if (g_RegCreateKeyExA(
            &self->field_0004, nullptr,
            reinterpret_cast<void*>(0x80000001u), subkey, 0, nullptr,
            0, 0xF003Fu, nullptr, &self->field_0004, nullptr) == 0)
    {
        self->field_0000 = 1;
    }

    const std::uint8_t opened = self->field_0000;
    if (CSF_AT(std::uint32_t, pathBytes, 0x18) >= 0x10 &&
        path.field_0004)
    {
        Mem_FreeDebug(reinterpret_cast<void*>(path.field_0004));
    }

    return opened;
}

HOOK(0x0069D0F0, S_0069d0f0_OpenRegistryKey, "auto");
