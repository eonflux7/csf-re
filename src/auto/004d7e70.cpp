#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Bindings of the original functions this one calls, bound by address.
CSF_FUNC(void, __fastcall, TCharTask_BaseUpdate, 0x004DB0F0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, AiAutonomous_OwnerPassesReadyCheck, 0x004D7670, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiAutonomous_OwnerQueryMoveStatus, 0x004AEB90, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharTask_StartMovement, 0x004DB140, (void* self, void* edx, int a2));

// Updates the base task, then starts movement when the owner is ready and its move status allows it.
// Returns the low byte of the final result (0xFF when the move status query reports -1).
static char __fastcall AiAutonomous_CharTask_UpdateAndStartMovement(TCharTask* self, void* /*edx*/, int a2)
{
    TCharTask_BaseUpdate(self, nullptr, a2);

    C_0082f9f8* owner = reinterpret_cast<C_0082f9f8*>(self->field_0008);

    char result = static_cast<char>(AiAutonomous_OwnerPassesReadyCheck(owner, nullptr));
    if (!result) {
        return result;
    }

    int moveStatus = AiAutonomous_OwnerQueryMoveStatus(CSF_AT(void*, owner, 0x30C), nullptr);
    if (moveStatus == -1) {
        return static_cast<char>(moveStatus);
    }

    if (moveStatus == 0) {
        result = CSF_AT(char, owner, 0x33C);
        if (result) {
            return result;
        }
    }

    int movementParam = reinterpret_cast<int>(reinterpret_cast<uint8_t*>(owner) + 0x164);
    result = static_cast<char>(CharTask_StartMovement(owner, nullptr, movementParam));
    return result;
}

HOOK(0x004D7E70, AiAutonomous_CharTask_UpdateAndStartMovement, "auto");
