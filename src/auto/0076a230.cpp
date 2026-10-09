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

// Game heap allocator, called with a single cdecl size argument.
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Builds the 11-argument object constructor. Thiscall with 11 stack args (ret 44).
CSF_FUNC(S_00768e80*, __fastcall, sub_7693C0, 0x007693C0,
         (void* self, void* edx, int a2, int a3, int a4, int a5, int a6, int a7, int a8,
          int* p9, int* p10, int* p11, int* p12));

// Stores the result pointer into this and returns a char status (ret 4).
CSF_FUNC(char, __fastcall, CommitResultPointer, 0x0076A140, (void* self, void* edx, uint32_t* a3));

// Local 12-byte scratch frame passed by address to the constructor (four aliases of the same slot).
struct InputBindingScratch {
    int words[2];
    char flag;
};

static char __fastcall InputBindings_CreateAndCommitHandle(S_0076a140* self, void* /*edx*/,
                                                           int a2, int a3, int a4, int a5,
                                                           int a6, int a7, int* a8)
{
    InputBindingScratch scratch;
    scratch.words[0] = 0;
    scratch.words[1] = static_cast<int>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x0086F164)));
    scratch.flag = 1;

    void* rawMemory = Mem_Alloc(32);
    if (rawMemory) {
        a8 = reinterpret_cast<int*>(sub_7693C0(rawMemory, nullptr, a2, a3, a4, a5, a6, a7,
                                               reinterpret_cast<int>(a8),
                                               scratch.words, scratch.words,
                                               scratch.words, scratch.words));
        return CommitResultPointer(self, nullptr, reinterpret_cast<uint32_t*>(&a8));
    }
    else {
        a8 = nullptr;
        return CommitResultPointer(self, nullptr, reinterpret_cast<uint32_t*>(&a8));
    }
}

HOOK(0x0076A230, InputBindings_CreateAndCommitHandle, "auto");
