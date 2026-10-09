#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Game heap free (__cdecl, pops nothing).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destructor body for C_00841128 (derived from CSoundInstance).
// __thiscall with one stack arg (char releaseFlag) -> ret 4.
static uint32_t* __fastcall C_00841128_Destroy(C_00841128* self, void* /*edx*/, char releaseFlag)
{
    self->vtbl = CSF_VTBL(0x00841128);

    if (self->field_0070)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0070));
        self->field_0070 = 0;
    }

    void** voice = reinterpret_cast<void**>(self->m_voice);
    self->vtbl = CSF_VTBL(0x008410B0);
    if (voice)
    {
        // Virtual slot 0 of the voice object: (voice, 1)
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, voice, 0)[0])(voice, nullptr, 1);
        self->m_voice = 0;
    }

    self->m_entityHandle = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D410)));
    self->field_0028 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D410)));
    self->vtbl = CSF_VTBL(0x0082D410);

    if ((releaseFlag & 1) != 0)
        Mem_Free(static_cast<void*>(self));

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x005278D0, C_00841128_Destroy, "auto");
