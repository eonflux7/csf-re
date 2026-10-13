#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, WeaponFirearms_NotifyColorAnimation, 0x00444B20, (void* self, void* , C_008342e8* a2, C_008342e8* a3));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t*, __fastcall, InitializeDispatchRecord, 0x00474810, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(char, __cdecl, S_00526d40_RotateVectorWithStateAngle, 0x0048AD40, (S_00526d40* vector, float angle, uint32_t* out));
CSF_FUNC(C_00833b88*, __fastcall, S_0048b280_TraceShotAndDispatchImpact, 0x0048B650, (S_0048b280* self, void* , int rayAddress, uint32_t weaponId, int shotAddress, char emitShot, C_00833b88* ignoredObject));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(int, __fastcall, CGuiControl_GetColorAnim, 0x004FE4B0, (CGuiControl* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static C_008342e8* __fastcall S_0048b280_CreateSpawn(
    S_0048b280* self, void* /*edx*/, uint32_t* origin,
    uint32_t* direction, int shotCount, float angle, uint32_t weaponId,
    int shotAddress, int useOriginalDirection)
{
    void* shot = reinterpret_cast<void*>(shotAddress);

    alignas(8) uint8_t recordBytes[0x58];
    C_00831678& record = *reinterpret_cast<C_00831678*>(recordBytes);
    alignas(8) uint8_t rayBytes[0x18];
    alignas(8) uint8_t rotatedBytes[0x10];

    CSF_AT(uint32_t, rayBytes, 0x00) = 1065353216;
    CSF_AT(uint32_t, rayBytes, 0x04) = 0;
    CSF_AT(uint32_t, rayBytes, 0x08) = 0;
    CSF_AT(uint32_t, rayBytes, 0x0C) = 0;
    CSF_AT(uint32_t, rayBytes, 0x10) = 0;
    CSF_AT(uint32_t, rayBytes, 0x14) = 0;
    CSF_AT(uint32_t, rotatedBytes, 0x00) = 0;
    CSF_AT(uint32_t, rotatedBytes, 0x04) = 0;
    CSF_AT(uint32_t, rotatedBytes, 0x08) = 0;

    if (shotCount > 1 &&
        CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0))
    {
        InitializeDispatchRecord(&record, nullptr, 22);
        uint32_t owner = CSF_AT(uint32_t, shot, 0x04);
        record.field_0010 = CSF_AT(uint32_t, shot, 0x08);
        record.field_0034 = origin[0];
        record.field_0038 = origin[1];
        record.field_003c = origin[2];
        record.field_000c = owner;
        record.field_0040 = direction[0];
        record.field_0044 = direction[1];
        record.field_0048 = direction[2];
        record.field_0024 = weaponId;
        record.field_004c = weaponId;
        record.field_0050 = shotCount;
        std::memcpy(&record.field_0054, &angle, sizeof(angle));

        void* dispatcher = reinterpret_cast<void*>(GetGlobalObjectField184());
        DispatchRecordToGlobalObject(dispatcher, nullptr, &record, 1);
    }

    C_00833b88* hitObject = nullptr;
    if (shotCount > 0)
    {
        const char emitShot = shotCount == 1;
        do
        {
            int shotKind = 0;
            if (useOriginalDirection ||
                ((shotKind = CSF_AT(int, shot, 0x0C)) != 0 && shotKind != 3))
            {
                CSF_AT(uint32_t, rayBytes, 0x00) = direction[0];
                CSF_AT(uint32_t, rayBytes, 0x04) = direction[1];
                CSF_AT(uint32_t, rayBytes, 0x08) = direction[2];
            }
            else
            {
                S_00526d40_RotateVectorWithStateAngle(
                    reinterpret_cast<S_00526d40*>(direction), angle,
                    reinterpret_cast<uint32_t*>(rotatedBytes));
                CSF_AT(uint32_t, rayBytes, 0x00) =
                    CSF_AT(uint32_t, rotatedBytes, 0x00);
                CSF_AT(uint32_t, rayBytes, 0x04) =
                    CSF_AT(uint32_t, rotatedBytes, 0x04);
                CSF_AT(uint32_t, rayBytes, 0x08) =
                    CSF_AT(uint32_t, rotatedBytes, 0x08);
            }

            CSF_AT(uint32_t, rayBytes, 0x0C) = origin[0];
            CSF_AT(uint32_t, rayBytes, 0x10) = origin[1];
            CSF_AT(uint32_t, rayBytes, 0x14) = origin[2];
            NormalizeVector3(reinterpret_cast<C_00833b88*>(rayBytes), nullptr);

            hitObject = S_0048b280_TraceShotAndDispatchImpact(
                self, nullptr, reinterpret_cast<int>(rayBytes),
                weaponId, shotAddress, emitShot, nullptr);

            if (hitObject && CSF_AT(uint8_t, shot, 0x2E) &&
                (hitObject->field_0170 & 0x2000) != 0)
            {
                S_0048b280_TraceShotAndDispatchImpact(
                    self, nullptr, reinterpret_cast<int>(rayBytes),
                    weaponId, shotAddress, emitShot, hitObject);
            }
            --shotCount;
        } while (shotCount);
    }

    int resultValue = CSF_AT(int, shot, 0x08);
    C_008342e8* result = reinterpret_cast<C_008342e8*>(resultValue);
    if (resultValue >= 0)
    {
        result = reinterpret_cast<C_008342e8*>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr, shotAddress));
        C_008342e8* shooter = result;
        if (shooter)
        {
            void* state = reinterpret_cast<void*>(GetValue8B0554());
            void* weaponTable = CSF_AT(void*, state, 0x1FC0);
            void* weapon =
                ((void* (__fastcall*)(void*, void*, uint32_t))
                 CSF_AT(void**, weaponTable, 0)[2])(
                    weaponTable, nullptr, weaponId);

            int weaponKind = CSF_AT(int, weapon, 0x74);
            result = reinterpret_cast<C_008342e8*>(
                static_cast<uint8_t*>(weapon) + 4);
            if (weaponKind != 9)
            {
                CGuiControl* control = reinterpret_cast<CGuiControl*>(
                    GetGlobalValue_8B0568());
                void* animation = reinterpret_cast<void*>(
                    CGuiControl_GetColorAnim(control, nullptr));
                WeaponFirearms_NotifyColorAnimation(
                    animation, nullptr,
                    reinterpret_cast<C_008342e8*>(hitObject), shooter);
                // The final void call leaves no defined return value.
                return nullptr;
            }
        }
    }
    return result;
}

HOOK(0x48C370, S_0048b280_CreateSpawn, "auto");
