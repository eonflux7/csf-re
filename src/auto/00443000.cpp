#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, DynamicObj_GetResourceId, 0x00442A20, (int a1));
CSF_FUNC(int, __cdecl, DynamicObj_GetResourceCollection, 0x00442A80, (int a1));
CSF_FUNC(void, __cdecl, DynamicObj_ConfigureResource, 0x00442AD0, (int a1, int a2, uint32_t* a3));
CSF_FUNC(void*, __stdcall, DynamicObj_ActivateRecord, 0x00442BB0, (int a1));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveRwManager, 0x006B13B0, (int managerId, void* edx));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_FloatMax, 0x00831090);

static int __fastcall DynamicObj_ProcessSubObjectRecord(
    void* self, void* /*edx*/, uint32_t* input)
{
    void* record = nullptr;
    bool activateWithoutResource = false;
    bool isNew = false;

    // Retain the packet's array-growth vtable reference.
    (void)CSF_VTBL(0x0041E5F0);

    for (uint32_t index = 0;
         index < CSF_AT(uint32_t, self, 0x14) && !record;
         ++index)
    {
        void* candidate = CSF_AT(void**, self, 0x0C)[index];
        if (CSF_AT(uint32_t, candidate, 0x04) == CSF_AT(uint32_t, input, 0x00))
            record = candidate;
    }

    if (!record)
    {
        record = Mem_Alloc(44);
        if (record)
        {
            CSF_AT(void*, record, 0x00) =
                reinterpret_cast<void*>(CSF_ADDR_DATA(0x00831058));
            CSF_AT(int, record, 0x04) = -1;
            CSF_AT(int, record, 0x08) = -1;
            CSF_AT(uint32_t, record, 0x0C) = 0;
            CSF_AT(uint32_t, record, 0x10) = 0;
            CSF_AT(uint32_t, record, 0x14) = 0;
            CSF_AT(uint32_t, record, 0x18) = 0;
            CSF_AT(uint32_t, record, 0x1C) = 0;
            CSF_AT(uint8_t, record, 0x20) = 0;
            CSF_AT(uint32_t, record, 0x24) = 0;
            CSF_AT(uint32_t, record, 0x28) = 0;
        }
        CSF_AT(uint32_t, record, 0x04) = CSF_AT(uint32_t, input, 0x00);
        isNew = true;
    }

    CSF_AT(uint32_t, record, 0x10) = 0;
    CSF_AT(uint32_t, record, 0x14) = CSF_AT(uint32_t, input, 0x08);
    CSF_AT(float, record, 0x18) =
        CSF_AT(float, input, 0x04) < g_fZero
            ? g_FloatMax
            : CSF_AT(float, input, 0x08) + CSF_AT(float, input, 0x04);

    if (CSF_AT(uint32_t, input, 0x00) == 1)
    {
        CSF_AT(uint32_t, record, 0x1C) = CSF_AT(uint32_t, input, 0x0C);
        CSF_AT(uint8_t, record, 0x20) = CSF_AT(uint8_t, input, 0x10);
    }
    else if (CSF_AT(uint32_t, input, 0x00) == 2)
    {
        CSF_AT(uint32_t, record, 0x28) = CSF_AT(uint32_t, input, 0x0C);
    }

    const int resourceId = DynamicObj_GetResourceId(CSF_AT(int, input, 0x00));
    if (resourceId == -1)
    {
        activateWithoutResource = true;
    }
    else
    {
        if (!CSF_AT(uint32_t, record, 0x24))
        {
            alignas(8) uint8_t requestBytes[0x10] = {};
            CSF_AT(int, requestBytes, 0x00) = resourceId;
            CSF_AT(uint32_t, requestBytes, 0x04) = 0;

            const int globalManager = GetGlobalInt();
            void* manager = reinterpret_cast<void*>(
                ResolveRwManager(globalManager, nullptr));
            const int resource =
                ((int(__fastcall*)(void*, void*, int*))
                    CSF_AT(void**, manager, 0x00)[0x20 / 4])(
                        manager, nullptr, reinterpret_cast<int*>(requestBytes));
            CSF_AT(int, record, 0x24) = resource;
            if (!resource)
                return -1;
        }
        DynamicObj_ConfigureResource(
            CSF_AT(int, record, 0x24), resourceId, input);
    }

    if (isNew)
    {
        CSF_AT(uint32_t, record, 0x08) = CSF_AT(uint32_t, self, 0x08);
        ++CSF_AT(uint32_t, self, 0x08);

        if (CSF_AT(uint32_t, self, 0x14) >= CSF_AT(uint32_t, self, 0x10))
        {
            S_0041e5f0_GrowArrayCapacity(
                reinterpret_cast<S_0041e5f0*>(
                    static_cast<uint8_t*>(self) + 0x0C),
                nullptr, CSF_AT(int, self, 0x18));
        }

        const uint32_t index = CSF_AT(uint32_t, self, 0x14);
        void** destination = CSF_AT(void**, self, 0x0C) + index;
        CSF_AT(uint32_t, self, 0x14) = index + 1;
        *destination = record;
    }
    else
    {
        void* collection = reinterpret_cast<void*>(
            DynamicObj_GetResourceCollection(CSF_AT(int, record, 0x04)));
        if (collection)
        {
            const uint32_t count =
                ((uint32_t(__fastcall*)(void*, void*))
                    CSF_AT(void**, collection, 0x00)[0x38 / 4])(
                        collection, nullptr);
            bool found = false;
            for (uint32_t index = 0; index < count; ++index)
            {
                const int resource =
                    ((int(__fastcall*)(void*, void*, uint32_t))
                        CSF_AT(void**, collection, 0x00)[0x34 / 4])(
                            collection, nullptr, index);
                found = resource == CSF_AT(int, record, 0x24);
                if (found)
                    break;
            }
            if (!found)
            {
                ((void(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, collection, 0x00)[0x30 / 4])(
                        collection, nullptr, CSF_AT(int, record, 0x24));
            }
        }
    }

    if (activateWithoutResource ||
        (isNew && CSF_AT(float, record, 0x14) == g_fZero))
    {
        DynamicObj_ActivateRecord(reinterpret_cast<int>(record));
    }

    const int recordIndex = CSF_AT(int, record, 0x08);
    if (!CSF_AT(uint32_t, record, 0x04))
    {
        void* resource = CSF_AT(void*, record, 0x24);
        const int enabled =
            ((int(__fastcall*)(void*, void*, int))
                CSF_AT(void**, resource, 0x00)[0x20 / 4])(
                    resource, nullptr, 1);
        if (enabled)
            CSF_AT(uint32_t, record, 0x0C) |= 4;
        else
            CSF_AT(uint32_t, record, 0x0C) &= 0xFFFFFFFB;
    }
    return recordIndex;
}

HOOK(0x00443000, DynamicObj_ProcessSubObjectRecord, "auto");
