#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class constructor chain step (bound by address, __fastcall/__thiscall, ret 0).
CSF_FUNC(uint32_t*, __fastcall, AreaMeshBase_Init, 0x007711C0, (void* self, void* edx));

// Legacy hook reads code bytes here (known emudiff DIFF). Keep hook behaviour
// unchanged in this mechanical cutover; the binary actually stores an immediate.
#ifndef CSF_STANDALONE
CSF_GLOBAL(uint32_t, g_DefaultAreaMeshParam, 0x00800000);
#endif

static uint32_t* __fastcall GameAreaMesh_Construct(C_0083eee4* self, void* /*edx*/)
{
    AreaMeshBase_Init(self, nullptr);

    CSF_AT(uint32_t, self, 0x14) = reinterpret_cast<uint32_t>(CSF_VTBL(0x008304D8));
    self->field_0018 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D428));

    self->vtbl = CSF_VTBL(0x0083EEE4);

    CSF_AT(uint32_t, self, 0x14) = reinterpret_cast<uint32_t>(CSF_VTBL(0x0083EEE0));
    self->field_0018 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0083EEC4));

    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 10;
    self->field_002c = 0;
#ifdef CSF_STANDALONE
    self->field_0030 = 1u << 23;
#else
    self->field_0030 = g_DefaultAreaMeshParam;
#endif

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004EF4B0, GameAreaMesh_Construct, "auto");
