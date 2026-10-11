#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(uint32_t*, __fastcall, WaterParams_Construct, 0x006FDEE0, (void* self, void* edx));

// Constructs the water effect object in place: installs the base vtable,
// constructs the embedded parameter block at +0x0C, initializes the
// simulation fields, creates an RW matrix, then installs the derived vtable
// and zeroes the trailing three dwords. Returns this.
static char* __fastcall WaterEffect_Construct(char* self, void* /*edx*/)
{
    CSF_AT(void*, self, 0) = CSF_VTBL(0x862FEC);
    CSF_AT(uint8_t, self, 0x04) = 0;

    WaterParams_Construct(self + 12, nullptr);

    CSF_AT(int, self, 0xB0) = 15;
    CSF_AT(int, self, 0xAC) = 0;
    CSF_AT(uint8_t, self, 0x9C) = 0;

    CSF_AT(uint32_t, self, 0xB8) =
        static_cast<uint32_t>(reinterpret_cast<uintptr_t>(RwMatrixCreate()));

    CSF_AT(uint8_t, self, 0xBC) = 1;
    CSF_AT(void*, self, 0) = CSF_VTBL(0x86350C);
    CSF_AT(int, self, 0xE0) = 0;
    CSF_AT(int, self, 0xE4) = 0;
    CSF_AT(int, self, 0xE8) = 0;

    return self;
}

HOOK(0x70A2C0, WaterEffect_Construct, "auto");
