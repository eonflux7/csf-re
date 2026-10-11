#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __fastcall, C_6B81C0_GetFirstItem, 0x006B81C0, (void* self, void*));

using ResourceSearchCallback = int (__cdecl*)(std::uint32_t, int);

CSF_FUNC(bool, __fastcall, GfxScene_VisitResources, 0x006BCA60,
         (void* self, void* edx, ResourceSearchCallback callback, int context));
CSF_FUNC(int, __fastcall, C_00860bb8_CreateClumpObject, 0x006CB8C0,
         (void* self, void* edx, int resource));

static int __fastcall C_00860bb8_FindOrAddClump(
    C_00860bb8* self, void* /*edx*/, const void* name)
{
    int result = -1;
    std::uint32_t index = 0;
    bool searching = true;

    if (self->field_005c != 0)
    {
        while (searching)
        {
            void* clump = CSF_AT(
                void*, reinterpret_cast<void*>(self->field_0054), index * 4);

            void* items =
                reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, clump, 0)[0x1C / 4])(clump, nullptr);

            int firstItem = C_6B81C0_GetFirstItem(items, nullptr);
            if (firstItem != 0)
            {
                void* item = reinterpret_cast<void*>(firstItem);
                unsigned int itemLength = CSF_AT(unsigned int, item, 0x14);
                if (itemLength != 0)
                {
                    unsigned int nameLength =
                        CSF_AT(unsigned int, name, 0x14);
                    const char* nameData =
                        CSF_AT(unsigned int, name, 0x18) < 0x10u
                            ? reinterpret_cast<const char*>(name) + 4
                            : CSF_AT(const char*, name, 0x04);

                    if (CompareStringSubstring(
                            item, nullptr, 0, itemLength,
                            nameData, nameLength) == 0)
                    {
                        result = static_cast<int>(index);
                        searching = false;
                    }
                }
            }

            ++index;
            if (index >= self->field_005c)
                break;
        }
    }

    if (!searching)
        return result;

    alignas(8) std::uint8_t searchBytes[0x28];
    C_0082d410& search =
        *reinterpret_cast<C_0082d410*>(searchBytes);

    CSF_AT(std::uint32_t, searchBytes, 0x18) = 15;
    CSF_AT(std::uint32_t, searchBytes, 0x14) = 0;
    reinterpret_cast<std::uint8_t*>(&search.field_0004)[0] = 0;

    AssignStringSubstring(&search, nullptr, name, 0, -1);

    CSF_AT(int, searchBytes, 0x1C) = 0;
    GfxScene_VisitResources(
        reinterpret_cast<void*>(self->field_0008), nullptr,
        reinterpret_cast<ResourceSearchCallback>(CSF_ADDR_FN(0x006CCE70)),
        reinterpret_cast<int>(searchBytes));

    int resource = CSF_AT(int, searchBytes, 0x1C);
    if (resource != 0)
    {
        int clump = C_00860bb8_CreateClumpObject(self, nullptr, resource);

        if (self->field_005c >= CSF_AT(std::uint32_t, self, 0x58))
        {
            // Retain the binding identified by the packet.
            (void)CSF_VTBL(0x0041E5F0);
            S_0041e5f0_GrowArrayCapacity(
                reinterpret_cast<S_0041e5f0*>(
                    reinterpret_cast<std::uint8_t*>(self) + 0x54),
                nullptr, CSF_AT(int, self, 0x60));
        }

        std::uint32_t count = self->field_005c;
        int* slot = reinterpret_cast<int*>(self->field_0054) + count;
        self->field_005c = count + 1;
        *slot = clump;
        result = static_cast<int>(self->field_005c - 1);
    }

    if (CSF_AT(std::uint32_t, searchBytes, 0x18) >= 0x10)
    {
        void* buffer = reinterpret_cast<void*>(search.field_0004);
        if (buffer != nullptr)
            Mem_FreeDebug(buffer);
    }

    return result;
}

HOOK(0x006CD7D0, C_00860bb8_FindOrAddClump, "auto");
