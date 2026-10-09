#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"
#include "core/memory.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));

static S_004f21d0* __fastcall S_004f21d0_ResetAndAssign(S_004f21d0* self, void* /*edx*/, void* a2)
{
    self->field_0014 = 0;
    self->field_0018 = 7;
    self->field_0004 = 0;
    AssignStringRange_488830(self, nullptr, a2, 0, 0xFFFFFFFF);
    return self;
}

HOOK(0x488BE0, S_004f21d0_ResetAndAssign, "auto");
