#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Packs `a2` into a three-dword request block {0, 0, a2} on the stack and
// forwards its address to the class's virtual slot +0x48. Returns the slot's result.
static int __fastcall CCamera_DispatchProjectionRequest(CCamera* self, void* /*edx*/, int a2)
{
    uint32_t requestBlock[3];
    requestBlock[0] = 0;
    requestBlock[1] = 0;
    requestBlock[2] = static_cast<uint32_t>(a2);

    int requestAddress = static_cast<int>(reinterpret_cast<uintptr_t>(requestBlock));
    return self->vSlot48(requestAddress);
}

HOOK(0x006D2D80, CCamera_DispatchProjectionRequest, "auto");
