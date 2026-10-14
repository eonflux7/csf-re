#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char*, __fastcall, AssignElement, 0x00773200, (char* self, void*, std::uint8_t* source));

static uint32_t __cdecl Assign_00773bd0(uint32_t begin, uint32_t end, uint32_t source)
{
    // The empty range leaves EAX untouched in the binary: no defined result.
    uint32_t result = 0;
    for (; begin != end; begin += 96u)
        result = reinterpret_cast<uint32_t>(AssignElement(reinterpret_cast<char*>(begin), nullptr, reinterpret_cast<std::uint8_t*>(source)));
    return result;
}
HOOK(0x00773BD0, Assign_00773bd0, "stl");
