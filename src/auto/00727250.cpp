#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// int __cdecl sub_690E60(void *Block)  -> Mem_FreeDebug
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x690E60, (void* block));

// Resets a MSVC 7.1 std::string-like storage:
//   +0x08  buffer / _Bx pointer (when heap-allocated)
//   +0x18  _Mysize
//   +0x1C  _Myres
// If the storage was heap-allocated (_Myres >= 0x10) the buffer is freed.
// Then the string is reset to the empty state with the small-buffer capacity (15).
static int __fastcall ResetStringStorage(void* self, void* /*edx*/)
{
    int result;

    if (*(uint32_t*)((uint8_t*)self + 0x1C) >= 0x10)
    {
        result = *(int*)((uint8_t*)self + 0x08);
        if (result)
            result = Mem_FreeDebug((void*)result);
    }

    *(uint32_t*)((uint8_t*)self + 0x1C) = 15;
    *(uint32_t*)((uint8_t*)self + 0x18) = 0;
    *((uint8_t*)self + 0x08) = 0;

    return result;
}

HOOK(0x727250, ResetStringStorage, "auto");
