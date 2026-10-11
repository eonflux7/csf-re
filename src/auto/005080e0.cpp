#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

using std::uint32_t;

CSF_FUNC(char, __fastcall, AppMain_ProcessBracketCommand, 0x00507CB0,
         (void* self, void* edx, uint32_t* a2, char* Str1));
CSF_FUNC(std::uint32_t*, __fastcall, MenuProgressInput_AssignText, 0x00696F90,
         (std::uint32_t* self, void* edx, std::int16_t* text));

static char __fastcall S_005080e0_ProcessBracketedText(
    S_005080e0* self, void* /*edx*/, std::uint32_t* input,
    std::uint16_t* source)
{
    alignas(8) std::uint8_t textBytes[0xC8];
    alignas(8) std::uint8_t commandBytes[0x24];
    auto* text = reinterpret_cast<std::int16_t*>(textBytes);
    auto* output = text;
    auto* command = reinterpret_cast<char*>(commandBytes);
    char success = 1;

    do
    {
        const std::uint16_t character = *source;
        if (character == 0)
            break;

        if (character == 91)
        {
            ++source;
            char* destination = command;
            while (*source != 93)
            {
                *destination++ = *reinterpret_cast<char*>(source);
                ++source;
            }
            *destination = 0;
            ++source;
            success = AppMain_ProcessBracketCommand(self, nullptr, input, command);
        }
        else
        {
            *output++ = static_cast<std::int16_t>(character);
            ++source;
        }
    } while (success);

    *output = 0;
    MenuProgressInput_AssignText(
        reinterpret_cast<std::uint32_t*>(
            reinterpret_cast<std::uint8_t*>(input) + 0x1C),
        nullptr, text);
    return success;
}

HOOK(0x005080E0, S_005080e0_ProcessBracketedText, "auto");
