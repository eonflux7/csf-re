#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390,
         (void* self, void* edx));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160,
         (void* self, void* edx));

static int __fastcall MenuManager_InitializeCachedInterface(
    void* self, void* /*edx*/)
{
    int result = CSF_AT(int, self, 0x34);
    if (result != 0)
        return result;

    void* provider = MenuControls_ResolveObject(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);

    result = ((int(__fastcall*)(void*, void*))
        CSF_AT(void**, provider, 0)[0x48 / 4])(provider, nullptr);

    CSF_AT(int, self, 0x34) = result;
    if (result == 0)
        return result;

    provider = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);

    void* source = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, provider, 0)[0x18 / 4])(provider, nullptr);
    const std::uint32_t firstValue = CSF_AT(std::uint32_t, source, 0);

    provider = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);

    void* getPair = CSF_AT(void**, provider, 0)[0x18 / 4];
    std::uint32_t values[2];
    values[0] = firstValue;
    source = ((void*(__fastcall*)(void*, void*))getPair)(provider, nullptr);

    const std::uint32_t secondValue = CSF_AT(std::uint32_t, source, 4);
    void* cachedInterface = reinterpret_cast<void*>(
        CSF_AT(int, self, 0x34));
    values[1] = secondValue;

    return ((int(__fastcall*)(void*, void*, std::uint32_t*))
        CSF_AT(void**, cachedInterface, 0)[0x24 / 4])(
            cachedInterface, nullptr, values);
}

HOOK(0x0050C030, MenuManager_InitializeCachedInterface, "auto");
