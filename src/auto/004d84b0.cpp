#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

// Bound callees (addresses from the packet).
CSF_FUNC(void, __fastcall, TCharTask_BaseUpdate, 0x004DB0F0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, IsMovementAllowed, 0x004D7670, (void* self, void* edx));
CSF_FUNC(int, __fastcall, QueryMovementResult, 0x004AEB90, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharTask_StartMovement, 0x004DB140, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, AiCoverAttackTasks_UpdateCoverState, 0x004D1120, (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

// Original: void __thiscall sub_4D84B0(TCharTask* this, int a2); ret 4
static void __fastcall TCharTask_UpdateStateAndMovement(void* self, void* edx, int a2) {
    (void)edx;
    TCharTask_BaseUpdate(self, nullptr, a2);

    uint32_t v3 = CSF_AT(uint32_t, self, 0x08);
    uint8_t* owner = reinterpret_cast<uint8_t*>(v3);
    if (IsMovementAllowed(owner, nullptr)) {
        int movementResult = QueryMovementResult(CSF_AT(void*, owner, 0x30C), nullptr);
        if (movementResult != -1 &&
            (movementResult != 0 || !CSF_AT(uint8_t, owner, 0x33C))) {
            CharTask_StartMovement(owner, nullptr,
                                   static_cast<int>(reinterpret_cast<uintptr_t>(owner + 0x164)));
        }
    }

    // a2 is passed to the cover-state update as a float (bit pattern preserved).
    float deltaTime;
    std::memcpy(&deltaTime, &a2, sizeof(deltaTime));
    AiCoverAttackTasks_UpdateCoverState(owner + 0x330, nullptr, deltaTime);

    // The owner pointer is reloaded from this->field_0008 after the cover call.
    uint8_t* ownerAgain = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x08));
    if (CSF_AT(uint8_t, ownerAgain, 0x330)) {
        if (CSF_AT(float, ownerAgain, 0x334) > CSF_AT(float, ownerAgain, 0x338)) {
            CharTask_LookupByOffset(self, nullptr,
                                    static_cast<int>(CSF_AT(uint32_t, self, 0x0C) + 0x50));
        }
    }
}

HOOK(0x4D84B0, TCharTask_UpdateStateAndMovement, "auto");
