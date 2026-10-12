#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(uint32_t*, __cdecl, RwMatrixMultiply, 0x00624630, (uint32_t* a1, uint32_t* a2, uint32_t* a3));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));

// Copies the node's 4x4 matrix (at +0x10) into a1, then walks the linked chain at +4,
// multiplying a1 by each node's matrix through a scratch matrix. The scratch matrix is
// destroyed at the end and its destroy result is returned. An empty chain returns a2.
static int __cdecl GfxFrames_ComposeMatrixChain(void* a1, int a2)
{
    uint8_t* node = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a2));
    uint32_t firstLink = CSF_AT(uint32_t, node, 4);
    std::memcpy(a1, node + 0x10, 0x40);

    if (firstLink == 0)
        return a2;

    std::uint32_t* scratchMatrix = RwMatrixCreate();
    uint8_t* link = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(firstLink));
    do
    {
        std::memcpy(scratchMatrix, a1, 0x40);
        RwMatrixMultiply(reinterpret_cast<uint32_t*>(a1),
                         reinterpret_cast<uint32_t*>(link + 0x10),
                         scratchMatrix);
        link = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, link, 4)));
    } while (link != nullptr);

    return RwMatrixDestroy(scratchMatrix);
}

HOOK(0x006C0350, GfxFrames_ComposeMatrixChain, "auto");
