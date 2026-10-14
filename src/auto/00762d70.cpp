#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char*, __fastcall, AssignElement, 0x00762990, (char* self, void*, int sourceAddress));

static uint32_t __cdecl Assign_00762d70(uint32_t begin, uint32_t end, uint32_t source)
{
    // The empty range leaves EAX untouched in the binary: no defined result.
    uint32_t result = 0;
    for (; begin != end; begin += 556u)
        result = reinterpret_cast<uint32_t>(AssignElement(reinterpret_cast<char*>(begin), nullptr, static_cast<int>(source)));
    return result;
}
HOOK(0x00762D70, Assign_00762d70, "stl");
