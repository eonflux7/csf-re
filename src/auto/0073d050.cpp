#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Bound by address; the original calls this with ecx = this + 0x7c.
CSF_FUNC(std::uint32_t*, __fastcall, MenuProgressInput_AssignText, 0x00696F90,
         (std::uint32_t* self, void* edx, std::int16_t* text));

// Sets the wide-string name of a character-select entry. Non-empty input is
// assigned through the embedded text object at +0x7C. Otherwise the name is
// cleared in place (inline buffer at +0x80 when capacity at +0x94 is below 8,
// otherwise the heap buffer pointer stored at +0x80).
static std::int16_t* __fastcall CharacterSelect_SetWideName(std::uint8_t* self, void* /*edx*/,
                                                            std::int16_t* a2)
{
    std::int16_t* result = a2;

    if (a2 && *a2) {
        return reinterpret_cast<std::int16_t*>(
            MenuProgressInput_AssignText(reinterpret_cast<std::uint32_t*>(self + 0x7C), nullptr, a2));
    }

    std::int16_t* nameBuffer;
    if (CSF_AT(std::uint32_t, self, 0x94) < 8u) {
        nameBuffer = reinterpret_cast<std::int16_t*>(self + 0x80);
    } else {
        nameBuffer = reinterpret_cast<std::int16_t*>(CSF_AT(std::uint32_t, self, 0x80));
    }
    *nameBuffer = 0;
    return result;
}

HOOK(0x0073D050, CharacterSelect_SetWideName, "auto");
