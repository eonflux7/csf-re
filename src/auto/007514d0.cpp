#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GameConfig_WriteWordField, 0x006563D0, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, GameConfig_WriteDwordField, 0x006563F0, (void* self, void* edx, char a2));

static int __fastcall C_008622e8_WriteSpawnMessage(
    C_008622e8* self, void* /*edx*/, C_008622e8* output,
    int /*a3*/, int /*a4*/, int /*a5*/)
{
    GameConfig_WriteDwordField(
        output, nullptr, static_cast<char>(reinterpret_cast<std::uintptr_t>(self->vtbl)));
    GameConfig_WriteWordField(output, nullptr, static_cast<char>(self->field_0004));
    GameConfig_WriteWordField(output, nullptr, static_cast<char>(self->field_0004 >> 16));
    GameConfig_WriteDwordField(output, nullptr, static_cast<char>(self->field_0008));

    std::uint32_t packedField;
    std::memcpy(&packedField, &self->field_000c, sizeof(packedField));
    GameConfig_WriteDwordField(output, nullptr, static_cast<char>(packedField));

    GameConfig_WriteDwordField(output, nullptr, static_cast<char>(self->field_0010));
    GameConfig_WriteDwordField(output, nullptr, static_cast<char>(self->field_0014));
    GameConfig_WriteDwordField(output, nullptr, static_cast<char>(self->field_0018));
    GameConfig_WriteDwordField(output, nullptr, static_cast<char>(self->field_001c));
    GameConfig_WriteDwordField(output, nullptr, static_cast<char>(self->field_0020));
    return GameConfig_WriteDwordField(output, nullptr, static_cast<char>(self->field_0024));
}

HOOK(0x007514D0, C_008622e8_WriteSpawnMessage, "auto");
