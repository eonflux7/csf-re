#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Game heap allocator (__cdecl, caller pops nothing).
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Constructor-style initializer for the 136-byte atomic object (__thiscall, one arg).
CSF_FUNC(uint32_t*, __fastcall, sub_6F4CD0, 0x006F4CD0, (void* self, void* edx));

// Registration/update routine taking the new object (__thiscall, one arg, ret 4).
CSF_FUNC(uint32_t*, __fastcall, sub_6CA030, 0x006CA030, (void* self, void* edx, uint32_t* a2));

static uint32_t* __fastcall C_00862748_AllocateAndRegisterClone(C_00862748* self, void* /*edx*/)
{
    constexpr std::size_t kAtomicObjectSize = 0x88;  // 136 bytes

    uint32_t* cloned = nullptr;
    void* block = Mem_Alloc(kAtomicObjectSize);
    if (block)
        cloned = sub_6F4CD0(block, nullptr);

    // Copy the three state fields from the source atomic into the new object.
    cloned[11] = self->field_002c;
    cloned[12] = self->field_0030;
    cloned[14] = self->field_0038;

    sub_6CA030(reinterpret_cast<C_00862598*>(self), nullptr, cloned);
    return cloned;
}

HOOK(0x006F5070, C_00862748_AllocateAndRegisterClone, "auto");
