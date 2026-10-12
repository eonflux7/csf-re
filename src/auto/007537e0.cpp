#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, WriteConfigFlag, 0x00656380, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, GameConfig_WriteDwordField, 0x006563F0, (void* self, void* edx, char a2));

static int __fastcall C_008622e8_SerializeSpawnMessage(
    C_008622e8* self, void* /*edx*/, C_008622e8* writer,
    int /*a3*/, int /*a4*/, int /*a5*/)
{
    WriteConfigFlag(writer, nullptr,
                    reinterpret_cast<const char*>(&self->vtbl)[0]);

    GameConfig_WriteDwordField(writer, nullptr, static_cast<char>(self->field_0004));
    GameConfig_WriteDwordField(writer, nullptr, static_cast<char>(self->field_0008));
    GameConfig_WriteDwordField(
        writer, nullptr,
        static_cast<char>(*reinterpret_cast<const std::uint32_t*>(&self->field_000c)));
    GameConfig_WriteDwordField(writer, nullptr, static_cast<char>(self->field_0010));
    GameConfig_WriteDwordField(writer, nullptr, static_cast<char>(self->field_0014));
    GameConfig_WriteDwordField(writer, nullptr, static_cast<char>(self->field_0018));
    GameConfig_WriteDwordField(writer, nullptr, static_cast<char>(self->field_001c));
    GameConfig_WriteDwordField(writer, nullptr, static_cast<char>(self->field_0020));
    GameConfig_WriteDwordField(writer, nullptr, static_cast<char>(self->field_0024));
    GameConfig_WriteDwordField(writer, nullptr, static_cast<char>(CSF_AT(std::uint32_t, self, 0x28)));
    GameConfig_WriteDwordField(writer, nullptr, static_cast<char>(CSF_AT(std::uint32_t, self, 0x2C)));
    GameConfig_WriteDwordField(writer, nullptr, static_cast<char>(CSF_AT(std::uint32_t, self, 0x30)));
    GameConfig_WriteDwordField(writer, nullptr, static_cast<char>(CSF_AT(std::uint32_t, self, 0x34)));
    GameConfig_WriteDwordField(writer, nullptr, static_cast<char>(CSF_AT(std::uint32_t, self, 0x38)));
    GameConfig_WriteDwordField(writer, nullptr, static_cast<char>(CSF_AT(std::uint32_t, self, 0x3C)));
    GameConfig_WriteDwordField(writer, nullptr, static_cast<char>(CSF_AT(std::uint32_t, self, 0x40)));
    return GameConfig_WriteDwordField(
        writer, nullptr, static_cast<char>(CSF_AT(std::uint32_t, self, 0x44)));
}

HOOK(0x007537E0, C_008622e8_SerializeSpawnMessage, "auto");
