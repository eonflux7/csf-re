#include <cstdint>
#include "globals.h"
#include "hook.h"

// __thiscall (this = ecx, one stack arg), ret 4. The second argument is passed
// through untouched, so it is typed as void*.
CSF_FUNC(int, __fastcall, ParticleEmitters_ApplyEmitterEntry, 0x006E11C0, (void* self, void* edx, void* a2));

// Forwards a2 to sub_6E11C0 (with a1 as the this pointer) when a1 is non-null;
// otherwise nothing is done. The original returned an uninitialized eax in that
// case; this returns 0 instead.
static int __stdcall ParticleEmitters_ForwardEntryIfNonNull(void* a1, void* a2)
{
    if (a1)
        return ParticleEmitters_ApplyEmitterEntry(a1, nullptr, a2);
    return 0;
}

HOOK(0x006E1540, ParticleEmitters_ForwardEntryIfNonNull, "auto");
