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

// Game heap allocator.
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Constructs the action binding object (__thiscall, ecx = allocated object).
// Takes the 10 caller arguments plus a pointer to the 12-byte name block.
CSF_FUNC(S_00768e80*, __fastcall, ConstructActionBinding, 0x007693C0,
         (void* self, void* edx, int a2, int a3, int a4, int a5, int a6, int a7,
          uint32_t* a8, uint32_t* a9, uint32_t* a10, void* a11, void* nameBlock));

// Stores a result pointer into the owner (bind line as given by the packet).
CSF_FUNC(char, __fastcall, sub_76A140, 0x0076A140, (void* self, void* edx, uint32_t* a3));

static char __fastcall S_0076a140_CreateAndStoreAction(S_0076a140* self, void* /*edx*/,
                                                       int a2, int a3, int a4, int a5,
                                                       int a6, int a7,
                                                       uint32_t* a8, uint32_t* a9,
                                                       uint32_t* a10, void* a11)
{
    // 12-byte local block: two dwords followed by a flag byte at offset 8.
    struct {
        uint32_t words[2];
        char flag;
    } nameBlock;
    nameBlock.words[0] = 0;
    nameBlock.words[1] = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x0086F164));
    nameBlock.flag = 1;

    // Referenced vtable address (no store in the original; compile-time reference only).
    (void)CSF_VTBL(0x0076A140);

    void* actionObject = Mem_Alloc(32);
    if (actionObject != nullptr)
    {
        a11 = ConstructActionBinding(actionObject, nullptr, a2, a3, a4, a5, a6, a7,
                                     a8, a9, a10, a11, &nameBlock);
        return sub_76A140(self, nullptr, reinterpret_cast<uint32_t*>(&a11));
    }
    else
    {
        a11 = nullptr;
        return sub_76A140(self, nullptr, reinterpret_cast<uint32_t*>(&a11));
    }
}

HOOK(0x0076A3E0, S_0076a140_CreateAndStoreAction, "auto");
