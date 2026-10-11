#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callees (bound by address)
CSF_FUNC(int, __fastcall, CSaveGameManager_ResetStateBlock, 0x00512CD0, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, InitStringFromLiteral, 0x00692720, (void* returnSlot, const char* text, int count));

static CSaveGameManager* __fastcall CSaveGameManager_InitializeDefaults(CSaveGameManager* self, void* /*edx*/)
{
    self->field_0004 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->vtbl = CSF_VTBL(0x00840920);
    self->field_0054 = 0;
    self->field_0058 = 7;
    self->field_0044 = 0;  // low word written by original (mov word ptr); upper half preserved
    self->field_0074 = 7;
    self->field_0070 = 0;

    S_004f21d0* pad_0040 = reinterpret_cast<S_004f21d0*>(CSF_AT(uint8_t*, self, 0x40));

    self->field_0060 = 0;  // low word written by original
    self->field_0090 = 7;
    self->field_008c = 0;
    self->field_007c = 0;  // low word written by original
    self->field_00ac = 15;
    self->field_00a8 = 0;
    self->field_0098 = 0;  // low byte written by original

    CSaveGameManager_ResetStateBlock(self, nullptr);

    self->field_0018 = 1;
    self->field_001c = 0;
    self->field_0024 = 0;
    self->field_0020 = 0;
    self->field_0028 = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_002c = 0;
    self->field_003c = 0;

    InitStringFromLiteral(pad_0040, CSF_STR(0x00840914, "SAV4"), 1);
    return self;
}

HOOK(0x00513BA0, CSaveGameManager_InitializeDefaults, "auto");
