#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, sub_426550, 0x00426550, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static void __fastcall S_00426550_PopEntryAndAdvance(S_00426550* self, void* /*edx*/)
{
    // Vtable reference from the original's jmp target (no runtime effect).
    (void)CSF_VTBL(0x00426550);

    uint32_t remainingAfterPop = self->field_000c - 1;
    if (self->field_000c == 1)
    {
        self->field_000c = 0;
    }
    else
    {
        uint32_t bufferAddress = self->field_0004;
        self->field_000c = remainingAfterPop;
        // Shift the remaining 36-byte entries down over the popped one.
        _memcpy(reinterpret_cast<void*>(static_cast<uintptr_t>(bufferAddress)),
                reinterpret_cast<const void*>(static_cast<uintptr_t>(bufferAddress + 36)),
                36 * remainingAfterPop);
    }

    if (self->field_000c == 0)
    {
        int cursor = static_cast<int>(self->field_0024);
        if (cursor < static_cast<int>(self->field_001c) - 2)
        {
            self->field_0024 = static_cast<uint32_t>(cursor + 1);
            // Tail call to sub_426550 with the same `this`.
            sub_426550(self, nullptr);
        }
    }
}

HOOK(0x00426D20, S_00426550_PopEntryAndAdvance, "auto");
