#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

CSF_FUNC(S_00768e80*, __fastcall, sub_7693C0, 0x007693C0,
         (S_00768e80* self, void* edx, int a2, int a3, int a4, int a5, int a6, int a7,
          uint32_t* a8, uint32_t* a9, uint32_t* a10, uint32_t* a11, void* a12));

CSF_FUNC(char, __fastcall, sub_76A140, 0x0076A140, (void* self, void* edx, uint32_t* a3));

// Allocates a 32-byte object, builds it from the 11 forwarded arguments, and stores
// the resulting pointer (or null on allocation failure) into the out slot passed to sub_76A140.
static char __fastcall S_0076a140_CreateBindingObject(S_0076a140* self, void* /*edx*/,
                                                      int a2, int a3, int a4, int a5, int a6, int a7,
                                                      uint32_t* a8, uint32_t* a9, uint32_t* a10,
                                                      uint32_t* a11, void* a12)
{
    S_00768e80* created = reinterpret_cast<S_00768e80*>(Mem_Alloc(32));
    if (created) {
        a12 = sub_7693C0(created, nullptr, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);
    } else {
        a12 = nullptr;
    }
    return sub_76A140(self, nullptr, reinterpret_cast<uint32_t*>(&a12));
}

HOOK(0x0076A470, S_0076a140_CreateBindingObject, "auto");
