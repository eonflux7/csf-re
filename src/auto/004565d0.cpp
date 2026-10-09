#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, SceneEntry_InitBlock, 0x00455F40, (void* self, void* edx, char a2));

static CScene* SceneLoader_AdvanceEntry() {
    CScene* result = reinterpret_cast<CScene*>(Mem_Alloc(436));
    if (result) {
        return reinterpret_cast<CScene*>(SceneEntry_InitBlock(result, nullptr, 0));
    }
    return result;
}

HOOK(0x4565D0, SceneLoader_AdvanceEntry, "auto");
