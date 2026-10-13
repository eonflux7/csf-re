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

// Dispatches a request block (argument block of 7 dwords) to the scene grid handler.
CSF_FUNC(unsigned int, __fastcall, DispatchWithArgBlock, 0x004DD060, (void* self, void* edx, uint32_t* a2));

// Builds a 7-dword argument block from a source position record and a target
// triple, tagged with the C_0082d448 vtable, and forwards it to DispatchWithArgBlock.
static unsigned int __fastcall SceneGrid_RegisterHandlePosition(S_004dca10* self, void* /*edx*/, void* sourcePos, uint32_t* targetPos)
{
    uint32_t argBlock[7];
    argBlock[2] = CSF_AT(uint32_t, sourcePos, 0x4);
    argBlock[3] = CSF_AT(uint32_t, sourcePos, 0x8);
    argBlock[4] = targetPos[0];
    argBlock[5] = targetPos[1];
    argBlock[6] = targetPos[2];
    argBlock[0] = 1;
    argBlock[1] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    return DispatchWithArgBlock(self, nullptr, argBlock);
}

HOOK(0x004DD220, SceneGrid_RegisterHandlePosition, "auto");
