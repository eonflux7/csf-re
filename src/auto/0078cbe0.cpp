#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(uint32_t*, __fastcall, S_00692b10_InitializeFile, 0x00692B10, (S_00692b10* self, void*));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuExtras_LoadFile, 0x00694770, (void* self, void* edx, const char* path));
CSF_FUNC(int, __cdecl, NetPerfCounters_ClearEntries, 0x0078BE10, (int a1, int a2, int a3, int a4));
CSF_FUNC(char, __fastcall, NetPerfCounters_LoadVersion1, 0x0078CA00, (void* self, void* edx, void* a2));

CSF_GLOBAL(const char*, g_pszKey_VERSION_net_perf_counters, 0x008A9DA4);
static const auto kFileInitializerVtable = CSF_VTBL(0x00692B10);

static char __fastcall C_00872abc_LoadPerfCounters(
    C_00872abc* self, void* /*edx*/, int pathAddress)
{
    auto* entries = reinterpret_cast<std::uint8_t*>(self->field_0004);
    const int oldBuffer = CSF_AT(int, entries, 0x4);

    if (oldBuffer)
    {
        NetPerfCounters_ClearEntries(
            oldBuffer, CSF_AT(int, entries, 0x8),
            reinterpret_cast<int>(entries), pathAddress);

        if (CSF_AT(int, entries, 0x4))
            Mem_FreeDebug(CSF_AT(void*, entries, 0x4));
    }

    CSF_AT(int, entries, 0x4) = 0;
    CSF_AT(int, entries, 0x8) = 0;
    CSF_AT(int, entries, 0xC) = 0;

    alignas(8) std::uint8_t fileBytes[0x3C];
    S_00692b10& file = *reinterpret_cast<S_00692b10*>(fileBytes);
    S_00692b10_InitializeFile(&file, nullptr);

    char result = 0;
    if (MenuExtras_LoadFile(
            &file, nullptr, reinterpret_cast<const char*>(pathAddress)))
    {
        auto* root = reinterpret_cast<std::uint8_t*>(file.field_0000);
        const int version = ReadIntegerAttribute(
            root, nullptr,
            const_cast<char*>(g_pszKey_VERSION_net_perf_counters));

        if (version <= 1)
        {
            if (version == 1)
                result = NetPerfCounters_LoadVersion1(self, nullptr, root);
        }
    }

    MenuExtras_CloseFile(&file, nullptr);
    return result;
}

HOOK(0x0078CBE0, C_00872abc_LoadPerfCounters, "auto");
