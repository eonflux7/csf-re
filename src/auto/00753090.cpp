#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, WriteConfigFlag, 0x00656380, (void* self, void* edx, char a2));
CSF_FUNC(void*, __fastcall, GameConfig_WriteByteField, 0x006563B0, (void* self, void* edx, char a2));
CSF_FUNC(void*, __fastcall, GameConfig_WriteWordField, 0x006563D0, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, GameConfig_WriteDwordField, 0x006563F0, (void* self, void* edx, char a2));

static void* __fastcall C_008622e8_WriteSpawnMessage(
    C_008622e8* self, void* /*edx*/, C_008622e8* destination,
    int /*a3*/, int /*a4*/, int /*a5*/)
{
    WriteConfigFlag(destination, nullptr,
        static_cast<char>(reinterpret_cast<std::uintptr_t>(self->vtbl)));
    WriteConfigFlag(destination, nullptr,
        static_cast<char>(reinterpret_cast<std::uintptr_t>(self->vtbl) >> 8));
    WriteConfigFlag(destination, nullptr,
        static_cast<char>(reinterpret_cast<std::uintptr_t>(self->vtbl) >> 16));
    WriteConfigFlag(destination, nullptr,
        static_cast<char>(reinterpret_cast<std::uintptr_t>(self->vtbl) >> 24));
    WriteConfigFlag(destination, nullptr, static_cast<char>(self->field_0004));
    GameConfig_WriteByteField(destination, nullptr,
        static_cast<char>(self->field_0004 >> 8));
    GameConfig_WriteByteField(destination, nullptr,
        static_cast<char>(self->field_0004 >> 16));

    for (int i = 0; i < 128; ++i)
        GameConfig_WriteByteField(destination, nullptr,
            static_cast<char>(CSF_AT(std::uint8_t, self, 0x07 + i)));

    for (int j = 0; j < 128; ++j)
        GameConfig_WriteByteField(destination, nullptr,
            static_cast<char>(CSF_AT(std::uint8_t, self, 0x87 + j)));

    for (int k = 0; k < 128; ++k)
        GameConfig_WriteByteField(destination, nullptr,
            static_cast<char>(CSF_AT(std::uint8_t, self, 0x107 + k)));

    // These callees read two/four bytes from their stack argument, despite
    // the supplied bindings declaring char. Preserve the entire argument.
    using WriteDword = int (__fastcall*)(void*, void*, std::uint32_t);
    using WriteWord = void* (__fastcall*)(void*, void*, std::uint32_t);
    const auto writeDword = reinterpret_cast<WriteDword>(GameConfig_WriteDwordField);
    const auto writeWord = reinterpret_cast<WriteWord>(GameConfig_WriteWordField);

    writeDword(destination, nullptr, CSF_AT(std::uint32_t, self, 0x188));
    writeDword(destination, nullptr, CSF_AT(std::uint32_t, self, 0x18C));
    writeDword(destination, nullptr, CSF_AT(std::uint32_t, self, 0x190));

    writeWord(destination, nullptr, CSF_AT(std::uint16_t, self, 0x194));
    writeWord(destination, nullptr, CSF_AT(std::uint16_t, self, 0x196));
    writeWord(destination, nullptr, CSF_AT(std::uint16_t, self, 0x198));
    writeWord(destination, nullptr, CSF_AT(std::uint16_t, self, 0x19A));
    return writeWord(destination, nullptr, CSF_AT(std::uint16_t, self, 0x19C));
}

HOOK(0x00753090, C_008622e8_WriteSpawnMessage, "auto");
