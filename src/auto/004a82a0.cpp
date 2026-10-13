#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_0082f9f8_BeginTaskAtTarget, 0x00420A70, (C_0082f9f8* self, void* , uint32_t* target, int param));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(double, __fastcall, C_008342e8_ComputeResolvedObjectScalar, 0x0048F220, (C_008342e8* self, void*));
CSF_FUNC(int, __fastcall, C_00833908_GetTargetName, 0x0048FE90, (C_00833908* self, void*));
CSF_FUNC(int, __fastcall, C_008342e8_CopyControlPosition, 0x0049ED80, (void* self, void* edx, int a2, uint32_t* a3));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_DegreesToRadians, 0x00834818);

static char __fastcall AiMountTasks_BeginTaskAtResolvedTarget(void* self, void* /*edx*/)
{
    void* configuration = CSF_AT(void*, self, 0xC);
    auto* mount = reinterpret_cast<C_008342e8*>(
        CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(static_cast<uint8_t*>(configuration) + 0x100)));

    if (!mount)
        return 0;

    void* owner = CSF_AT(void*, self, 0x8);
    void* attachedObject = CSF_AT(void*, owner, 0x118);
    auto* task = CSF_AT(C_0082f9f8*, attachedObject, 0x30);
    configuration = CSF_AT(void*, self, 0xC);

    alignas(8) uint8_t positionBytes[0x14];
    CSF_AT(C_0082f9f8*, positionBytes, 0xC) = task;

    C_008342e8_CopyControlPosition(
        mount, nullptr, CSF_AT(int, configuration, 0x110),
        reinterpret_cast<uint32_t*>(positionBytes));

    void* controls = reinterpret_cast<void*>(mount->vGetField016c());
    int targetIndex =
        ((int(__fastcall*)(void*, void*, C_0082f9f8*))
            CSF_AT(void**, controls, 0)[0x98 / 4])(
                controls, nullptr,
                CSF_AT(C_0082f9f8*, positionBytes, 0x8));

    if (targetIndex == -1)
    {
        owner = CSF_AT(void*, self, 0x8);
        return static_cast<char>(C_00833908_GetTargetName(
            CSF_AT(C_00833908*, owner, 0x118), nullptr));
    }

    controls = reinterpret_cast<void*>(mount->vGetField016c());
    void* target =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, controls, 0)[0x8C / 4])(controls, nullptr);

    void* orientation =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, target, 0)[0x18 / 4])(target, nullptr);

    float angle = static_cast<float>(
        ((double(__fastcall*)(void*, void*))
            CSF_AT(void**, orientation, 0)[0x28 / 4])(orientation, nullptr));

    angle = static_cast<float>(
        C_008342e8_ComputeResolvedObjectScalar(mount, nullptr) + angle);

    orientation =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, target, 0)[0x18 / 4])(target, nullptr);

    uint32_t* taskTarget =
        ((uint32_t*(__fastcall*)(void*, void*, float))
            CSF_AT(void**, orientation, 0)[0x10 / 4])(
                orientation, nullptr, angle * g_DegreesToRadians);

    return C_0082f9f8_BeginTaskAtTarget(
        CSF_AT(C_0082f9f8*, positionBytes, 0xC),
        nullptr, taskTarget, targetIndex);
}

HOOK(0x004A82A0, AiMountTasks_BeginTaskAtResolvedTarget, "auto");
