#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, StateRecord_Reset, 0x00401870, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_558150, 0x00558150, ());
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, GetSpawnFlagFromGlobal, 0x0073CD30, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_MaxPercent, 0x0082E120);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static C_00833b88* __fastcall SceneState_TickInteraction(
    uint32_t* self, void* /*edx*/, float elapsed)
{
    alignas(8) uint8_t recordBytes[0x3C];
    C_0082d478& record = *reinterpret_cast<C_0082d478*>(recordBytes);
    uint32_t recordState = 0;

    if (!CSF_AT(uint8_t, CSF_AT(void*, self, 0x00), 0xBC))
        goto BuildRecord;

    {
        void* owner = CSF_AT(void*, self, 0x00);
        void* context = CSF_AT(void*, owner, 0x08);
        if (CSF_AT(uint32_t, context, 0x244) != CSF_AT(uint32_t, self, 0x14))
            goto BuildRecord;

        void* settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        owner = CSF_AT(void*, self, 0x00);
        context = CSF_AT(void*, owner, 0x08);
        if (CSF_AT(float, context, 0x258) > CSF_AT(float, settings, 0xB1C))
            goto BuildRecord;
    }

    if (CSF_AT(int32_t, self, 0x18) < 0)
        goto MissingRecord;

    {
        int resolved = CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x10));
        if (!resolved)
            goto MissingRecord;

        uint32_t component = CSF_AT(uint32_t, reinterpret_cast<void*>(resolved), 0x18);
        if (!component)
            goto MissingRecord;

        void* actor = reinterpret_cast<void*>(component - 1712);
        if (!actor)
            goto MissingRecord;

        int spawnFlag = CSF_AT(int, actor, 0x668);
        int globals = GetGlobalDword_73C020();
        int currentSpawnFlag = GetSpawnFlagFromGlobal(reinterpret_cast<void*>(globals), nullptr);
        if ((spawnFlag != currentSpawnFlag && CSF_AT(int, actor, 0x668) != -1)
            || (CSF_AT(uint8_t, actor, 0x453) & 1) == 0)
        {
            goto CancelRecord;
        }

        spawnFlag = CSF_AT(int, actor, 0x668);
        globals = GetGlobalDword_73C020();
        currentSpawnFlag = GetSpawnFlagFromGlobal(reinterpret_cast<void*>(globals), nullptr);
        if (spawnFlag == currentSpawnFlag)
        {
            if (CSF_AT(uint8_t, actor, 0x458) == 1)
            {
                void* values = reinterpret_cast<void*>(GetGlobalValue_558150());
                CSF_AT(uint32_t, self, 0x04) = CSF_AT(uint32_t, values, 0x1A4);
            }
            else
            {
                void* values = reinterpret_cast<void*>(GetGlobalValue_558150());
                CSF_AT(uint32_t, self, 0x04) = CSF_AT(uint32_t, values, 0x1A8);
            }
        }

        float remaining = CSF_AT(float, self, 0x08) - elapsed;
        CSF_AT(float, self, 0x08) = remaining;
        if (remaining < g_fZero)
        {
            recordState = 1;
            goto BuildRecord;
        }

        float progress = g_MaxPercent
            - remaining / CSF_AT(float, self, 0x0C) * g_MaxPercent;
        void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
        CSF_AT(float, globalState, 0xFC) = progress;

        C_00833b88* result =
            reinterpret_cast<C_00833b88*>(CSF_AT(uint32_t, self, 0x18));
        if (CSF_AT(int32_t, self, 0x18) >= 0)
        {
            result = reinterpret_cast<C_00833b88*>(CHandleTable_Resolve(
                g_pCurrentScene, nullptr,
                reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x10)));
            if (result)
            {
                int inactive = SceneObj_IsInactive(result, nullptr);
                result = reinterpret_cast<C_00833b88*>(inactive);
                if (static_cast<uint8_t>(inactive))
                {
                    void* owner = CSF_AT(void*, self, 0x00);
                    reinterpret_cast<C_00833b88*(__fastcall*)(void*, void*, int, int)>(
                        CSF_AT(void**, owner, 0x00)[0x134 / 4])(
                            owner, nullptr, 0, 0);

                    StateRecord_Reset(&record, nullptr, 32);
                    uint32_t handle = CSF_AT(uint32_t, self, 0x18);
                    uint32_t parameter = CSF_AT(uint32_t, self, 0x04);
                    record.field_000c = CSF_AT(uint32_t, self, 0x14);
                    record.field_0010 = handle;
                    record.field_002c = 0;
                    record.field_0024 = parameter;

                    void* dispatcher = reinterpret_cast<void*>(GetGlobalObjectField184());
                    return reinterpret_cast<C_00833b88*>(
                        DispatchRecordToGlobalObject(dispatcher, nullptr, &record, 0));
                }
            }
        }
        return result;
    }

BuildRecord:
    {
        uint32_t handle = CSF_AT(uint32_t, self, 0x18);
        uint32_t parameter = CSF_AT(uint32_t, self, 0x04);
        record.field_0008 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
        record.field_0014 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
        record.field_000c = CSF_AT(uint32_t, self, 0x14);
        record.vtbl = CSF_VTBL(0x0082D478);
        record.field_0018 = -1;
        record.field_001c = -1;
        record.field_0004 = 32;
        record.field_0020 = 0;
        record.field_0028 = 0;
        record.field_0030 = 0;
        record.field_0010 = handle;
        record.field_002c = recordState;
        record.field_0024 = parameter;
    }
    goto DispatchAndExit;

MissingRecord:
    if (CSF_AT(int32_t, self, 0x04) == -1)
        goto CancelRecord;

    StateRecord_Reset(&record, nullptr, 32);
    record.field_000c = -1;
    record.field_0010 = -1;
    record.field_002c = 2;
    record.field_0024 = CSF_AT(uint32_t, self, 0x04);
    goto DispatchAndExit;

CancelRecord:
    StateRecord_Reset(&record, nullptr, 32);
    {
        uint32_t handle = CSF_AT(uint32_t, self, 0x18);
        uint32_t parameter = CSF_AT(uint32_t, self, 0x04);
        record.field_000c = CSF_AT(uint32_t, self, 0x14);
        record.field_0010 = handle;
        record.field_002c = 0;
        record.field_0024 = parameter;
    }

DispatchAndExit:
    {
        void* dispatcher = reinterpret_cast<void*>(GetGlobalObjectField184());
        DispatchRecordToGlobalObject(dispatcher, nullptr, &record, 0);
        void* owner = CSF_AT(void*, self, 0x00);
        return reinterpret_cast<C_00833b88*(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, owner, 0x00)[0x134 / 4])(
                owner, nullptr, 0, 0);
    }
}

HOOK(0x0048D9C0, SceneState_TickInteraction, "auto");
