#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Fills a clump descriptor: 8 stack args (ret 0x20). Copies the name string
// into the embedded string object at +8 when the source string is non-empty.
static char* __fastcall GfxEnvironment_BuildClumpDescriptor(
    std::uint8_t* self, void* /*edx*/,
    int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9)
{
    CSF_AT(int, self, 0x00) = a2;
    CSF_AT(int, self, 0x04) = a3;
    CSF_AT(int, self, 0x20) = 15;
    CSF_AT(int, self, 0x1C) = 0;
    CSF_AT(std::uint8_t, self, 0x0C) = 0;
    CSF_AT(int, self, 0x24) = a5;
    CSF_AT(int, self, 0x28) = a6;
    CSF_AT(int, self, 0x2C) = a7;
    CSF_AT(int, self, 0x30) = a8;
    CSF_AT(int, self, 0x34) = a9;

    if (a4 != 0 && CSF_AT(int, reinterpret_cast<std::uint8_t*>(a4), 0x14) != 0)
    {
        AssignStringSubstring(self + 8, nullptr,
                              reinterpret_cast<const void*>(a4),
                              0u,
                              static_cast<std::uint32_t>(-1));
    }

    return reinterpret_cast<char*>(self);
}

HOOK(0x006BDC50, GfxEnvironment_BuildClumpDescriptor, "auto");
