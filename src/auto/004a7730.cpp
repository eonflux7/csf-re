#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, C_00833908_GetTargetName, 0x0048FE90, (C_00833908* self, void*));
CSF_FUNC(int, __fastcall, C_008342e8_CopyControlPosition, 0x0049ED80, (void* self, void* edx, int a2, uint32_t* a3));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(double, g_DegreesPerTurn, 0x00834E00);
CSF_GLOBAL(float, g_flt_0082DC84, 0x0082DC84);

static C_008342e8* __fastcall AiMountTasks_PrepareMountRotation(
    void* self, void* /*edx*/)
{
    auto* control = reinterpret_cast<C_008342e8*>(
        CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            static_cast<int>(CSF_AT(uint32_t, self, 0x0C) + 256)));
    if (!control)
        return control;

    alignas(8) uint8_t positionBytes[0x28];
    C_008342e8_CopyControlPosition(
        control, nullptr,
        CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x110),
        reinterpret_cast<uint32_t*>(positionBytes));

    void* attachment = reinterpret_cast<void*>(control->vGetField016c());
    int attachmentIndex =
        reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, attachment, 0)[0x98 / 4])(
                attachment, nullptr, CSF_AT(uint32_t, positionBytes, 0x04));

    if (attachmentIndex == -1)
    {
        return reinterpret_cast<C_008342e8*>(
            C_00833908_GetTargetName(
                CSF_AT(C_00833908*, CSF_AT(void*, self, 0x08), 0x118),
                nullptr));
    }

    attachment = reinterpret_cast<void*>(control->vGetField016c());
    void* mountPoint =
        reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, attachment, 0)[0x8C / 4])(
                attachment, nullptr, attachmentIndex);
    void* mountTransform =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, mountPoint, 0)[0x18 / 4])(
                mountPoint, nullptr);

    attachment = reinterpret_cast<void*>(control->vGetField016c());
    void* baseTransform =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, attachment, 0)[0x18 / 4])(
                attachment, nullptr);

    float* mountRotation =
        reinterpret_cast<float*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, mountTransform, 0)[0x48 / 4])(
                mountTransform, nullptr);
    float* baseRotation =
        reinterpret_cast<float*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, baseTransform, 0)[0x48 / 4])(
                baseTransform, nullptr);

    float targetX = mountRotation[0] + baseRotation[0];
    float targetY = mountRotation[1] + baseRotation[1];
    float targetZ = mountRotation[2] + baseRotation[2];

    auto* actor =
        CSF_AT(C_00833908*, CSF_AT(void*, self, 0x08), 0x118);
    void* actorObject = reinterpret_cast<void*>(actor->vGetField016c());
    void* actorTransform =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, actorObject, 0)[0x18 / 4])(
                actorObject, nullptr);
    float* actorRotation =
        reinterpret_cast<float*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, actorTransform, 0)[0x48 / 4])(
                actorTransform, nullptr);

    float deltaX = targetX - actorRotation[0];
    float deltaY = targetY - actorRotation[1];
    float deltaZ = targetZ - actorRotation[2];

    CSF_AT(float, self, 0x14) =
        static_cast<float>(std::fmod(deltaX, g_DegreesPerTurn) * g_flt_0082DC84);
    CSF_AT(float, self, 0x18) =
        static_cast<float>(std::fmod(deltaY, g_DegreesPerTurn) * g_flt_0082DC84);
    double stepZ = std::fmod(deltaZ, g_DegreesPerTurn) * g_flt_0082DC84;
    void* owner = CSF_AT(void*, self, 0x08);
    CSF_AT(int, self, 0x20) = 10;
    CSF_AT(float, self, 0x1C) = static_cast<float>(stepZ);

    actor = CSF_AT(C_00833908*, owner, 0x118);
    return reinterpret_cast<C_008342e8*(__fastcall*)(
        void*, void*, int, int, int, int)>(
            actor->vtbl[0x8C / 4])(actor, nullptr, 38, 0, -1, 0);
}

HOOK(0x004A7730, AiMountTasks_PrepareMountRotation, "auto");
