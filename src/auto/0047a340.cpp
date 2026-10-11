#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, DestroySceneObjectOptionsStorage, 0x004018E0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, InitializeSceneObjectOptions, 0x004777D0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_008317c8_Initialize, 0x00479350, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

static int __fastcall C_008318c8_Initialize(
    C_008318c8* self, void* /*edx*/, int a2, int a3)
{
    C_008317c8_Initialize(self, nullptr, a2, a3);

    auto* objectName = reinterpret_cast<uint8_t*>(self->field_0040) + 60;
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008318C8);
    self->field_007c = 0;
    self->field_0080 = 0;
    self->field_0084 = 0;
    self->field_0088 = 0;

    alignas(4) uint8_t request[0x18] = {};
    CSF_AT(uint32_t, request, 0x00) =
        CSF_AT(uint32_t, objectName, 0x18) < 0x10u
        ? static_cast<uint32_t>(reinterpret_cast<uintptr_t>(objectName + 4))
        : CSF_AT(uint32_t, objectName, 0x04);
    CSF_AT(uint8_t, request, 0x04) = 1;
    CSF_AT(uint32_t, request, 0x08) = 0;
    CSF_AT(uint32_t, request, 0x0C) = 0;
    CSF_AT(uint32_t, request, 0x10) = 0;
    CSF_AT(uint32_t, request, 0x14) = 0;

    void* loader = reinterpret_cast<void*>(GetGlobalInt());
    void* controller =
        reinterpret_cast<void*>(SceneLoader_GetSceneController(loader, nullptr));
    uint32_t sceneObject =
        reinterpret_cast<uint32_t(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, controller, 0)[0x14 / 4])(
                controller, nullptr, request);
    self->field_0078 = sceneObject;

    if (sceneObject)
    {
        alignas(4) uint8_t options[0xB8] = {};
        InitializeSceneObjectOptions(options, nullptr);

        uint32_t flags = CSF_AT(uint32_t, options, 0);
        void* object = reinterpret_cast<void*>(self->field_0078);
        CSF_AT(uint32_t, options, 0) = (flags & 0xFFFFFF36) | 9;
        reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, object, 0)[0x10 / 4])(
                object, nullptr, options);

        if (CSF_AT(uint32_t, options, 0x90) >= 0x10)
        {
            void* block = CSF_AT(void*, options, 0x7C);
            if (block)
                Mem_FreeDebug(block);
        }
        CSF_AT(uint32_t, options, 0x90) = 15;
        CSF_AT(uint32_t, options, 0x8C) = 0;
        CSF_AT(uint8_t, options, 0x7C) = 0;
        DestroySceneObjectOptionsStorage(options + 4, nullptr);
    }

    return static_cast<int>(reinterpret_cast<uintptr_t>(self));
}

HOOK(0x0047A340, C_008318c8_Initialize, "auto");
