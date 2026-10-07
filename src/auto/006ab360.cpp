#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, AllocateGatheringStorage, 0x00647160,
         (std::uint32_t size, const char* sourceFile, int sourceLine));

CSF_FUNC(void, __fastcall, C_0085ff6c_ConstructGatheringBase, 0x00798DF0,
         (C_0085ff4c* self, void* edx));

CSF_GLOBAL(void*, g_Commandos3DGameGatheringVtable, 0x0085FF6C);

static C_0085ff4c* __cdecl CreateCommandos3DGameGathering()
{
    auto* gathering = static_cast<C_0085ff4c*>(
        AllocateGatheringStorage(
            600,
            CSF_STR(0x00860028, ".\\Commandos3DGameGatheringDDL.cpp"),
            66));

    if (!gathering)
        return nullptr;

    C_0085ff6c_ConstructGatheringBase(gathering, nullptr);
    gathering->vtbl = &g_Commandos3DGameGatheringVtable;
    return gathering;
}

HOOK(0x006AB360, CreateCommandos3DGameGathering, "auto");
