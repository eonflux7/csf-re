#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpMatFXMaterialSetUVTransformMatrices, 0x00571920, (int a1, int a2, int a3));
CSF_FUNC(int, __fastcall, IsFlagBit3Set, 0x006B8180, (void* self, void*));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(bool, __fastcall, GfxObjectProps_HasNonZeroOffsetXY, 0x006B8B40, (void* self, void*));
CSF_FUNC(char, __cdecl, GfxEnvironment_SetSectorObjectFlag4, 0x006C0160, (int a1, char a2));
CSF_FUNC(int, __fastcall, C_0085e7b8_FindEntryPayloadByKey, 0x006C4870, (C_0085e7b8* self, void* , int key));
CSF_FUNC(int*, __fastcall, C_0085e7b8_ResolveChainedValue, 0x006C48C0, (C_0085e7b8* self, void* , int* a2, int a3));
CSF_FUNC(int, __cdecl, SkinGeometry_SetMaterialAlpha, 0x006C4E60, (int atomicPtr, int alpha));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(char, __fastcall, GlobalHud_Commit_6C7C30, 0x006C7C30, (C_0085e7b8* self, void*));
CSF_FUNC(int, __fastcall, C_0085e7b8_AttachWidget, 0x006C8420, (C_0085e7b8* self, void* , C_0085e7b8* widget));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);

static char __cdecl GfxUvAnim_RenderSkinSubObject(int atomicPtr, C_0085e7b8* widget)
{
    void* attachment =
        reinterpret_cast<void*>(GfxObjectProps_GetObjectAttachment(atomicPtr));
    if (attachment)
    {
        const char skip = static_cast<char>(IsFlagBit3Set(attachment, nullptr));
        if (skip)
            return skip;
    }

    C_0085e7b8* hud = CSF_AT(
        C_0085e7b8*, reinterpret_cast<void*>(GetGlobalValue_6C5760()), 0x4);
    C_0085e7b8_AttachWidget(hud, nullptr, widget);

    alignas(8) uint8_t opacityBytes[0x4] = {};
    int* opacityStorage = reinterpret_cast<int*>(opacityBytes);
    *opacityStorage =
        *C_0085e7b8_ResolveChainedValue(widget, nullptr, opacityStorage, 3);

    float opacity;
    std::memcpy(&opacity, opacityBytes, sizeof(opacity));
    if (opacity < g_TransitionOne)
    {
        SkinGeometry_SetMaterialAlpha(
            atomicPtr,
            static_cast<int>(static_cast<int64_t>(opacity * g_ColorChannelScale)));
    }

    void* uvTransform = nullptr;
    if (GfxObjectProps_HasNonZeroOffsetXY(attachment, nullptr))
    {
        void* entry = reinterpret_cast<void*>(
            C_0085e7b8_FindEntryPayloadByKey(widget, nullptr, 7));
        if (entry)
        {
            uvTransform = CSF_AT(void*, entry, 0x0);
            if (uvTransform)
            {
                RpMatFXMaterialSetUVTransformMatrices(
                    CSF_AT(int, uvTransform, 0x0),
                    CSF_AT(int, uvTransform, 0x4),
                    0);
            }
        }
    }

    GfxEnvironment_SetSectorObjectFlag4(atomicPtr, 1);
    using RenderCallback = void (__cdecl*)(int);
    CSF_AT(RenderCallback, reinterpret_cast<void*>(atomicPtr), 0x48)(atomicPtr);
    GfxEnvironment_SetSectorObjectFlag4(atomicPtr, 0);

    if (uvTransform)
        RpMatFXMaterialSetUVTransformMatrices(CSF_AT(int, uvTransform, 0x0), 0, 0);

    std::memcpy(&opacity, opacityBytes, sizeof(opacity));
    if (opacity < g_TransitionOne)
        SkinGeometry_SetMaterialAlpha(atomicPtr, 255);

    return GlobalHud_Commit_6C7C30(hud, nullptr);
}

HOOK(0x006EFB20, GfxUvAnim_RenderSkinSubObject, "auto");
