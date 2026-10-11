#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, MpCharacterSelectState_Destroy, 0x005554A0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_GLOBAL(int32_t, g_Int8B0688, 0x008B0688);

// Finalizes the multiplayer character-select menu: if its state block exists,
// runs the state teardown, frees the block through the game heap, and clears
// the global pointer. Returns the heap free result, or 0 when nothing was allocated.
static int __cdecl MenuMpCharacterSelect_Finalize()
{
    if (g_Int8B0688 == 0)
        return 0;

    void* stateBlock = reinterpret_cast<void*>(static_cast<uintptr_t>(g_Int8B0688));
    MpCharacterSelectState_Destroy(stateBlock, nullptr);
    int freeResult = Mem_Free(stateBlock);
    g_Int8B0688 = 0;
    return freeResult;
}

HOOK(0x555790, MenuMpCharacterSelect_Finalize, "auto");
