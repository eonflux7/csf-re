#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Base initializer reached through C_00863ba8/C_00863800 chain (unnamed callee, name chosen here).
CSF_FUNC(uint32_t*, __fastcall, AudioEffectBaseInit, 0x00710A50, (void* self, void* edx));

// C_00863ac0 constructor: runs the base initializer, clears the two flag bytes,
// installs C_00863ac0's vtable, sets the default field at +0x34 to 32, and returns this.
static uint8_t* __fastcall AudioEffect_ConstructDefault(C_00863ac0* self, void* /*edx*/)
{
    AudioEffectBaseInit(self, nullptr);

    self->field_0038 = 0;
    self->field_0039 = 0;
    self->vtbl = CSF_VTBL(0x00863AC0);
    self->field_0034 = 32;

    return reinterpret_cast<uint8_t*>(self);
}

HOOK(0x70EC70, AudioEffect_ConstructDefault, "auto");
