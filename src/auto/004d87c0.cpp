#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Forwarder 0x4DB360: this function passes ecx=this and one stack argument (a2);
// the target pops its arguments, so it is bound __fastcall with one stack parameter.
CSF_FUNC(int, __fastcall, InnerForward_4DB360, 0x004DB360, (void* self, void* edx, int a2));

// CharTask::StartMovement (0x4DB140): __thiscall with one stack argument, returns the new task pointer.
CSF_FUNC(int, __fastcall, CharTask_StartMovement, 0x004DB140, (void* self, void* edx, int a2));

// Sub-object reset helper (0x4D7340): __fastcall with no stack arguments, returns char (ignored here).
CSF_FUNC(char, __fastcall, InnerSubobject_UpdateCachedState, 0x004D7340, (void* self, void* edx));

// Starts the movement task on this character: forwards a2 through 0x4DB360, checks the
// owner's state (vtable slot 32 == 15 means "already in that state"), then creates a
// movement task, clears bit 1 of the inner object's flags, copies two values from a2,
// updates the inner sub-object, and starts a second task on the new object.
static int __fastcall C_0083dbb0_StartMovementTask(C_0083dbb0* self, void* /*edx*/, int a2)
{
    InnerForward_4DB360(self, nullptr, a2);

    uint32_t ownerAddr = CSF_AT(uint32_t, self, 0x10);
    if (ownerAddr != 0) {
        void* owner = reinterpret_cast<void*>(static_cast<uintptr_t>(ownerAddr));
        int ownerResult = reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[128 / 4])(owner, nullptr);
        if (ownerResult == 15) {
            return ownerResult;
        }
    }

    uint32_t startedAddr = static_cast<uint32_t>(CharTask_StartMovement(
        reinterpret_cast<void*>(self), nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(reinterpret_cast<uint8_t*>(self) + 0x1A0))));
    uint8_t* started = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(startedAddr));

    uint32_t innerAddr = CSF_AT(uint32_t, started, 8);
    uint8_t* inner = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(innerAddr));
    CSF_AT(uint32_t, inner, 0x18) &= ~2u;

    uint8_t* args = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(a2)));
    CSF_AT(uint32_t, started, 0x7C) = CSF_AT(uint32_t, args, 4);
    CSF_AT(uint32_t, started, 0x80) = CSF_AT(uint32_t, args, 8);

    InnerSubobject_UpdateCachedState(
        reinterpret_cast<void*>(static_cast<uintptr_t>(innerAddr + 0x224)), nullptr);

    return CharTask_StartMovement(
        reinterpret_cast<void*>(static_cast<uintptr_t>(startedAddr)), nullptr,
        static_cast<int>(startedAddr + 0x14));
}

HOOK(0x004D87C0, C_0083dbb0_StartMovementTask, "auto");
