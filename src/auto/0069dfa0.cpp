#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, S_0069b1c0_TryReleaseBlock, 0x0069B590, (S_0069b1c0* self, void*, unsigned int address));
CSF_FUNC(void, __cdecl, _free, 0x007B17CC, (void* Block));

// ECX is this; Block is the single stack argument, popped by ret 4.
// The four calls are direct calls, not vtable dispatches; only AL is tested.
static void __fastcall C_0085eac0_ForwardBlockHandler(C_0085eac0* self, void* /*edx*/, void* Block)
{
    if (Block == nullptr)
        return;

    const unsigned int address = static_cast<unsigned int>(reinterpret_cast<uintptr_t>(Block));
    if (self->field_0118 != 0)
    {
        if (S_0069b1c0_TryReleaseBlock(reinterpret_cast<S_0069b1c0*>(reinterpret_cast<uint8_t*>(self) + 0x08), nullptr, address))
            return;
        if (S_0069b1c0_TryReleaseBlock(reinterpret_cast<S_0069b1c0*>(reinterpret_cast<uint8_t*>(self) + 0x4C), nullptr, address))
            return;
        if (S_0069b1c0_TryReleaseBlock(reinterpret_cast<S_0069b1c0*>(reinterpret_cast<uint8_t*>(self) + 0x90), nullptr, address))
            return;
        if (S_0069b1c0_TryReleaseBlock(reinterpret_cast<S_0069b1c0*>(reinterpret_cast<uint8_t*>(self) + 0xD4), nullptr, address))
            return;
    }

    // The original's free is cdecl: call followed by add esp, 4.
    _free(Block);
}

HOOK(0x0069DFA0, C_0085eac0_ForwardBlockHandler, "auto");
