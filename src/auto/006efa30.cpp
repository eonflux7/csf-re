#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpAtomicGetWorldBoundingSphere, 0x0058EC60, (int a1));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GetAttachmentRegionOverrideFlag, 0x006B8150, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetField04Bit1, 0x006B8160, (void* self, void*));
CSF_FUNC(int, __fastcall, AttachmentRecord_Resolve, 0x006B8230, (void* self, void* edx));
CSF_FUNC(double, __fastcall, GetAttachmentFirstWeight, 0x006B8290, (void* self, void* edx, char a2));
CSF_FUNC(double, __fastcall, GetAttachmentSecondWeight, 0x006B82C0, (void* self, void* edx, char a2));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(char, __cdecl, GfxSkin_ApplyDistanceFade, 0x006C5430, (void* sceneObject, void* viewObject));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static char __cdecl GfxUvAnim_IsAtomicVisible(int atomic)
{
    void* globalState = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    void* view = CSF_AT(void*, globalState, 0xF0);
    int worldSphere = RpAtomicGetWorldBoundingSphere(atomic);
    auto* attachment = reinterpret_cast<C_0085ec18*>(
        GfxObjectProps_GetObjectAttachment(atomic));

    bool regionOverride =
        static_cast<uint8_t>(GetAttachmentRegionOverrideFlag(attachment, nullptr)) != 0;

    char visible = static_cast<char>(
        (reinterpret_cast<int(__fastcall*)(void*, void*, int, uint32_t*, bool)>(
            CSF_AT(void**, view, 0)[0x6C / 4]))(
            view, nullptr, worldSphere,
            reinterpret_cast<uint32_t*>(
                reinterpret_cast<uint8_t*>(attachment) + 0x1C),
            regionOverride));

    if (!visible)
        return visible;

    float firstWeight =
        static_cast<float>(GetAttachmentFirstWeight(attachment, nullptr, 1));
    double secondWeight = GetAttachmentSecondWeight(attachment, nullptr, 1);

    if (firstWeight > g_fZero || secondWeight > g_fZero)
        visible = GfxSkin_ApplyDistanceFade(reinterpret_cast<void*>(atomic), view);

    if (visible &&
        static_cast<uint8_t>(GetField04Bit1(attachment, nullptr)) != 0)
    {
        void* sceneLoader = reinterpret_cast<void*>(GetGlobalInt());
        void* sceneController = reinterpret_cast<void*>(
            SceneLoader_GetSceneController(sceneLoader, nullptr));
        void* sceneRegion = CSF_AT(void*, sceneController, 0xBC);

        if (AttachmentRecord_Resolve(attachment, nullptr) == 1 &&
            !CSF_AT(uint8_t, sceneRegion, 0x34))
            return 0;

        if (AttachmentRecord_Resolve(attachment, nullptr) == 0 &&
            CSF_AT(uint8_t, sceneRegion, 0x34))
            return 0;
    }

    return visible;
}

HOOK(0x006EFA30, GfxUvAnim_IsAtomicVisible, "auto");
