#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(char, __fastcall, Sub467CC0, 0x00467CC0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __fastcall, SceneFallbackLookup, 0x004686A0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneFindStateRecord, 0x004686F0, (void* self, void* edx, int a2, int a3));

static int __fastcall C_008313f8_ExecuteHandleCommand5035(
    C_008313f8* self, void* /*edx*/, int /*a2*/, int /*a3*/,
    std::uint32_t* arguments)
{
    const std::uint32_t handleAddress =
        CSF_AT(std::uint32_t, reinterpret_cast<void*>(arguments[0]), 0x04) + 4;
    void* handle = reinterpret_cast<void*>(handleAddress);

    if (CSF_AT(int, handle, 0x08) < 0)
        return static_cast<int>(handleAddress);

    const int resolvedAddress = CHandleTable_Resolve(
        g_pCurrentScene, nullptr, static_cast<int>(handleAddress));
    if (!resolvedAddress)
        return resolvedAddress;

    void* stateArgument = reinterpret_cast<void*>(
        CSF_AT(std::uint32_t, reinterpret_cast<void*>(arguments[1]), 0x04));
    const int stateId = CSF_AT(int, stateArgument, 0x04);
    void* stateTable = CSF_AT(void*, g_pCurrentScene, 0xBC);

    const int recordAddress = SceneFindStateRecord(
        stateTable, nullptr, resolvedAddress, stateId);
    if (!recordAddress)
    {
        stateArgument = reinterpret_cast<void*>(
            CSF_AT(std::uint32_t, reinterpret_cast<void*>(arguments[1]), 0x04));
        const int fallbackId = CSF_AT(int, stateArgument, 0x04);
        stateTable = CSF_AT(void*, g_pCurrentScene, 0xBC);
        (void)CSF_VTBL(0x004686A0);
        return SceneFallbackLookup(stateTable, nullptr, fallbackId);
    }

    void* record = reinterpret_cast<void*>(recordAddress);
    int result = CSF_AT(int, record, 0x18);
    if (result)
        return result;

    (void)CSF_VTBL(0x00467CC0);
    Sub467CC0(record, nullptr, 0);
    result = CSF_AT(int, record, 0x18);
    if (result)
    {
        CSF_AT(void*, self, 0x70) = record;
        CSF_AT(int, self, 0x18) = 3;
        CSF_AT(std::uint8_t, self, 0x82) = 0;

        void* resolvedObject = reinterpret_cast<void*>(resolvedAddress);
        CSF_AT(std::uint32_t, self, 0x60) =
            CSF_AT(std::uint32_t, resolvedObject, 0x20);
        CSF_AT(std::uint32_t, self, 0x64) =
            CSF_AT(std::uint32_t, resolvedObject, 0x24);
    }
    return result;
}

HOOK(0x00468250, C_008313f8_ExecuteHandleCommand5035, "auto");
