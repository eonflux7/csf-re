#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Clears any previously loaded data in the buffer.
CSF_FUNC(uint32_t*, __fastcall, DstData_ReleaseContents, 0x007822B0, (void* self, void* edx));

// Parses/loads the data source passed in a2 into the buffer.
CSF_FUNC(char, __fastcall, DstData_ParseFromSource, 0x007823C0, (void* self, void* edx, int a2));

// Loads a file into the DstData object. Returns true (1) on success.
static char __fastcall DstData_LoadFile(S_007822b0* self, void* /*edx*/, int a2)
{
    if (self->field_0000)
    {
        DstData_ReleaseContents(self, nullptr);
        self->field_0000 = 0;
    }

    // 0x3f800000 is the IEEE-754 bit pattern of 1.0f.
    self->field_0010 = 0x3f800000;

    if (a2)
    {
        DstData_ParseFromSource(self, nullptr, a2);
    }

    self->field_0000 = 1;
    return 1;
}

HOOK(0x00782550, DstData_LoadFile, "auto");
