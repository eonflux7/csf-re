#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x00625270: __cdecl RenderWare image path setter (caller pops its argument).
CSF_FUNC(const void*, __cdecl, RwImageSetPath, 0x00625270, (const void* path));

// 0x006C1040: __stdcall forwarder (ret 4). Passes its single path argument straight
// to RwImageSetPath and returns the resulting pointer as the raw 32-bit value.
static int __stdcall GfxTexDict_ForwardImageSetPath(int path)
{
    const void* result = RwImageSetPath(reinterpret_cast<const void*>(static_cast<std::uintptr_t>(path)));
    return static_cast<int>(reinterpret_cast<std::uintptr_t>(result));
}

HOOK(0x6C1040, GfxTexDict_ForwardImageSetPath, "auto");
