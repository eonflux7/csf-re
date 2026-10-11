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

CSF_FUNC(void*, __fastcall, GameConfig_WriteByteField, 0x006563B0, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, GameConfig_WriteDwordField, 0x006563F0, (void* self, void* edx, char a2));
CSF_FUNC(void*, __fastcall, GameConfig_WriteWordField, 0x006563D0, (void* self, void* edx, char a2));

// Writes the header byte (vtable low byte), field_0004, the low word and the high word of field_0008
// to the stream/writer object a2. Returns the last writer result, as the original does.
static void* __fastcall GameConfig_WriteSoundAndSubtitleFields(C_008622e8* self, void* /*edx*/,
                                                               C_008622e8* a2, int /*a3*/, int /*a4*/, int /*a5*/)
{
    uint8_t headerByte = CSF_AT(uint8_t, self, 0);
    GameConfig_WriteByteField(a2, nullptr, (char)headerByte);

    GameConfig_WriteDwordField(a2, nullptr, (char)self->field_0004);

    uint16_t lowWord = (uint16_t)self->field_0008;
    GameConfig_WriteWordField(a2, nullptr, (char)lowWord);

    uint16_t highWord = (uint16_t)(self->field_0008 >> 16);
    return GameConfig_WriteWordField(a2, nullptr, (char)highWord);
}

HOOK(0x7539E0, GameConfig_WriteSoundAndSubtitleFields, "auto");
