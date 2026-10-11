#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, GfxScene_PrepareOwnedResource, 0x005655B0, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, RenderHandle_FromObject, 0x006B2D50, (void* self, void* edx));
CSF_FUNC(char, __fastcall, GfxScene_PrepareClump, 0x006BD3D0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(void, __fastcall, GfxScene_PrepareGlobalSceneState, 0x006C5770, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GfxScene_ConstructSceneAttachment, 0x006CB070, (void* self, void* edx));
CSF_FUNC(char, __fastcall, GfxScene_LoadSceneResources, 0x006CB6A0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, GfxScene_InitializeSceneAttachment, 0x006D5FF0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, GfxScene_ActivateRenderHandle, 0x006D6FC0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, GfxScene_PrepareObjectRegistry, 0x006DF860, (void* self, void* edx));
CSF_FUNC(char, __fastcall, GfxScene_InitializeSceneController, 0x006F60F0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, GfxScene_ConstructSceneController, 0x006F62D0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __cdecl, GfxScene_PrepareSceneServices, 0x006F7830, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));

static char __fastcall C_00860bb8_InitializeScene(
    C_00860bb8* self, void* /*edx*/, uint8_t* loadFlags)
{
    const uint8_t wasInitialized = self->field_0068;
    self->field_00f4 = 0;
    if (wasInitialized)
        self->vSlot28();

    GfxScene_PrepareGlobalSceneState(
        reinterpret_cast<void*>(GetGlobalValue_6C5760()), nullptr);

    const char result = GfxScene_LoadSceneResources(
        self, nullptr, reinterpret_cast<int>(loadFlags));

    const int registry = SceneLoader_GetObjectRegistry(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    GfxScene_PrepareObjectRegistry(reinterpret_cast<void*>(registry), nullptr);
    GfxScene_PrepareSceneServices();

    GfxScene_PrepareClump(
        reinterpret_cast<void*>(self->field_0008), nullptr);
    GfxScene_PrepareOwnedResource(
        reinterpret_cast<void*>(self->field_000c), nullptr);

    void* attachmentStorage = Mem_Alloc(164);
    void* attachment = attachmentStorage
        ? GfxScene_ConstructSceneAttachment(attachmentStorage, nullptr)
        : nullptr;

    const int attachmentArgument = static_cast<int>(self->field_00c0);
    self->field_00bc = reinterpret_cast<uint32_t>(attachment);
    GfxScene_InitializeSceneAttachment(
        attachment, nullptr, attachmentArgument);

    void* controllerStorage = Mem_Alloc(160);
    uint32_t* controller = controllerStorage
        ? GfxScene_ConstructSceneController(controllerStorage, nullptr)
        : nullptr;

    self->field_00c4 = reinterpret_cast<uint32_t>(controller);
    GfxScene_InitializeSceneController(controller, nullptr);

    if (*loadFlags)
    {
        self->field_006a = 0;
    }
    else
    {
        const int renderHandle = RenderHandle_FromObject(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr);
        GfxScene_ActivateRenderHandle(
            reinterpret_cast<void*>(renderHandle), nullptr);
        self->field_006a = 1;
    }

    self->field_0068 = 1;
    return result;
}

HOOK(0x006CC0A0, C_00860bb8_InitializeScene, "auto");
