#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Forwards an integer argument to vtable slot 6 of the m_voice object and returns it unchanged.
static int __fastcall CSoundInstance_ForwardToVoice(CSoundInstance* self, void* /*edx*/, int a2)
{
    void* voice = reinterpret_cast<void*>(self->m_voice);
    ((void(__fastcall*)(void*, void*, int))(CSF_AT(void**, voice, 0)[6]))(voice, nullptr, a2);
    return a2;
}

HOOK(0x005265E0, CSoundInstance_ForwardToVoice, "auto");
