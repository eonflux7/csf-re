#include <cstdint>
#include "hook.h"

// __thiscall sub_44F640(char *this) -> returns this + 0x170
// The original is a trivial accessor: lea eax, [ecx+0x170]; ret
// The concrete class owning this offset is unknown, so we keep the raw pointer type.
static char* __fastcall C_Unknown_GetMember0170(char* self, void* /*edx*/)
{
    return self + 0x170;
}

HOOK(0x44F640, C_Unknown_GetMember0170, "auto");
