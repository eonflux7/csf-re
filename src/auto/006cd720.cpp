#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CharActionFsm_GetProviderFrameObject, 0x006C0500, (int a1));
CSF_FUNC(int, __cdecl, SceneLoader_GetObjectAttachment, 0x006B87A0, (int a1));
CSF_FUNC(int, __fastcall, DName_IsPinPtr, 0x006B81B0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_6B81C0_GetFirstItem, 0x006B81C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GfxScene_IndexOfItem, 0x006CCE10, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(void*, __fastcall, GfxScene_InsertItem, 0x006CD460, (void* self, void* edx, void* a2, int a3));

// Adds the first item of a scene object's attachment to the scene list when it is not already present.
static int __cdecl GfxScene_AddFrameItemIfMissing(int a1, S_006cd2d0* a2)
{
    const uint8_t requirePinCheck = a2->field_0004;
    S_006cd2d0* sceneState = reinterpret_cast<S_006cd2d0*>(a2->field_0000);

    int frameObject = CharActionFsm_GetProviderFrameObject(a1);
    if (frameObject == 0)
        return a1;

    void* attachment = reinterpret_cast<void*>(SceneLoader_GetObjectAttachment(frameObject));

    if (!requirePinCheck || (DName_IsPinPtr(attachment, nullptr) & 0xFF) != 0)
    {
        C_0082d410* firstItem = reinterpret_cast<C_0082d410*>(C_6B81C0_GetFirstItem(attachment, nullptr));
        if (firstItem && firstItem[1].field_0008 != 0
            && GfxScene_IndexOfItem(sceneState, nullptr, reinterpret_cast<uint32_t*>(firstItem)) == -1)
        {
            GfxScene_InsertItem(sceneState, nullptr, firstItem, 0);
        }
    }
    return a1;
}

HOOK(0x006CD720, GfxScene_AddFrameItemIfMissing, "auto");
