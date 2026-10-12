#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GameConfig_WriteWordField, 0x006563D0, (void* self, void* edx, char a2));

static void* __fastcall C_008622e8_WriteWordField(
    C_008622e8* self, void* /*edx*/, C_008622e8* destination,
    int /*a3*/, int /*a4*/, int /*a5*/)
{
    return GameConfig_WriteWordField(
        destination, nullptr, static_cast<char>(CSF_AT(uint16_t, self, 0)));
}

HOOK(0x00751AD0, C_008622e8_WriteWordField, "auto");
