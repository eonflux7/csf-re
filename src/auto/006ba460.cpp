#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Game heap allocator (__cdecl, one stack arg, ret 0).
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Sub-routine that takes the new node pointer/count pair (__thiscall, ret 4).
CSF_FUNC(char, __fastcall, sub_6BA300, 0x006BA300, (void* self, void* edx, uint32_t* a3));

static uint32_t* __fastcall GfxObjectProps_CreateNodeAndInsert(S_006b9580* self, void* /*edx*/)
{
    uint32_t* node = nullptr;
    uint32_t* allocated = static_cast<uint32_t*>(Mem_Alloc(8));
    if (allocated)
    {
        allocated[1] = 0xFFFFFFFFu;
        allocated[0] = 0;
        node = allocated;
    }

    uint32_t insertArgs[2];
    insertArgs[0] = reinterpret_cast<uint32_t>(node);
    insertArgs[1] = 1;
    sub_6BA300(self, nullptr, insertArgs);

    return node;
}

HOOK(0x006BA460, GfxObjectProps_CreateNodeAndInsert, "auto");
