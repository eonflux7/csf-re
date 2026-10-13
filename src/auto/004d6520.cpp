#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CScene_RemoveObjectSlot, 0x00453390, (CScene* self, void* , int a2));

static char __fastcall C_0083c388_NotifySceneMenuAndRemoveSlot(C_0083c388* self, void* /*edx*/, int a2, char a3)
{
    void* sceneMenuContext = reinterpret_cast<void*>(GetSceneMenuContext(g_pCurrentScene, nullptr));
    void* menuObject = CSF_AT(void*, sceneMenuContext, 0x44);
    // Slot 172 / 4 = 43 on the menu object's vtable.
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, menuObject, 0)[43])(menuObject, nullptr, a2);

    char result = a3;
    if (a3) {
        result = static_cast<char>(CScene_RemoveObjectSlot(
            reinterpret_cast<CScene*>(g_pCurrentScene), nullptr,
            static_cast<int>(self->field_0014 + 0x1c)));
    }
    self->field_0040 = 1;
    return result;
}

HOOK(0x004D6520, C_0083c388_NotifySceneMenuAndRemoveSlot, "auto");
