#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Original 0x0069F850: __thiscall taking one stack argument (ret 4); the packet's bind line is authoritative.
CSF_FUNC(char, __fastcall, RtFSFileSystemState_Apply, 0x0069F850, (void* self, void* edx, int a2));

// Resets the RtFS file-system state record to its defaults (default value 10 at +0x38), then, when a
// non-zero argument is supplied, applies it through RtFSFileSystemState_Apply. Returns a pointer to the record's
// first dword (+0x00), as the original does.
static uint32_t* __fastcall RtFSFileSystemState_Construct(S_0069f3b0* self, void* /*edx*/, int a2)
{
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0038 = 10;
    self->field_0000 = 0;
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_003c = 0;
    self->field_0040 = 0;

    if (a2 != 0)
        RtFSFileSystemState_Apply(self, nullptr, a2);

    return &self->field_0000;
}

HOOK(0x0069F980, RtFSFileSystemState_Construct, "auto");
