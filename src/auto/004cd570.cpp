#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(unsigned int, __fastcall, AttackTasks_QueueShot, 0x0048C630, (void* self, void* edx, int a2, float a3, float a4, int a5, int a6));

// Queues an attack shot using the coordinates and parameters stored in the
// four global-value objects (offsets 0x1d80..0x1d8c), with the queue
// subobject at this+0x50 as the receiver. Returns the queue result.
static unsigned int __fastcall C_0083ab58_QueueAttackShot(C_0083ab58* self, void* /*edx*/)
{
    void* globalFirst  = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalValue_8BC1E0()));
    void* globalSecond = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalValue_8BC1E0()));
    void* globalThird  = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalValue_8BC1E0()));
    void* globalFourth = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalValue_8BC1E0()));

    void* queue = reinterpret_cast<uint8_t*>(self) + 0x50;

    return AttackTasks_QueueShot(
        queue,
        nullptr,
        0,
        CSF_AT(float, globalFourth, 0x1d80),
        CSF_AT(float, globalThird, 0x1d84),
        CSF_AT(int, globalSecond, 0x1d88),
        CSF_AT(int, globalFirst, 0x1d8c));
}

HOOK(0x004CD570, C_0083ab58_QueueAttackShot, "auto");
