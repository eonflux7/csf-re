#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, SceneLoader_SerializeReference, 0x00442E70, (void* a3, bool* a2));
CSF_FUNC(void**, __fastcall, SceneLoader_RegisterArchivedObject, 0x0069ACB0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, AiAttackTasks_UpdateSlotFromManager, 0x006B32C0, (int owner, S_00691860* slot));

static int __fastcall GfxCamera_Serialize(void* self, void* /*edx*/, void* archive)
{
    auto* camera = static_cast<uint8_t*>(self);

    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0) == 0)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, int, void*))
            CSF_AT(void**, serializer, 0)[32])(serializer, nullptr, 0, archive);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0) = failed;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        AiAttackTasks_UpdateSlotFromManager(
            reinterpret_cast<int>(camera + 8),
            reinterpret_cast<S_00691860*>(archive));
    }

    bool proceed = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (proceed)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        SceneLoader_SerializeReference(camera + 40, reinterpret_cast<bool*>(archive));
    }

    proceed = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (proceed)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[4])(
                    serializer, nullptr, camera + 44, archive) == -1;
    }

    proceed = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (proceed)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[21])(
                    serializer, nullptr, camera + 60, archive) == -1;
    }

    proceed = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (proceed)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[21])(
                    serializer, nullptr, camera + 64, archive) == -1;
    }

    proceed = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (proceed)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[21])(
                    serializer, nullptr, camera + 72, archive) == -1;
    }

    proceed = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (proceed)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[22])(
                    serializer, nullptr, camera + 68, archive) == -1;
    }

    proceed = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (proceed)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[21])(
                    serializer, nullptr, camera + 84, archive) == -1;
    }

    proceed = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (proceed)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[21])(
                    serializer, nullptr, camera + 88, archive) == -1;
    }

    proceed = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (proceed)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[22])(
                    serializer, nullptr, camera + 92, archive) == -1;
    }

    if (CSF_AT(uint8_t, archive, 0x14) == 0 &&
        CSF_AT(uint8_t, archive, 0) == 0)
    {
        void* cameraObject = camera - 8;
        ((void(__fastcall*)(void*, void*, uint32_t))
            CSF_AT(void**, cameraObject, 0)[14])(
                cameraObject, nullptr, CSF_AT(uint32_t, camera, 64));
        ((void(__fastcall*)(void*, void*, uint32_t))
            CSF_AT(void**, cameraObject, 0)[16])(
                cameraObject, nullptr, CSF_AT(uint32_t, camera, 60));
        ((void(__fastcall*)(void*, void*, uint32_t))
            CSF_AT(void**, cameraObject, 0)[8])(
                cameraObject, nullptr, CSF_AT(uint32_t, camera, 72));
        ((void(__fastcall*)(void*, void*, uint32_t))
            CSF_AT(void**, cameraObject, 0)[24])(
                cameraObject, nullptr, CSF_AT(uint32_t, camera, 68));
        SceneLoader_RegisterArchivedObject(
            CSF_AT(void*, archive, 0x10), nullptr,
            reinterpret_cast<uintptr_t>(camera) != 8
                ? reinterpret_cast<int>(camera) : 0);
    }

    proceed = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = savedContext;
    if (proceed)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        const int result = ((int(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, serializer, 0)[31])(serializer, nullptr, archive);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, archive, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, archive, 0) = 1;
    return 1;
}

HOOK(0x006D3DB0, GfxCamera_Serialize, "auto");
