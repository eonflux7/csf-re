#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, Get_008b0678, 0x00525130, ());
CSF_FUNC(int, __fastcall, MenuMissionInfo_SetGlobalIndex, 0x00525DD0, (void* self, void* /*edx*/, int value));
// Pops nothing: the three pushed arguments stay on the stack for sub_4F4980 to consume.
CSF_FUNC(int, __cdecl, GetGlobal8B0558, 0x004F4920, (int a, int b, int c));
// __fastcall-style: ecx = self, three stack arguments popped by the callee (ret 12).
CSF_FUNC(int, __fastcall, sub_4F4980, 0x004F4980, (void* self, void* /*edx*/, int a, int b, int c));

static uint32_t MenuMission_ReadU32(uint32_t base, uint32_t offset)
{
    return *reinterpret_cast<uint32_t*>(base + offset);
}

static int __stdcall MenuMission_ApplyRecordsAndDispatch(int /*a1*/, int /*a2*/, uint32_t* a3)
{
    void* missionInfo = reinterpret_cast<void*>(Get_008b0678());
    MenuMissionInfo_SetGlobalIndex(missionInfo, nullptr, 92);

    // v3 = *(*a3 + 4)
    uint32_t v3 = MenuMission_ReadU32(a3[0], 4);
    // v4 = *(v3 + 0x1C)
    uint32_t v4 = MenuMission_ReadU32(v3, 0x1C);
    uint32_t v5 = v3 + 4;

    uint32_t v6;
    if (v4 < 0x10u)
        v6 = v5 + 4;
    else
        v6 = MenuMission_ReadU32(v5, 4);

    // v9 = (int)*(float*)(*(a3[2] + 4) + 4)
    float scaleFloat = *reinterpret_cast<float*>(MenuMission_ReadU32(a3[2], 4) + 4);
    int32_t v9 = static_cast<int32_t>(scaleFloat);

    // v8 = *(*(a3[1] + 4) + 4)
    int32_t v8 = static_cast<int32_t>(MenuMission_ReadU32(MenuMission_ReadU32(a3[1], 4), 4));

    void* dispatchTarget = reinterpret_cast<void*>(
        GetGlobal8B0558(static_cast<int>(v6), v8, v9));
    return sub_4F4980(dispatchTarget, nullptr, static_cast<int>(v6), v8, v9);
}

HOOK(0x0045D7E0, MenuMission_ApplyRecordsAndDispatch, "auto");
