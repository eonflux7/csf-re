#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void, __fastcall, sub_6A7640, 0x006A7640, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, sub_6A8780, 0x006A8780, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, sub_6A8ED0, 0x006A8ED0, (void* self, void* edx, int a2));

// Vtable of S_006a7640, referenced by the original's jump table setup.
[[maybe_unused]] static void* const kS006a7640Vtable = CSF_VTBL(0x006A7640);

// Dispatches on the object's state field (+0x20). The original tail-jumps into
// one of three handlers, so the handler's result (char) is what the caller gets.
static char __fastcall S_006a7640_DispatchByState(S_006a7640* self, void* /*edx*/, int a2)
{
    uint32_t state = self->field_0020;

    if (state == 0)
    {
        sub_6A8ED0(self, nullptr, a2);
        return 0;
    }
    if (state == 1)
    {
        return sub_6A8780(self, nullptr, a2);
    }
    if (state == 2)
    {
        sub_6A7640(self, nullptr, a2);
        return 0;
    }
    return 0;
}

HOOK(0x6A97F0, S_006a7640_DispatchByState, "auto");
