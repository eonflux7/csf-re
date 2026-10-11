#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// CharTask_StartMovement: __thiscall(this, a2), returns the task object that now moves.
CSF_FUNC(int, __fastcall, CharTask_StartMovement, 0x004DB140, (void* self, void* edx, int a2));

// Second callee: __thiscall(this, a2) on the movement-task object, returns int.
CSF_FUNC(int, __fastcall, sub_47D3F0, 0x0047D3F0, (void* self, void* edx, int a2));

// Starts movement on the embedded sub-object at +0x114, forwards a2 to the
// movement task, stores the byte argument at +0x2A8 and sets the flag at +0x2ED.
static int __fastcall C_0082f9f8_StartMovementAndSetFlag(C_0082f9f8* self, void* /*edx*/, int a2, uint8_t a3)
{
    // The sub-object address is this + 0x114 (passed by value as an int argument).
    int subObjectAddress = static_cast<int>(reinterpret_cast<uintptr_t>(reinterpret_cast<uint8_t*>(self) + 0x114));
    int startedValue = CharTask_StartMovement(self, nullptr, subObjectAddress);
    C_0082f9f8* started = reinterpret_cast<C_0082f9f8*>(static_cast<uintptr_t>(static_cast<uint32_t>(startedValue)));

    int result = sub_47D3F0(started, nullptr, a2);

    CSF_AT(uint8_t, self, 0x2A8) = a3;
    CSF_AT(uint8_t, self, 0x2ED) = 1;
    return result;
}

HOOK(0x0047E0C0, C_0082f9f8_StartMovementAndSetFlag, "auto");
