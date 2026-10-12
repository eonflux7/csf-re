#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(std::uint32_t*, __fastcall, MenuProgressInput_AssignText, 0x00696F90, (std::uint32_t* self, void* edx, std::int16_t* text));
CSF_FUNC(void*, __fastcall, TextInputSubobject_Init, 0x00697130, (void* self, void* edx, uint8_t* arg));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

static int __cdecl VariantValues_ConstructWideString(
    S_004f21d0* destination, int valueType, S_004f21d0* value)
{
    alignas(8) std::uint8_t bufferBytes[0x100];
    alignas(8) std::uint8_t temporaryBytes[0x1C]{};
    char* buffer = reinterpret_cast<char*>(bufferBytes);

    CSF_AT(std::uint32_t, temporaryBytes, 0x18) = 7;
    CSF_AT(std::uint32_t, temporaryBytes, 0x14) = 0;
    CSF_AT(std::uint16_t, temporaryBytes, 0x04) = 0;

    switch (valueType)
    {
    case 100: // 'd'
        _sprintf(buffer, CSF_STR(0x0085E688, "%i"),
                 CSF_AT(std::int32_t, value, 0x00));
        TextInputSubobject_Init(temporaryBytes, nullptr, bufferBytes);
        break;

    case 102: // 'f'
        _sprintf(buffer, CSF_STR(0x00840C3C, "%f"),
                 static_cast<double>(CSF_AT(float, value, 0x00)));
        TextInputSubobject_Init(temporaryBytes, nullptr, bufferBytes);
        break;

    case 115: // 's'
        TextInputSubobject_Init(
            temporaryBytes, nullptr, reinterpret_cast<std::uint8_t*>(value));
        break;

    case 119: // 'w'
        MenuProgressInput_AssignText(
            reinterpret_cast<std::uint32_t*>(temporaryBytes), nullptr,
            reinterpret_cast<std::int16_t*>(value));
        break;

    default:
        break;
    }

    destination->field_0018 = 7;
    destination->field_0014 = 0;
    const std::uint16_t emptyCharacter = 0;
    std::memcpy(&destination->field_0004, &emptyCharacter, sizeof(emptyCharacter));

    AssignStringRange_488830(destination, nullptr, temporaryBytes, 0, 0xFFFFFFFF);

    if (CSF_AT(std::uint32_t, temporaryBytes, 0x18) >= 8)
    {
        void* block = CSF_AT(void*, temporaryBytes, 0x04);
        if (block)
            Mem_FreeDebug(block);
    }

    return static_cast<int>(reinterpret_cast<std::intptr_t>(destination));
}

HOOK(0x00695B80, VariantValues_ConstructWideString, "auto");
