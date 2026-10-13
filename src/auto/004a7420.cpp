#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CMenuMpGameMode_GetSceneEntry, 0x0040F790, (CMenuMpGameMode* self, void* , int index));
CSF_FUNC(char, __fastcall, C_0082dd78_StoreByteAndSetFlag, 0x0040FD20, (C_0082dd78* self, void* , char a2));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, C_00833908_GetTargetName, 0x0048FE90, (C_00833908* self, void*));
CSF_FUNC(int, __fastcall, AiMountTasks_SetMountTarget, 0x00492AE0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, C_008342e8_CopyControlPosition, 0x0049ED80, (void* self, void* edx, int a2, uint32_t* a3));

CSF_GLOBAL(float, g_flt_0082DC84, 0x0082DC84);
CSF_GLOBAL(double, g_DegreesPerTurn, 0x00834E00);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall AiMountTasks_InitializeMountAlignment(void* self, void* /*edx*/)
{
    void* parameters = CSF_AT(void*, self, 0x0C);
    auto* mount = reinterpret_cast<C_008342e8*>(
        CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(static_cast<uint8_t*>(parameters) + 256)));
    if (!mount)
        return 0;

    parameters = CSF_AT(void*, self, 0x0C);
    int controlIndex = CSF_AT(int, parameters, 0x110);
    void* owner = CSF_AT(void*, self, 0x08);
    void* actor = CSF_AT(void*, owner, 0x118);
    auto* actorState = CSF_AT(C_0082dd78*, actor, 0x4C);
    AiMountTasks_SetMountTarget(
        actor, nullptr,
        reinterpret_cast<int>(static_cast<uint8_t*>(parameters) + 256),
        controlIndex);
    C_0082dd78_StoreByteAndSetFlag(actorState, nullptr, 0);

    parameters = CSF_AT(void*, self, 0x0C);
    int entryIndex = CSF_AT(int, parameters, 0x10C);
    auto* mountState = reinterpret_cast<C_0082dd78*>(mount->field_004c);
    void* entry = reinterpret_cast<void*>(
        CMenuMpGameMode_GetSceneEntry(
            reinterpret_cast<CMenuMpGameMode*>(mountState), nullptr, entryIndex));

    if (!CSF_AT(uint8_t, entry, 0x0C)
        && CSF_AT(uint8_t, entry, 0x0D)
        && (CSF_AT(int, entry, 0x10) != -1
            || CSF_AT(int, entry, 0x14) != -1))
    {
        parameters = CSF_AT(void*, self, 0x0C);
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, mountState, 0)[0x48 / 4])(
                mountState, nullptr, CSF_AT(int, parameters, 0x10C));
    }

    CSF_AT(int, self, 0x20) = 0;
    if (CSF_AT(int, entry, 0) == 0)
    {
        alignas(8) uint8_t positionBytes[0x28];
        parameters = CSF_AT(void*, self, 0x0C);
        C_008342e8_CopyControlPosition(
            mount, nullptr, CSF_AT(int, parameters, 0x110),
            reinterpret_cast<uint32_t*>(positionBytes));

        void* mountObject = reinterpret_cast<void*>(mount->vGetField016c());
        int partIndex =
            ((int(__fastcall*)(void*, void*, uint32_t))
                CSF_AT(void**, mountObject, 0)[0x98 / 4])(
                    mountObject, nullptr, CSF_AT(uint32_t, positionBytes, 4));

        if (partIndex == -1)
        {
            owner = CSF_AT(void*, self, 0x08);
            C_00833908_GetTargetName(
                CSF_AT(C_00833908*, owner, 0x118), nullptr);
        }
        else
        {
            mountObject = reinterpret_cast<void*>(mount->vGetField016c());
            void* part =
                ((void*(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, mountObject, 0)[0x8C / 4])(
                        mountObject, nullptr, partIndex);
            void* partTransform =
                ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, part, 0)[0x18 / 4])(part, nullptr);

            mountObject = reinterpret_cast<void*>(mount->vGetField016c());
            void* mountTransform =
                ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, mountObject, 0)[0x18 / 4])(
                        mountObject, nullptr);

            float* partAngles =
                ((float*(__fastcall*)(void*, void*))
                    CSF_AT(void**, partTransform, 0)[0x48 / 4])(
                        partTransform, nullptr);
            float* mountAngles =
                ((float*(__fastcall*)(void*, void*))
                    CSF_AT(void**, mountTransform, 0)[0x48 / 4])(
                        mountTransform, nullptr);

            float targetAngles[3] = {
                partAngles[0] + mountAngles[0],
                partAngles[1] + mountAngles[1],
                partAngles[2] + mountAngles[2]
            };

            owner = CSF_AT(void*, self, 0x08);
            actor = CSF_AT(void*, owner, 0x118);
            void* actorObject =
                ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, actor, 0)[0x28 / 4])(actor, nullptr);
            void* actorTransform =
                ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, actorObject, 0)[0x18 / 4])(
                        actorObject, nullptr);
            float* actorAngles =
                ((float*(__fastcall*)(void*, void*))
                    CSF_AT(void**, actorTransform, 0)[0x48 / 4])(
                        actorTransform, nullptr);

            float angleDelta[3] = {
                targetAngles[0] - actorAngles[0],
                targetAngles[1] - actorAngles[1],
                targetAngles[2] - actorAngles[2]
            };

            CSF_AT(float, self, 0x14) = static_cast<float>(
                std::fmod(angleDelta[0], g_DegreesPerTurn) * g_flt_0082DC84);
            float secondStep = static_cast<float>(
                std::fmod(angleDelta[2], g_DegreesPerTurn) * g_flt_0082DC84);
            CSF_AT(int, self, 0x20) = 10;
            CSF_AT(float, self, 0x1C) = secondStep;
        }
    }

    owner = CSF_AT(void*, self, 0x08);
    actor = CSF_AT(void*, owner, 0x118);
    return ((int(__fastcall*)(void*, void*, int, int, int, int))
        CSF_AT(void**, actor, 0)[0x8C / 4])(actor, nullptr, 36, 0, -1, 0);
}

HOOK(0x004A7420, AiMountTasks_InitializeMountAlignment, "auto");
