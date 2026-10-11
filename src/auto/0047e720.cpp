#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, AiAttackTasks_AppendEntityHandle, 0x0044B9A0, (void* self, void* edx, int a3));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, AiAttackTasks_BuildMovementPath, 0x0047C950, (void* self, void* edx, int arg0, int a3, void* a4));
CSF_FUNC(int, __fastcall, TaskCharAction_48CFE0, 0x0048CFE0, (void* self, void* edx, int a2, char a3, char a4));
CSF_FUNC(int, __fastcall, IsDistinctNonSentinelValue, 0x0048E0D0, (void* self, void* , std::int32_t candidate));
CSF_FUNC(int, __fastcall, GuiControl_GetPathPointRecord, 0x004A0F30, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CharTask_StartMovement, 0x004DB140, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, FillEntityElementRange, 0x004F0210, (void* self, void* edx, void* a1, void* a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __fastcall, C_0086c12c_ReleaseOwnedBuffer, 0x0075A190, (C_0086c12c* self, void*));
CSF_FUNC(uint32_t*, __fastcall, C_0086c12c_Construct, 0x0075A360, (C_0086c12c* self, void*));
CSF_FUNC(int, __fastcall, AiAttackTasks_FindSceneEntryIndex, 0x00770610, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneEntry, 0x00770690, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_0082f9f8_StartTimedMovement(
    C_0082f9f8* self, void* /*edx*/,
    S_004ef780* entity, unsigned int options, int durationBits)
{
    CSF_AT(unsigned int, self, 0x80) = options;
    CSF_AT(S_004ef780*, self, 0x7C) = entity;
    CSF_AT(int, reinterpret_cast<void*>(self->field_0008), 0x2B0) = durationBits;
    CSF_AT(int, reinterpret_cast<void*>(self->field_0008), 0x2AC) = 0;

    void* loader = CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FDC);
    int sceneIndex = AiAttackTasks_FindSceneEntryIndex(
        loader, nullptr, reinterpret_cast<unsigned int>(entity));

    alignas(8) uint8_t pathBytes[0x14];
    auto& path = *reinterpret_cast<C_0086c12c*>(pathBytes);
    alignas(8) uint8_t filteredBytes[0x10];
    auto& filtered = *reinterpret_cast<S_0044b8c0*>(filteredBytes);
    alignas(8) uint8_t elementsBytes[0x14];
    auto& elements = *reinterpret_cast<S_0044b8c0*>(elementsBytes);

    std::memset(&elements.field_0004, 0, 12);
    FillEntityElementRange(loader, nullptr, entity, &elements);
    std::memset(&filtered.field_0004, 0, 12);

    uint32_t handleOffset = 0;
    for (uint32_t index = 0;
         elements.field_0004 &&
         index < static_cast<uint32_t>(
             static_cast<int32_t>(elements.field_0008 - elements.field_0004) / 12);
         ++index, handleOffset += 12)
    {
        int resolved = CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            static_cast<int>(elements.field_0004 + handleOffset));
        if (resolved)
        {
            int candidate = CSF_AT(int, reinterpret_cast<void*>(resolved), 0x78);
            void* actor = CSF_AT(
                void*, reinterpret_cast<void*>(self->field_0008), 0x14);
            if (static_cast<uint8_t>(
                    IsDistinctNonSentinelValue(actor, nullptr, candidate)))
            {
                AiAttackTasks_AppendEntityHandle(
                    &filtered, nullptr,
                    static_cast<int>(elements.field_0004 + handleOffset));
            }
        }
    }

    C_0086c12c_Construct(&path, nullptr);
    int sceneEntry = SceneLoader_GetSceneEntry(loader, nullptr, sceneIndex);
    AiAttackTasks_BuildMovementPath(
        self, nullptr, sceneEntry, reinterpret_cast<int>(&filtered), &path);

    void* movementPath = reinterpret_cast<uint8_t*>(self) + 0x88;
    ((void(__fastcall*)(void*, void*))
        CSF_AT(void**, movementPath, 0)[0x1C / 4])(movementPath, nullptr);

    uint32_t pointCount = reinterpret_cast<uint32_t>(
        GetCachedObjectProvider(&path, nullptr));
    float totalSquaredDistance = 0.0f;
    for (uint32_t index = 1; index < pointCount; ++index)
    {
        void* previous = reinterpret_cast<void*>(
            GuiControl_GetPathPointRecord(&path, nullptr, static_cast<int>(index - 1)));
        void* current = reinterpret_cast<void*>(
            GuiControl_GetPathPointRecord(&path, nullptr, static_cast<int>(index)));
        float dx = CSF_AT(float, current, 0) - CSF_AT(float, previous, 0);
        float dy = CSF_AT(float, current, 4) - CSF_AT(float, previous, 4);
        float dz = CSF_AT(float, current, 8) - CSF_AT(float, previous, 8);
        totalSquaredDistance = dz * dz + dy * dy + dx * dx + totalSquaredDistance;
    }

    void** pathVtable = CSF_AT(void**, movementPath, 0);
    float accumulatedSquaredDistance = 0.0f;
    int firstPoint = GuiControl_GetPathPointRecord(&path, nullptr, 0);
    ((void(__fastcall*)(void*, void*, int, float))
        pathVtable[0x18 / 4])(movementPath, nullptr, firstPoint, 0.0f);

    if (pointCount > 1)
    {
        float inverseDistance = g_TransitionOne / totalSquaredDistance;
        for (uint32_t index = 1; index < pointCount; ++index)
        {
            void* previous = reinterpret_cast<void*>(
                GuiControl_GetPathPointRecord(&path, nullptr, static_cast<int>(index - 1)));
            void* current = reinterpret_cast<void*>(
                GuiControl_GetPathPointRecord(&path, nullptr, static_cast<int>(index)));
            float dx = CSF_AT(float, current, 0) - CSF_AT(float, previous, 0);
            float dy = CSF_AT(float, current, 4) - CSF_AT(float, previous, 4);
            float dz = CSF_AT(float, current, 8) - CSF_AT(float, previous, 8);
            void* taskState = reinterpret_cast<void*>(self->field_0008);
            pathVtable = CSF_AT(void**, movementPath, 0);
            accumulatedSquaredDistance =
                dz * dz + dy * dy + dx * dx + accumulatedSquaredDistance;
            float time = accumulatedSquaredDistance *
                         CSF_AT(float, taskState, 0x2B0) * inverseDistance;
            int point = GuiControl_GetPathPointRecord(
                &path, nullptr, static_cast<int>(index));
            ((void(__fastcall*)(void*, void*, int, float))
                pathVtable[0x18 / 4])(movementPath, nullptr, point, time);
        }
    }

    void* actor = CSF_AT(void*, reinterpret_cast<void*>(self->field_0008), 0x14);
    TaskCharAction_48CFE0(
        CSF_AT(void*, actor, 0x204), nullptr, reinterpret_cast<int>(self), 7, 1);
    CharTask_StartMovement(self, nullptr, reinterpret_cast<int>(&self->field_0014));

    int result = C_0086c12c_ReleaseOwnedBuffer(&path, nullptr);
    void* filteredBlock = reinterpret_cast<void*>(filtered.field_0004);
    if (filteredBlock)
    {
        uint32_t end = filtered.field_0008;
        for (uint32_t address = filtered.field_0004; address != end; address += 12)
        {
            void* handle = reinterpret_cast<void*>(address);
            ((int(__fastcall*)(void*, void*, int))
                CSF_AT(void**, handle, 0)[0])(handle, nullptr, 0);
        }
        result = Mem_FreeDebug(filteredBlock);
    }

    uint8_t* element = reinterpret_cast<uint8_t*>(elements.field_0004);
    if (element)
    {
        uint8_t* end = reinterpret_cast<uint8_t*>(elements.field_0008);
        if (element != end)
        {
            do
            {
                result = ((int(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, element, 0)[0])(element, nullptr, 0);
                element += 12;
            } while (element != end);
            element = reinterpret_cast<uint8_t*>(elements.field_0004);
        }
        if (element)
            return Mem_FreeDebug(element);
    }
    return result;
}

HOOK(0x0047E720, C_0082f9f8_StartTimedMovement, "auto");
