#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x0053D060 — __thiscall, no stack args (ret 0): updates/handles the matched control.
CSF_FUNC(void*, __fastcall, CMenuSoundOptions_OnControlMatched, 0x0053D060, (CMenuSoundOptions* self, void* edx));

// 0x0053D1D0 — __thiscall(self, other), ret 4.
// If the stored control pointer (field_0048) is non-null and equals other->field_0004,
// forward to the matched-control handler (0x0053D060); otherwise just return the stored pointer.
static void* __fastcall CMenuSoundOptions_HandleControlMatch(CMenuSoundOptions* self, void* /*edx*/, void* other)
{
    void* result = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0048));
    if (result)
    {
        if (result == reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, other, 0x4))))
            return CMenuSoundOptions_OnControlMatched(self, nullptr);
    }
    return result;
}

HOOK(0x0053D1D0, CMenuSoundOptions_HandleControlMatch, "auto");
