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

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, MenuManager_SetInnerState, 0x0050E930, (int* self, void* , char a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(int, __fastcall, Sub_538190, 0x00538190, (int self, void* edx));

// Sets up the interface's instancing scene: enters inner menu state 1, caches whether
// the global value's string at +0x98 matches the byte constant at 0x82E65C, then runs
// the follow-up setup and marks the state as 3.
static int __fastcall S_00537cd0_SetupInstancingScene(S_00537cd0* self, void* /*edx*/)
{
    MenuManager_SetInnerState(reinterpret_cast<int*>(GetGlobalState_50D190()), nullptr, 1);

    int globalValue = GetGlobalValue_8B0568();
    uint8_t* globalValueBase = reinterpret_cast<uint8_t*>(globalValue);
    uint32_t sceneCount = CSF_AT(uint32_t, globalValueBase, 0xAC);

    self->field_0142 = CompareStringSubstring(
        reinterpret_cast<void*>(globalValueBase + 0x98),
        nullptr,
        0,
        sceneCount,
        reinterpret_cast<const char*>(CSF_ADDR_DATA(0x82E65C)),
        0) != 0;

    int result = Sub_538190(reinterpret_cast<int>(self), nullptr);
    self->field_0134 = 3;
    return result;
}

HOOK(0x538370, S_00537cd0_SetupInstancingScene, "auto");
