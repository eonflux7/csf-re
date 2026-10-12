#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GameConfig_WriteWordField, 0x006563D0, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, GameConfig_WriteDwordField, 0x006563F0, (void* self, void* edx, char a2));

static int __fastcall C_008622e8_WriteSpawnMessage(
    C_008622e8* self, void* /*edx*/, C_008622e8* writer,
    int /*a3*/, int /*a4*/, int /*a5*/)
{
    GameConfig_WriteWordField(
        writer, nullptr, static_cast<char>(CSF_AT(uint16_t, self, 0x000)));
    GameConfig_WriteWordField(
        writer, nullptr, static_cast<char>(CSF_AT(uint16_t, self, 0x002)));

    GameConfig_WriteDwordField(writer, nullptr, static_cast<char>(self->field_0004));
    GameConfig_WriteDwordField(writer, nullptr, static_cast<char>(self->field_0008));

    uint32_t packedField;
    std::memcpy(&packedField, &self->field_000c, sizeof(packedField));
    GameConfig_WriteDwordField(writer, nullptr, static_cast<char>(packedField));

    GameConfig_WriteDwordField(writer, nullptr, static_cast<char>(self->field_0010));
    GameConfig_WriteDwordField(writer, nullptr, static_cast<char>(self->field_0014));
    GameConfig_WriteDwordField(writer, nullptr, static_cast<char>(self->field_0018));
    GameConfig_WriteDwordField(writer, nullptr, static_cast<char>(self->field_001c));
    return GameConfig_WriteDwordField(
        writer, nullptr, static_cast<char>(self->field_0020));
}

HOOK(0x00751780, C_008622e8_WriteSpawnMessage, "auto");
