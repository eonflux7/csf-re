#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GameConfig_WriteByteField, 0x006563B0, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, GameConfig_WriteDwordField, 0x006563F0, (void* self, void* edx, char a2));

static int __fastcall C_008622e8_WriteSpawnFields(
    C_008622e8* self, void* /*edx*/, C_008622e8* destination,
    int /*a3*/, int /*a4*/, int /*a5*/)
{
    GameConfig_WriteByteField(
        destination, nullptr, static_cast<char>(CSF_AT(uint8_t, self, 0x000)));
    return GameConfig_WriteDwordField(
        destination, nullptr, static_cast<char>(self->field_0004));
}

HOOK(0x00753280, C_008622e8_WriteSpawnFields, "auto");
