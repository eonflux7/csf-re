#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// int __cdecl sub_690E60(void *Block)  -> Mem_FreeDebug
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x690E60, (void* block));

// Resets a MSVC 7.1 std::string-like storage:
//   +0x04  buffer / _Bx pointer (when heap-allocated)
//   +0x14  _Mysize
//   +0x18  _Myres
// If the storage was heap-allocated (_Myres >= 0x10) the buffer is freed.
// Then the string is reset to the empty state with the small-buffer capacity (15).
static int __fastcall CMenuSaveGame_ResetStringStorage(void* self, void* /*edx*/)
{
    int result = 0;  // the original (a void _Tidy) leaves eax undefined when nothing is freed

    if (*(uint32_t*)((uint8_t*)self + 0x18) >= 0x10)
    {
        result = *(int*)((uint8_t*)self + 0x04);
        if (result)
            result = Mem_FreeDebug((void*)result);
    }

    *(uint32_t*)((uint8_t*)self + 0x18) = 15;
    *(uint32_t*)((uint8_t*)self + 0x14) = 0;
    *((uint8_t*)self + 0x04) = 0;

    return result;
}

HOOK(0x544310, CMenuSaveGame_ResetStringStorage, "auto");
