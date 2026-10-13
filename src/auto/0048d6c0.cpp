#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetCachedProgressBarControl, 0x0050E220, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_558150, 0x00558150, ());
CSF_FUNC(char, __fastcall, C_00858180_SetRequestedValue, 0x0073BD20, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, GetSpawnFlagFromGlobal, 0x0073CD30, (void* self, void*));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static std::uint32_t* __fastcall CharActionFsm_EnterLinkedActionState41(
    void* self, void* /*edx*/)
{
    void* owner = CSF_AT(void*, self, 0);
    void* linkedAction = CSF_AT(void*, owner, 8);

    CSF_AT(std::uint32_t, self, 0x28) =
        CSF_AT(std::uint32_t, linkedAction, 0x2B8);
    CSF_AT(std::uint32_t, self, 0x2C) =
        CSF_AT(std::uint32_t, linkedAction, 0x2BC);

    auto* result = reinterpret_cast<std::uint32_t*>(
        CSF_AT(std::uint32_t, self, 0x2C));
    if (CSF_AT(std::int32_t, self, 0x2C) < 0)
        return result;

    void* globalValue = reinterpret_cast<void*>(GetGlobalValue_558150());
    const float time = CSF_AT(float, globalValue, 0x1B4);
    CSF_AT(float, self, 0x1C) = time;
    CSF_AT(float, self, 0x20) = time;

    void* handle = static_cast<std::uint8_t*>(self) + 0x24;
    result = reinterpret_cast<std::uint32_t*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr,
                             reinterpret_cast<int>(handle)));
    if (!result)
        return result;
    if (CSF_AT(int, result, 0x50) != 32)
        return result;

    result = CSF_AT(std::uint32_t*, result, 0x18);
    if (!result)
        return result;

    void* control = reinterpret_cast<void*>(
        reinterpret_cast<std::uintptr_t>(result) - 0x80);
    if (!control)
        return result;

    void* spawnGlobals = reinterpret_cast<void*>(GetGlobalDword_73C020());
    const int requestedValue = GetSpawnFlagFromGlobal(spawnGlobals, nullptr);
    C_00858180_SetRequestedValue(control, nullptr, requestedValue);

    void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
    GetCachedProgressBarControl(state, nullptr);

    CSF_AT(std::uint8_t, self, 0x30) = 1;
    state = reinterpret_cast<void*>(GetGlobalState_50D190());
    CSF_AT(std::uint32_t, state, 0xF8) = 0x47303730;

    result = reinterpret_cast<std::uint32_t*>(GetGlobalState_50D190());
    CSF_AT(std::uint32_t, result, 0xFC) = 0;
    return result;
}

HOOK(0x0048D6C0, CharActionFsm_EnterLinkedActionState41, "auto");
