#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee at 0x7703E0: __thiscall, one stack arg, ret 4.
CSF_FUNC(void, __fastcall, sub_7703E0, 0x007703E0, (void* self, void* edx, void* a2));

// __thiscall(this, int a2, void* a3), ret 8 -> __fastcall with two stack args.
static int* __fastcall S_00769550_InitSceneVariables(S_00769550* self, void* /*edx*/, int a2, void* a3)
{
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0000 = static_cast<uint32_t>(a2);
    sub_7703E0(self, nullptr, a3);
    return reinterpret_cast<int*>(self);
}

HOOK(0x00770440, S_00769550_InitSceneVariables, "auto");
