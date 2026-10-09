#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Game heap allocator (cdecl, caller pops 4 bytes).
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Builds the object on the heap; __thiscall with 11 stack dwords (ret 44).
CSF_FUNC(S_00768e80*, __fastcall, sub_7693C0, 0x007693C0,
         (S_00768e80* self, void* edx, int a2, int a3, int a4, int a5, int a6, int a7,
          uint32_t* a8, int* a9, uint32_t* p1, uint32_t* p2, uint32_t* p3));

// Stores a result pointer through the given slot and returns a status char.
CSF_FUNC(char, __fastcall, WriteResultSlot, 0x0076A140,
         (void* self, void* edx, uint32_t* a3));

static char __fastcall InputBindings_CreateBindingAndStore(S_0076a140* self, void* /*edx*/,
                                                           int a2, int a3, int a4, int a5,
                                                           int a6, int a7, uint32_t* a8, int* a9)
{
    // The original references S_0076a140's vtable here; no store is made by this routine.
    (void)CSF_VTBL(0x0076A140);

    uint32_t v12[2];
    uint8_t v13;
    v12[0] = 0;
    v12[1] = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x0086F164));
    v13 = 1;
    (void)v13;

    S_00768e80* v10 = reinterpret_cast<S_00768e80*>(Mem_Alloc(32));
    if (v10)
    {
        a9 = reinterpret_cast<int*>(sub_7693C0(v10, nullptr, a2, a3, a4, a5, a6, a7,
                                               a8, a9,
                                               reinterpret_cast<uint32_t*>(v12),
                                               reinterpret_cast<uint32_t*>(v12),
                                               reinterpret_cast<uint32_t*>(v12)));
        return WriteResultSlot(self, nullptr, reinterpret_cast<uint32_t*>(&a9));
    }
    else
    {
        a9 = nullptr;
        return WriteResultSlot(self, nullptr, reinterpret_cast<uint32_t*>(&a9));
    }
}

HOOK(0x0076A2C0, InputBindings_CreateBindingAndStore, "auto");
