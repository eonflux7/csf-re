#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(void*, __fastcall, WriteConfigFlag, 0x00656380, (void* self, void* edx, char a2));
static CSF_FUNC(int, __fastcall, GameConfig_WriteDwordField, 0x006563F0, (void* self, void* edx, char a2));

static int __fastcall C_008622e8_WriteSpawnMessage(
    C_008622e8* self, void* /*edx*/, C_008622e8* writer,
    int /*a3*/, int /*a4*/, int /*a5*/)
{
    WriteConfigFlag(writer, nullptr, static_cast<char>(CSF_AT(uint8_t, self, 0)));
    GameConfig_WriteDwordField(writer, nullptr, static_cast<char>(self->field_0004));
    GameConfig_WriteDwordField(writer, nullptr, static_cast<char>(self->field_0008));

    uint32_t finalField;
    std::memcpy(&finalField, &self->field_000c, sizeof(finalField));
    return GameConfig_WriteDwordField(writer, nullptr, static_cast<char>(finalField));
}

HOOK(0x00752D60, C_008622e8_WriteSpawnMessage, "auto");
