#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(uint32_t, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_4AD600, 0x004AD600, (void* self, void* edx, void* a2, uint32_t* a3));

// Estatico (guard AI) task update: computes the guard task for the linked object
// and the current scene's position vector, then hands both to the task dispatcher.
static int __stdcall Estatico_UpdateGuardTask(C_0085e7b8* a1)
{
    // First linked-object lookup: base pointer minus 4 (or 0 if unlinked),
    // plus the task-slot offset 1476 (0x5C4) passed to UnnamedApply_4DC180.
    int linkedObjectValue = GetLinkedObjectValue_4DB5C0(a1, nullptr);
    uint8_t* linkedBase = nullptr;
    if (linkedObjectValue)
        linkedBase = reinterpret_cast<uint8_t*>(linkedObjectValue) - 4;
    UnnamedApply_4DC180(a1, nullptr, reinterpret_cast<int>(linkedBase) + 1476);

    // Second lookup: result discarded by the original, but the call is kept.
    GetLinkedObjectValue_4DB5C0(a1, nullptr);

    // Third lookup: base used as the `this` for the final dispatcher call.
    int linkedObjectValueForTask = GetLinkedObjectValue_4DB5C0(a1, nullptr);
    uint8_t* taskBase = nullptr;
    if (linkedObjectValueForTask)
        taskBase = reinterpret_cast<uint8_t*>(linkedObjectValueForTask) - 4;

    uint32_t sceneMenuContext = GetSceneMenuContext(g_pCurrentScene, nullptr);
    uint32_t objectPositionVector =
        GetObjectPositionVector(reinterpret_cast<void*>(sceneMenuContext), nullptr);

    void* cachedObjectProvider = GetCachedObjectProvider(a1, nullptr);

    return sub_4AD600(taskBase + 1476, nullptr, cachedObjectProvider,
                      reinterpret_cast<uint32_t*>(objectPositionVector));
}

HOOK(0x004AFF10, Estatico_UpdateGuardTask, "auto");
