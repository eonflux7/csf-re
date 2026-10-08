#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Original calls: GetGlobalValue_8B056C (0x506BD0, __cdecl, no args) and
// sub_53D740 (0x53D740, __fastcall-style: ecx=this, edi passed through, one stack arg).
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x506BD0, ());
CSF_FUNC(int, __fastcall, CMenuMissionInfo_EnterPanelState, 0x53D740, (void* self, void* edx, int a3));

// Refreshes the mission info panel when the global game state is in mode 1 with
// its secondary flag clear and the selected record flag set.
static int __fastcall CMenuMissionInfo_RefreshOnBriefingReady(void* self, void* /*edx*/, int /*a3*/)
{
    uint8_t* self_bytes = reinterpret_cast<uint8_t*>(self);

    // v4 = *(GetGlobalValue() + 0x1c)
    uint8_t* global_root = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalValue_8B056C()));
    uint32_t v4 = *reinterpret_cast<uint32_t*>(global_root + 0x1c);

    uint8_t* state_base = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(v4));
    uint32_t v5 = *reinterpret_cast<uint32_t*>(state_base + 0x1d0);

    // result = v4 + 0x1d0 (address), returned unless the branch below overwrites it
    uint32_t result = v4 + 0x1d0;

    if (v5 == 1 && *reinterpret_cast<uint32_t*>(state_base + 0x1d4) == 0)
    {
        // result = *(self + 0x10)
        result = *reinterpret_cast<uint32_t*>(self_bytes + 0x10);
        uint8_t* info = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(result));
        if (*(info + 4) != 0)
        {
            return CMenuMissionInfo_EnterPanelState(self_bytes - 4, nullptr, 1);
        }
    }
    return static_cast<int>(result);
}

HOOK(0x0053D7F0, CMenuMissionInfo_RefreshOnBriefingReady, "auto");
