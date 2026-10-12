#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee at 0x006C4330: __fastcall (this in ecx), receives the inner object pointer.
CSF_FUNC(int, __fastcall, ParticleInner_Update, 0x006C4330, (void* self, void* edx));

// Sets the byte at inner+0x20 to 1, runs the inner update, then clears it again.
// The inner object pointer lives at self+0x28; the result of the update is returned.
static int __fastcall ParticleSystem_UpdateWithInnerFlag(void* self, void* /*edx*/)
{
    uint8_t* inner = CSF_AT(uint8_t*, self, 0x28);
    CSF_AT(uint8_t, inner, 0x20) = 1;

    int result = ParticleInner_Update(inner, nullptr);

    inner = CSF_AT(uint8_t*, self, 0x28);
    CSF_AT(uint8_t, inner, 0x20) = 0;
    return result;
}

HOOK(0x701030, ParticleSystem_UpdateWithInnerFlag, "auto");
