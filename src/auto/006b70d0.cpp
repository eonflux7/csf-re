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

CSF_FUNC(int, __cdecl, RpWorldDestroy, 0x0058CBD0, (int a1));
CSF_FUNC(char, __fastcall, RenderWorldList_FreeNodes, 0x006B6FE0, (void* self, void* edx, void** Block));
CSF_FUNC(char, __fastcall, SceneLoader_ReleaseRenderState, 0x006B6CF0, (void* self, void* edx));

// Resets the render world: destroys the RpWorld, frees the list nodes hanging
// off the list head at field_0008, re-initializes the list to empty, then
// tail-jumps into SceneLoader_ReleaseRenderState.
static void* __fastcall SceneLoader_ResetRenderWorld(C_00860388* self, void* /*edx*/)
{
    // Destroy the render world if one exists.
    if (self->field_0010)
    {
        RpWorldDestroy(static_cast<int>(self->field_0010));
        self->field_0010 = 0;
    }

    // Free the list nodes; the list lives at (self + 4).
    uint32_t listHead = self->field_0008;
    uint32_t firstNode = CSF_AT(uint32_t, listHead, 4);
    RenderWorldList_FreeNodes(reinterpret_cast<uint8_t*>(self) + 4, nullptr,
                              reinterpret_cast<void**>(firstNode));

    // Re-initialize the list head as empty (self-linked).
    listHead = self->field_0008;
    CSF_AT(uint32_t, listHead, 4) = listHead;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, listHead, 0) = listHead;
    CSF_AT(uint32_t, listHead, 8) = listHead;

    // Tail jump into the subsystem reset (ecx = this, no stack args).
    (void)CSF_ADDR_VT(0x006B6CF0);
    char result = SceneLoader_ReleaseRenderState(self, nullptr);
    return reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint8_t>(result)));
}

HOOK(0x006B70D0, SceneLoader_ResetRenderWorld, "auto");
