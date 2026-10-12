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

CSF_FUNC(bool, __fastcall, AutoTask_CheckTarget, 0x0048C720, (void* self, void* edx, int a2, int a3));
CSF_FUNC(void, __fastcall, Game_SetPendingRequest, 0x00496CD0, (void* self, void* , void* request));

// Builds a pending request record from the target-check result and hands it to the game.
static void __fastcall C_0083ab58_IssueTargetCheckRequest(C_0083ab58* self, void* /*edx*/)
{
    C_00833908* v1 = reinterpret_cast<C_00833908*>(self->field_0014);
    uint32_t* v2 = &self->field_0044;

    alignas(8) uint8_t requestBytes[0x24];

    CSF_AT(int, requestBytes, 0x0C) = -1;
    CSF_AT(int, requestBytes, 0x10) = -1;
    CSF_AT(int, requestBytes, 0x00) = 1;
    CSF_AT(int, requestBytes, 0x04) = 0;
    CSF_AT(void*, requestBytes, 0x08) = CSF_VTBL(0x0082D448);
    CSF_AT(int, requestBytes, 0x14) = 0;
    CSF_AT(int, requestBytes, 0x18) = 0;
    CSF_AT(int, requestBytes, 0x1C) = 0;
    CSF_AT(uint8_t, requestBytes, 0x20) = 1;
    CSF_AT(uint8_t, requestBytes, 0x21) = 0;
    CSF_AT(uint8_t, requestBytes, 0x22) = 1;

    const bool targetOk = AutoTask_CheckTarget(
        reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(self) + 0x50),
        nullptr,
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(v1) + 0x1c),
        reinterpret_cast<int>(v2));

    // Fields are re-read after the call, as in the original.
    CSF_AT(uint32_t, requestBytes, 0x0C) = v2[1];
    CSF_AT(uint32_t, requestBytes, 0x10) = v2[2];

    CSF_AT(uint8_t, requestBytes, 0x20) = 1;
    CSF_AT(uint8_t, requestBytes, 0x21) = 0;
    CSF_AT(uint8_t, requestBytes, 0x22) = 1;

    CSF_AT(int, requestBytes, 0x04) = (targetOk ? 1 : 0) + 1;

    Game_SetPendingRequest(reinterpret_cast<void*>(v1), nullptr, requestBytes);
}

HOOK(0x004CD870, C_0083ab58_IssueTargetCheckRequest, "auto");
