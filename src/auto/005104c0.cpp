#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_510570, 0x00510570, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_510510, 0x00510510, (void* self, void* edx, int a2));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall C_00869cd0_ResetAndAllocateSlot(C_00869cd0* self, void* /*edx*/)
{
    S_00756260* existingBlock = CSF_AT(S_00756260*, self, 0x04);
    if (existingBlock) {
        sub_510570(existingBlock, nullptr);
        Mem_Free(existingBlock);
        CSF_AT(S_00756260*, self, 0x04) = nullptr;
    }

    S_00756260* newBlock = static_cast<S_00756260*>(Mem_Alloc(16));
    if (newBlock) {
        int result = sub_510510(newBlock, nullptr, 0);
        CSF_AT(S_00756260*, self, 0x04) = newBlock;
        return result;
    }

    CSF_AT(S_00756260*, self, 0x04) = nullptr;
    return 0;
}

HOOK(0x005104C0, C_00869cd0_ResetAndAllocateSlot, "auto");
