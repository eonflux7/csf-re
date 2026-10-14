#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Bound by address: IsFieldAt10Equal36 (0x483360), __fastcall with ecx = object.
CSF_FUNC(int, __fastcall, IsFieldAt10Equal36, 0x00483360, (void* self, void*));

// Returns the result of IsFieldAt10Equal36(this->field_0040). When that check fails,
// clears bit 0x20 of the dword at offset 0x2E0 (736 decimal) of the object.
static int __fastcall ClearFlag0x20UnlessFsmFieldIs36(void* self, void* /*edx*/)
{
    void* fsm = CSF_AT(void*, self, 0x40);
    int result = IsFieldAt10Equal36(fsm, nullptr);
    if (static_cast<uint8_t>(result) == 0)
    {
        CSF_AT(uint32_t, self, 0x2E0) &= ~0x20u;
    }
    return result;
}

HOOK(0x00495250, ClearFlag0x20UnlessFsmFieldIs36, "auto");
