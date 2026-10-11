#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(uint32_t*, __fastcall, CopyPointSubobjectFrom, 0x0075C000, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(uint32_t*, __fastcall, CopyStructSubobjectFrom, 0x0075D9C0, (void* self, void* edx, int a2));

// Copy-constructs a waypoint graph node from a source node (__thiscall, one stack arg, ret 4).
static char* __fastcall Waypoint_CopyConstruct(char* self, void* /*edx*/, uint32_t* a2)
{
    CSF_AT(uint32_t, self, 0x00) = CSF_AT(uint32_t, a2, 0x00);

    // Inner string subobject at self+4: reset state, then assign substring from a2+1.
    CSF_AT(uint8_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 15;
    AssignStringSubstring(self + 4, nullptr, a2 + 1, 0, static_cast<std::uint32_t>(-1));

    CSF_AT(uint32_t, self, 0x20) = CSF_AT(uint32_t, a2, 0x20);
    CopyPointSubobjectFrom(self + 0x24, nullptr, a2 + 9);

    CopyStructSubobjectFrom(self + 0x34, nullptr,
        static_cast<int>(reinterpret_cast<std::intptr_t>(a2 + 13)));

    CSF_AT(uint32_t, self, 0x44) = CSF_AT(uint32_t, a2, 0x44);
    return self;
}

HOOK(0x0075DEC0, Waypoint_CopyConstruct, "auto");
