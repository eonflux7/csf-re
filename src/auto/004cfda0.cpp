#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0083b580_SetTargetObject, 0x004CFA70, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_Unknown_ResetTwoDwordsToMinusOne, 0x004CFAA0, (void* self, void*));
CSF_FUNC(uint32_t*, __fastcall, C_0083b580_GetTargetAssignment, 0x004CFAB0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __stdcall, NetSession_FindPlayerById, 0x0073E2C0, (uint32_t playerId));
CSF_FUNC(int, __fastcall, GetValueIfNotFlagged, 0x007408C0, (void* self, void*));

static uint32_t* __fastcall C_0083b580_UpdateTargetAssignment(
    C_0083b580* self, void* /*edx*/)
{
    alignas(8) uint8_t assignmentBytes[0xC];
    auto* assignment = reinterpret_cast<uint32_t*>(assignmentBytes);

    auto* owner = reinterpret_cast<C_00833908*>(self->field_0014);
    uint8_t* targetBase = nullptr;
    if (owner)
    {
        const uint32_t target = CSF_AT(uint32_t, owner, 0x18);
        if (target)
            targetBase = reinterpret_cast<uint8_t*>(target - 128);
    }

    if (CSF_AT(int32_t, targetBase, 0x5C) == -1)
    {
        uint32_t* result =
            C_0083b580_GetTargetAssignment(self, nullptr, assignment);
        if (result[1] != static_cast<uint32_t>(-1))
        {
            return reinterpret_cast<uint32_t*>(
                C_Unknown_ResetTwoDwordsToMinusOne(self, nullptr));
        }
        return result;
    }

    if (C_0083b580_GetTargetAssignment(self, nullptr, assignment)[1]
        != static_cast<uint32_t>(-1))
    {
        C_Unknown_ResetTwoDwordsToMinusOne(self, nullptr);
    }

    const uint32_t playerId = CSF_AT(uint32_t, targetBase, 0x5C);
    GetGlobalDword_73C020();
    const int player = NetSession_FindPlayerById(playerId);
    if (!player)
        return nullptr;

    const int value = GetValueIfNotFlagged(
        reinterpret_cast<void*>(player), nullptr);
    const int targetObject = value
        ? static_cast<int>(static_cast<uint32_t>(value) - 20 + 28)
        : 28;

    return reinterpret_cast<uint32_t*>(
        C_0083b580_SetTargetObject(self, nullptr, targetObject));
}

HOOK(0x004CFDA0, C_0083b580_UpdateTargetAssignment, "auto");
