#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

CSF_FUNC(S_00768e80*, __fastcall, S_00768e80_Initialize, 0x007693C0,
         (void* self, void* edx, int a2, int a3, int a4, int a5, int a6, int a7,
          uint32_t* a8, uint32_t* a9, int* a10, uint32_t* v13First, uint32_t* v13Second));

CSF_FUNC(char, __fastcall, S_0076a140_StoreResultPointer, 0x0076A140,
         (void* self, void* edx, uint32_t* a3));

// Allocates a 32-byte action object, initializes it from the nine stack arguments,
// and stores the resulting pointer (or nullptr when allocation fails) through the
// S_0076a140 helper.
static char __fastcall S_0076a140_BuildActionBinding(S_0076a140* self, void* /*edx*/,
                                                     int a2, int a3, int a4, int a5,
                                                     int a6, int a7,
                                                     uint32_t* a8, uint32_t* a9, int* a10)
{
    // Local 12-byte block: [0..3]=0, [4..7]=data address, byte at +8 = flag 1.
    uint32_t v13[3];
    v13[0] = 0;
    v13[1] = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x0086F164));
    v13[2] = 0;
    CSF_AT(char, v13, 8) = 1;

    void* allocated = Mem_Alloc(32);
    if (allocated != nullptr)
    {
        a10 = reinterpret_cast<int*>(
            S_00768e80_Initialize(allocated, nullptr, a2, a3, a4, a5, a6, a7,
                                  a8, a9, a10, v13, v13));
        return S_0076a140_StoreResultPointer(self, nullptr,
                                             reinterpret_cast<uint32_t*>(&a10));
    }

    a10 = nullptr;
    return S_0076a140_StoreResultPointer(self, nullptr,
                                         reinterpret_cast<uint32_t*>(&a10));
}

HOOK(0x0076A350, S_0076a140_BuildActionBinding, "auto");
