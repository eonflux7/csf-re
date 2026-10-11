#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee: sub_4DB0F0, __thiscall with one stack argument (ret 4).
CSF_FUNC(int, __fastcall, sub_4DB0F0, 0x004DB0F0, (void* self, void* edx, int a2));

// Callee: sub_4DB1D0, __thiscall with one stack argument (ret 4).
CSF_FUNC(void, __fastcall, sub_4DB1D0, 0x004DB1D0, (void* self, void* edx, int a2));

// Original: 0x420EC0. Forwards the request to the base handler, then, unless the
// owner object's flag at +0x205 is set, clears the girar state with argument 0.
static void __fastcall TCharTask_SetStateAndMaybeStopGirar(TCharTask* self, void* /*edx*/, int a2)
{
    sub_4DB0F0(self, nullptr, a2);

    // self->field_0008 holds a pointer to the owner object (not declared on TCharTask).
    uint8_t* owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x08));
    if (!CSF_AT(uint8_t, owner, 0x205))
    {
        sub_4DB1D0(self, nullptr, 0);
    }
}

HOOK(0x420EC0, TCharTask_SetStateAndMaybeStopGirar, "auto");
