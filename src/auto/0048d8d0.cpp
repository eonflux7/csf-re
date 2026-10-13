#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, StateRecord_Reset, 0x00401870, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetCachedProgressBarControl, 0x0050E220, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_558150, 0x00558150, ());

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall CharActionFsm_EnterLinkedActionState39(void* self, void* /*edx*/)
{
    void* owner = CSF_AT(void*, self, 0);
    CSF_AT(int, self, 0x04) = -1;
    void* actionData = CSF_AT(void*, owner, 0x08);
    auto* linkedHandle = reinterpret_cast<uint8_t*>(self) + 0x10;

    CSF_AT(uint32_t, linkedHandle, 0x04) = CSF_AT(uint32_t, actionData, 0x2B8);
    CSF_AT(uint32_t, linkedHandle, 0x08) = CSF_AT(uint32_t, actionData, 0x2BC);

    int result = CSF_AT(int, self, 0x18);
    if (result < 0)
        return result;

    result = CHandleTable_Resolve(
        g_pCurrentScene, nullptr, reinterpret_cast<int>(linkedHandle));
    if (!result)
        return result;

    if (CSF_AT(int, reinterpret_cast<void*>(result), 0x50) != 18)
        return result;

    result = CSF_AT(int, reinterpret_cast<void*>(result), 0x18);
    if (!result)
        return result;

    result = static_cast<int>(static_cast<uint32_t>(result) - 1712);
    if (!result)
        return result;

    float duration;
    if (CSF_AT(uint8_t, reinterpret_cast<void*>(result), 0x458) == 1)
        duration = CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_558150()), 0x198);
    else
        duration = CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_558150()), 0x19C);

    CSF_AT(float, self, 0x0C) = duration;
    CSF_AT(float, self, 0x08) = duration;

    alignas(8) uint8_t recordBytes[0x34];
    C_0082d478& record = *reinterpret_cast<C_0082d478*>(recordBytes);
    StateRecord_Reset(&record, nullptr, 31);
    record.field_000c = CSF_AT(uint32_t, linkedHandle, 0x04);
    record.field_0010 = CSF_AT(uint32_t, linkedHandle, 0x08);

    int dispatcher = GetGlobalObjectField184();
    DispatchRecordToGlobalObject(reinterpret_cast<void*>(dispatcher), nullptr, &record, 0);

    int globalState = GetGlobalState_50D190();
    GetCachedProgressBarControl(reinterpret_cast<void*>(globalState), nullptr);
    CSF_AT(uint8_t, self, 0x30) = 1;

    CSF_AT(uint32_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xF8) = 1194341944;
    result = GetGlobalState_50D190();
    CSF_AT(uint32_t, reinterpret_cast<void*>(result), 0xFC) = 0;
    return result;
}

HOOK(0x0048D8D0, CharActionFsm_EnterLinkedActionState39, "auto");
