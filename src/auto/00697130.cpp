#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x00697070, __thiscall with one stack argument (ret 4). Return value unused by the caller.
CSF_FUNC(char, __fastcall, TextInputSubobject_Setup, 0x00697070, (void* self, void* edx, uint8_t* a2));

// Original: 0x00697130, __thiscall with one stack argument (ret 4).
// Runs the setup on the same object and the same argument, then returns this.
static void* __fastcall TextInputSubobject_Init(S_004f21d0* self, void* /*edx*/, uint8_t* a2)
{
    TextInputSubobject_Setup(self, nullptr, a2);
    return self;
}

HOOK(0x697130, TextInputSubobject_Init, "auto");
