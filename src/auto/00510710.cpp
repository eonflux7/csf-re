#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ReleaseSlotBlockResources, 0x005107C0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, InitializeSlotBlock, 0x00510760, (void* self, void* edx, int a2));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall C_00872abc_ResetAndAllocateSlot(C_00872abc* self, void* /*edx*/)
{
    void* existingBlock = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0004));
    if (existingBlock) {
        ReleaseSlotBlockResources(existingBlock, nullptr);
        Mem_Free(existingBlock);
        self->field_0004 = 0;
    }

    void* newBlock = Mem_Alloc(16);
    if (newBlock) {
        int result = InitializeSlotBlock(newBlock, nullptr, 0);
        self->field_0004 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(newBlock));
        return result;
    }

    self->field_0004 = 0;
    return 0;
}

HOOK(0x00510710, C_00872abc_ResetAndAllocateSlot, "auto");
