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

// RwFrameAddChild(parent, child) in the RenderWare library.
CSF_FUNC(int, __cdecl, RwFrameAddChild, 0x00623B90, (int a1, int a2));

// Adds the frame stored at a2+0x0C as a child of this object's frame (+0x0C).
static int __fastcall C_00860700_AddChildFrame(C_00860700* self, void* /*edx*/, int a2)
{
    int childFrame = CSF_AT(int, a2, 0x0C);
    return RwFrameAddChild(static_cast<int>(self->field_000c), childFrame);
}

HOOK(0x006BF260, C_00860700_AddChildFrame, "auto");
