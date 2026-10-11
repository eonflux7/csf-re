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

// Inherited from TCharTask: starts a movement toward the sub-object at +0x1E0.
CSF_FUNC(int, __fastcall, CharTask_StartMovement, 0x004DB140, (void* self, void* edx, int a2));

// Attack routine that takes the movement result as `this`; three stack arguments (ret 12).
CSF_FUNC(int, __fastcall, AttackTaskRunWithTarget, 0x0047E720, (void* self, void* edx, void* arg0, unsigned int a3, int a4));

// Starts movement via the embedded sub-object at +0x1E0, runs the attack with the
// caller's three arguments, and marks the flag at +0x2ED as set.
static int __fastcall C_0082f9f8_StartMovementAndAttack(C_0082f9f8* self, void* /*edx*/, void* arg0, unsigned int a3, int a4)
{
    uint8_t* movementSubObject = reinterpret_cast<uint8_t*>(self) + 0x1E0;
    void* started = reinterpret_cast<void*>(static_cast<uintptr_t>(
        CharTask_StartMovement(self, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(movementSubObject)))));

    int result = AttackTaskRunWithTarget(started, nullptr, arg0, a3, a4);

    CSF_AT(uint8_t, self, 0x2ED) = 1;
    return result;
}

HOOK(0x0047F8F0, C_0082f9f8_StartMovementAndAttack, "auto");
