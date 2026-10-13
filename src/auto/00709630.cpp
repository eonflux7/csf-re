#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

// The caller pushes four arguments (begin, end, &field_0050, this) before the
// cdecl call and cleans up 0x10 bytes, so the binding is variadic to match.
CSF_FUNC(int, __cdecl, GfxScreenEffects_ResetEntryRange, 0x006F8810,
         (int begin, int end, ...));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall C_00862b58_ResetEntryStorage(C_00862b58* self, void* /*edx*/)
{
    int result = static_cast<int>(self->field_0054);
    auto* storage = reinterpret_cast<std::uint8_t*>(self) + 0x50;

    if (result != 0) {
        GfxScreenEffects_ResetEntryRange(
            result,
            CSF_AT(int, self, 0x58),
            storage,
            self);

        // Re-read after the call: the callee may have changed the object.
        result = static_cast<int>(self->field_0054);
        if (result != 0) {
            result = Mem_FreeDebug(
                reinterpret_cast<void*>(static_cast<std::intptr_t>(result)));
        }
    }

    self->field_0054 = 0;
    CSF_AT(int, self, 0x58) = 0;
    CSF_AT(int, self, 0x5C) = 0;
    return result;
}

HOOK(0x00709630, C_00862b58_ResetEntryStorage, "auto");
