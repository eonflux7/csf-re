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

CSF_FUNC(uint32_t*, __fastcall, sub_4CFDA0, 0x004CFDA0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_4CFE50, 0x004CFE50, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(int, __fastcall, C_00844694_ForwardSpawnRequest, 0x00551680, (C_00844694* self, void* , int a2, int a3, int a4, int a5));

static char __fastcall C_0083b580_SyncStateAndForwardSpawn(C_0083b580* self, void* /*edx*/, int /*a2*/)
{
    // Resolve the shared state block: field_0014 -> +0x18 -> state base (offset -0x80).
    uint8_t* state = nullptr;
    uint32_t ownerField14 = self->field_0014;
    if (ownerField14 != 0)
    {
        uint32_t ownerField18 = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(ownerField14), 0x18);
        if (ownerField18 != 0)
            state = reinterpret_cast<uint8_t*>(ownerField18 - 0x80);
    }

    // Mirror the desired flag into the state block when it differs.
    uint8_t desiredFlag = static_cast<uint8_t>(self->field_0044);
    if (desiredFlag != CSF_AT(uint8_t, state, 0x4A))
        CSF_AT(uint8_t, state, 0x4A) = desiredFlag;

    sub_4CFDA0(self, nullptr);
    sub_4CFE50(self, nullptr);

    char result = static_cast<char>(self->field_0058);
    if (result)
    {
        if (CSF_AT(uint8_t, state, 0x47) != 1)
            CSF_AT(uint8_t, state, 0x47) = 1;

        int spawnKindArg = 0x53423036 + (CSF_AT(uint8_t, state, 0x50) != 1 ? 1 : 0);
        C_00844694* forwardTarget = reinterpret_cast<C_00844694*>(GetGlobal8B0688());
        return static_cast<char>(C_00844694_ForwardSpawnRequest(forwardTarget, nullptr, spawnKindArg, -1, -1, -1));
    }
    return result;
}

HOOK(0x004CFFD0, C_0083b580_SyncStateAndForwardSpawn, "auto");
