#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, S_0069b1c0_TryReleaseBlock, 0x0069B590, (S_0069b1c0* self, void* , unsigned int address));
CSF_FUNC(void, __cdecl, _free, 0x007B17CC, (void* Block));

// Offers a block to the four S_0069b1c0 sub-pools (at +0x08, +0x4C, +0x90, +0xD4) in order.
// If the block is not taken by any pool, or pooling is disabled (field_0118 == 0), it is freed.
static void __fastcall C_0085eac0_ForwardBlockHandler(C_0085eac0* self, void* /*edx*/, void* Block)
{
    if (Block == nullptr)
        return;

    const unsigned int blockAddress = static_cast<unsigned int>(reinterpret_cast<uintptr_t>(Block));

    if (self->field_0118 == 0 ||
        (!S_0069b1c0_TryReleaseBlock(reinterpret_cast<S_0069b1c0*>((uint8_t*)self + 0x08), nullptr, blockAddress) &&
         !S_0069b1c0_TryReleaseBlock(reinterpret_cast<S_0069b1c0*>((uint8_t*)self + 0x4C), nullptr, blockAddress) &&
         !S_0069b1c0_TryReleaseBlock(reinterpret_cast<S_0069b1c0*>((uint8_t*)self + 0x90), nullptr, blockAddress) &&
         !S_0069b1c0_TryReleaseBlock(reinterpret_cast<S_0069b1c0*>((uint8_t*)self + 0xD4), nullptr, blockAddress)))
    {
        _free(Block);
    }
}

HOOK(0x0069DFA0, C_0085eac0_ForwardBlockHandler, "auto");
