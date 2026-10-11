// Module: game_config. Constructs a C_00869124 config entry from a C string:
// zeroes the header, runs the embedded sub-object reset, copies the string into
// the inline string buffer at +0x234, then stores a single flag byte (via
// sub_69DB80) through the caller's char* output.
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

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(char, __fastcall, GameConfigKey_ResolveFlag, 0x0069DB80, (void* self, void* edx, int a2, int a3, unsigned int a4));
CSF_FUNC(int, __fastcall, GameConfigSubobject_Reset, 0x00753B40, (void* self, void* edx));

static C_00869124* __fastcall GameConfigEntry_InitFromKey(C_00869124* self, void* /*edx*/, char* a2, char* a3)
{
    char* valueText = nullptr;

    self->field_0004 = 0;
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00869124);
    GameConfigSubobject_Reset(reinterpret_cast<void*>(self->pad_0008), nullptr);

    self->field_024c = 15;
    self->field_0248 = 0;
    *reinterpret_cast<uint8_t*>(&self->field_0238) = 0;

    AssignStringRange_429B60(reinterpret_cast<void*>(&self->pad_0008[556]), nullptr, a2, static_cast<unsigned int>(strlen(a2)));

    if (self->field_024c < 0x10)
        valueText = reinterpret_cast<char*>(&self->field_0238);
    else
        valueText = reinterpret_cast<char*>(self->field_0238);

    *a3 = GameConfigKey_ResolveFlag(self, nullptr, reinterpret_cast<int>(valueText), reinterpret_cast<int>(self->pad_0008), 1u);
    return self;
}

HOOK(0x755740, GameConfigEntry_InitFromKey, "auto");
