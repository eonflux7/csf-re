#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x005274F0 (12 bytes): returns (self->m_state == 2)
static int __fastcall CSoundInstance_IsField006cEqual2(CSoundInstance* self, void* /*edx*/)
{
    return self->m_state == 2;
}

HOOK(0x5274F0, CSoundInstance_IsField006cEqual2, "auto");
