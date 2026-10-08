#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x50D190, (void));
CSF_FUNC(int, __fastcall, Sub_538190, 0x538190, (int self, void* edx));

// 0x53D630: fetch the global state singleton, read the dword at +0x18 of it
// and forward that value as `this` to sub_538190 (tail call).
static int __fastcall CMenuMissionInfo_ProcessGlobalState(CMenuMissionInfo* /*self*/, void* /*edx*/)
{
    int globalState = GetGlobalState_50D190();
    return Sub_538190(static_cast<int>(CSF_AT(uint32_t, reinterpret_cast<void*>(globalState), 0x18)), nullptr);
}

HOOK(0x53D630, CMenuMissionInfo_ProcessGlobalState, "auto");
