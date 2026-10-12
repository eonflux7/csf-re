#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetWorldObjectCollection, 0x006B6AF0, (void* self, void* edx));

// Returns TRUE when the cover-attack task's world object collection reports type 32 (attack blocked).
static int __stdcall AiCoverAttackTasks_IsAttackBlocked(C_0085e7b8* a1) {
    int result = 0;
    if (GetCachedObjectProvider(a1, nullptr)) {
        void* provider = GetCachedObjectProvider(a1, nullptr);
        int worldObjectCollectionAddr = SceneLoader_GetWorldObjectCollection(provider, nullptr);
        void* worldObjectCollection = reinterpret_cast<void*>(worldObjectCollectionAddr);
        // Virtual slot at byte offset 0x1C -> index 7.
        int collectionType = ((int(__fastcall*)(void*, void*))CSF_AT(void**, worldObjectCollection, 0)[7])(worldObjectCollection, nullptr);
        if (collectionType == 0x20) {
            return 1;
        }
    }
    return result;
}

HOOK(0x004AA440, AiCoverAttackTasks_IsAttackBlocked, "auto");
