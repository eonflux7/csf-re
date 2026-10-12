#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __fastcall, DynamicObj_ConfigureObjectList, 0x004427C0, (void* self, void* edx, unsigned int a2, int a3, int a4, int a5, int a6));
CSF_FUNC(int, __fastcall, DynamicObj_ConfigureSectorLists, 0x00442840, (void* self, void* edx, unsigned int a2, int a3, int a4, int a5, int a6));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetSectorRecord, 0x00762470, (void* self, void* , int sectorIndex));

static auto const kArrayCapacityReference = CSF_VTBL(0x0041E5F0);
static auto const kObjectListReference = CSF_VTBL(0x004427C0);

static int* __fastcall S_004427c0_InitializeSectorObjectLists(
    S_004427c0* self, void* /*edx*/)
{
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;

    auto* sectorLists = reinterpret_cast<uint8_t*>(self) + 0x10;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 0;

    auto* sceneLoader = CSF_AT(
        C_0086f128*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FC8);

    DynamicObj_ConfigureSectorLists(sectorLists, nullptr, 0x23u, 0, 0, 0, 10);

    for (uint32_t sectorIndex = 0;; ++sectorIndex)
    {
        auto* records = reinterpret_cast<void*>(sceneLoader->field_0008);
        const uint32_t recordsBegin = CSF_AT(uint32_t, records, 0x04);
        if (!recordsBegin)
            break;

        const int32_t recordBytes = static_cast<int32_t>(
            CSF_AT(uint32_t, records, 0x08) - recordsBegin);
        const int32_t scaledCount = static_cast<int32_t>(
            (0x75DED953LL * recordBytes) >> 32) >> 8;
        const uint32_t recordCount = static_cast<uint32_t>(scaledCount)
            + (static_cast<uint32_t>(scaledCount) >> 31);
        if (sectorIndex >= recordCount)
            break;

        auto* record = reinterpret_cast<void*>(
            SceneLoader_GetSectorRecord(
                sceneLoader, nullptr, static_cast<int>(sectorIndex)));
        const int object = CSF_AT(int, record, 0x00);

        const uint32_t listsBegin = CSF_AT(uint32_t, sectorLists, 0x04);
        const uint32_t listCount = listsBegin
            ? static_cast<uint32_t>(static_cast<int32_t>(
                  CSF_AT(uint32_t, sectorLists, 0x08) - listsBegin) >> 4)
            : 0;

        const uint32_t listIndex = CSF_AT(uint32_t, record, 0xB4);
        if (listIndex < listCount)
        {
            auto* list = reinterpret_cast<S_0041e5f0*>(
                CSF_AT(uint32_t, self, 0x14) + 16 * listIndex);

            if (list->field_0008 >= list->field_0004)
                S_0041e5f0_GrowArrayCapacity(
                    list, nullptr, static_cast<int>(list->field_000c));

            const uint32_t count = list->field_0008;
            auto* destination = reinterpret_cast<int*>(
                list->field_0000 + 4 * count);
            list->field_0008 = count + 1;
            *destination = object;
        }
    }

    DynamicObj_ConfigureObjectList(self, nullptr, 0x23u, 0, 0, 0, 10);
    return reinterpret_cast<int*>(self);
}

HOOK(0x004428C0, S_004427c0_InitializeSectorObjectLists, "auto");
