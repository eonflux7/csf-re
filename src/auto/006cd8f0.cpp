#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

using ClumpVisitor = int (__cdecl*)(std::uint32_t, int);

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, C_6B81C0_GetFirstItem, 0x006B81C0, (void* self, void*));
CSF_FUNC(bool, __fastcall, GfxScene_VisitClumps, 0x006BCA60, (void* self, void* edx, ClumpVisitor visitor, int context));
CSF_FUNC(int, __fastcall, sub_6CB8C0, 0x006CB8C0, (void* self, void* edx, int a2));

static void __fastcall C_00860bb8_FindOrLoadClumpIndices(
    C_00860bb8* self, void* /*edx*/, void* name, S_0041e5f0* indices)
{
    (void)CSF_VTBL(0x0041E5F0);

    indices->field_0008 = 0;
    for (std::uint32_t index = 0; index < self->field_005c; ++index)
    {
        void* clump = reinterpret_cast<void**>(self->field_0054)[index];
        void* collection =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, clump, 0)[0x1C / 4])(clump, nullptr);
        void* firstItem = reinterpret_cast<void*>(
            C_6B81C0_GetFirstItem(collection, nullptr));

        if (firstItem && CSF_AT(std::uint32_t, firstItem, 0x14))
        {
            const std::uint32_t nameCapacity =
                CSF_AT(std::uint32_t, name, 0x18);
            const std::uint32_t nameLength =
                CSF_AT(std::uint32_t, name, 0x14);
            const char* nameData = nameCapacity < 0x10u
                ? reinterpret_cast<const char*>(name) + 4
                : CSF_AT(const char*, name, 0x04);

            if (!CompareStringSubstring(
                    firstItem, nullptr, 0,
                    CSF_AT(std::uint32_t, firstItem, 0x14),
                    nameData, nameLength))
            {
                if (indices->field_0008 >= indices->field_0004)
                    S_0041e5f0_GrowArrayCapacity(
                        indices, nullptr, indices->field_000c);

                const std::uint32_t count = indices->field_0008;
                auto* destination =
                    reinterpret_cast<std::uint32_t*>(indices->field_0000) + count;
                indices->field_0008 = count + 1;
                *destination = index;
            }
        }
    }

    if (indices->field_0008)
        return;

    alignas(8) std::uint8_t searchBytes[0x34];
    C_0082d410& search = *reinterpret_cast<C_0082d410*>(searchBytes);

    CSF_AT(std::uint32_t, searchBytes, 0x20) = 0;
    CSF_AT(std::uint32_t, searchBytes, 0x24) = 0;
    CSF_AT(std::uint32_t, searchBytes, 0x28) = 0;
    CSF_AT(std::uint32_t, searchBytes, 0x2C) = 10;
    CSF_AT(std::uint32_t, searchBytes, 0x18) = 15;
    CSF_AT(std::uint32_t, searchBytes, 0x14) = 0;
    reinterpret_cast<std::uint8_t*>(&search.field_0004)[0] = 0;

    AssignStringSubstring(&search, nullptr, name, 0, -1);
    CSF_AT(void*, searchBytes, 0x1C) = searchBytes + 0x20;

    GfxScene_VisitClumps(
        reinterpret_cast<void*>(self->field_0008), nullptr,
        reinterpret_cast<ClumpVisitor>(CSF_ADDR_FN(0x006CCEE0)),
        reinterpret_cast<int>(&search));

    if (CSF_AT(std::uint32_t, searchBytes, 0x28))
    {
        auto* clumps = reinterpret_cast<S_0041e5f0*>(&self->field_0054);
        std::uint32_t matchIndex = 0;
        do
        {
            const int loadedClump = sub_6CB8C0(
                self, nullptr,
                CSF_AT(int*, searchBytes, 0x20)[matchIndex]);

            if (clumps->field_0008 >= clumps->field_0004)
                S_0041e5f0_GrowArrayCapacity(
                    clumps, nullptr, clumps->field_000c);

            const std::uint32_t clumpCount = clumps->field_0008;
            auto* clumpDestination =
                reinterpret_cast<int*>(clumps->field_0000) + clumpCount;
            clumps->field_0008 = clumpCount + 1;
            *clumpDestination = loadedClump;

            const std::uint32_t addedIndex = self->field_005c - 1;
            if (indices->field_0008 >= indices->field_0004)
                S_0041e5f0_GrowArrayCapacity(
                    indices, nullptr, indices->field_000c);

            const std::uint32_t count = indices->field_0008;
            auto* destination =
                reinterpret_cast<std::uint32_t*>(indices->field_0000) + count;
            indices->field_0008 = count + 1;
            *destination = addedIndex;

            ++matchIndex;
        }
        while (matchIndex < CSF_AT(std::uint32_t, searchBytes, 0x28));
    }

    if (CSF_AT(std::uint32_t, searchBytes, 0x18) >= 0x10 &&
        search.field_0004)
    {
        Mem_FreeDebug(reinterpret_cast<void*>(search.field_0004));
    }

    void* matches = CSF_AT(void*, searchBytes, 0x20);
    CSF_AT(std::uint32_t, searchBytes, 0x18) = 15;
    CSF_AT(std::uint32_t, searchBytes, 0x14) = 0;
    reinterpret_cast<std::uint8_t*>(&search.field_0004)[0] = 0;
    if (matches)
        Mem_Free(matches);
}

HOOK(0x006CD8F0, C_00860bb8_FindOrLoadClumpIndices, "auto");
