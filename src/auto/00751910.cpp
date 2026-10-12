#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GameConfig_WriteByteField, 0x006563B0, (void* self, void* edx, char a2));
CSF_FUNC(void*, __fastcall, GameConfig_WriteWordField, 0x006563D0, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, GameConfig_WriteDwordField, 0x006563F0, (void* self, void* edx, char a2));

static void* __fastcall C_008622e8_ForwardOperation(
    C_008622e8* self, void* /*edx*/, C_008622e8* destination,
    int /*a3*/, int /*a4*/, int /*a5*/)
{
    GameConfig_WriteByteField(
        destination, nullptr, static_cast<char>(CSF_AT(uint8_t, self, 0)));

    for (int i = 0; i < 256; ++i)
        GameConfig_WriteByteField(
            destination, nullptr,
            static_cast<char>(CSF_AT(uint8_t, self, i + 1)));

    // Preserve the full stack arguments shown by the disassembly despite
    // the supplied bindings' narrower parameter declarations.
    using WriteDword = int (__fastcall*)(void*, void*, uint32_t);
    using WriteWord = void* (__fastcall*)(void*, void*, uint32_t);

    reinterpret_cast<WriteDword>(GameConfig_WriteDwordField)(
        destination, nullptr, CSF_AT(uint32_t, self, 0x104));
    reinterpret_cast<WriteDword>(GameConfig_WriteDwordField)(
        destination, nullptr, CSF_AT(uint32_t, self, 0x108));

    void* result;
    for (int i = 0; i < 128; ++i)
        result = reinterpret_cast<WriteWord>(GameConfig_WriteWordField)(
            destination, nullptr, CSF_AT(uint16_t, self, 0x10C + i * 2));

    return result;
}

HOOK(0x00751910, C_008622e8_ForwardOperation, "auto");
