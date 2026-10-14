#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// The handlers are bound as full eax: the original tail-jumps into them, so whatever eax they leave is the
// caller's result (8780 sets only al through its last call, the upper bits come from before).
CSF_FUNC(uint32_t, __fastcall, sub_6A7640, 0x006A7640, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t, __fastcall, sub_6A8780, 0x006A8780, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t, __fastcall, sub_6A8ED0, 0x006A8ED0, (void* self, void* edx, int a2));

// Vtable of S_006a7640, referenced by the original's jump table setup.
[[maybe_unused]] static void* const kS006a7640Vtable = CSF_VTBL(0x006A7640);

// Dispatches on the object's state field (+0x20). State 0, 1, 2 tail-jump into a handler and return its eax
// unchanged; any other state falls to the ret with eax = state - 2 (non-zero, full 32 bits).
static uint32_t __fastcall S_006a7640_DispatchByState(S_006a7640* self, void* /*edx*/, int a2)
{
    uint32_t state = self->field_0020;

    if (state == 0)
        return sub_6A8ED0(self, nullptr, a2);
    if (state == 1)
        return sub_6A8780(self, nullptr, a2);
    if (state == 2)
        return sub_6A7640(self, nullptr, a2);
    return state - 2;
}

HOOK(0x6A97F0, S_006a7640_DispatchByState, "auto");
