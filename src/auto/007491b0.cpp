#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

CSF_FUNC(char*, __fastcall, sub_749060, 0x00749060, (void* self, void* edx, int a2));

// Stores a3 at +4 and a2 at +0, then forwards a4 to the embedded subobject at +8.
static void* __fastcall InitPairAndForwardSubobject(void* self, void* /*edx*/, int a2, int a3, int a4)
{
    CSF_AT(int, self, 0x4) = a3;
    CSF_AT(int, self, 0x0) = a2;
    sub_749060(static_cast<uint8_t*>(self) + 8, nullptr, a4);
    return self;
}

HOOK(0x7491B0, InitPairAndForwardSubobject, "auto");
