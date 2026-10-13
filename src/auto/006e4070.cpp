#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpHAnimHierarchyGetMatrixArray, 0x0056CF80, (int a1));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetSector, 0x006D4720, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, GfxScene_ResolveResource, 0x006D59B0, (void* self, void* edx, char* String2));
CSF_FUNC(char, __stdcall, GfxIm3d_PrepareHierarchyData, 0x006E3F10, (int a1, int a2));
CSF_FUNC(int, __cdecl, GetHAnimChildFrameCount, 0x006E8090, (int a1));
CSF_FUNC(int*, __cdecl, HAnim_BuildParentIndexTable, 0x006E8340, (void* hierarchy));
CSF_FUNC(int, __cdecl, HAnimForwardToRwEngineSlot67, 0x006E83D0, (int a1));
CSF_FUNC(int, __cdecl, GfxHAnim_CopyFirstDwordsToNewArray, 0x006E83F0, (int hierarchy));
CSF_FUNC(int, __cdecl, GfxIm3d_ApplyHierarchyData, 0x0081C790, (int a1, int a2, int a3, unsigned int a4));

static char __fastcall C_00861b40_ConfigureFromEntry(
    C_00861b40* self, void* /*edx*/, std::uint32_t* entry)
{
    char success = 1;

    int globalObject = GetGlobalInt();
    int accessor = GlobalInt_ResolveAccessor(
        reinterpret_cast<void*>(globalObject), nullptr);
    void* provider = GetCachedObjectProvider(
        reinterpret_cast<void*>(accessor), nullptr);

    char* resourceName;
    if (CSF_AT(std::uint32_t, entry, 0x38) < 0x10u)
        resourceName = reinterpret_cast<char*>(entry) + 0x24;
    else
        resourceName = CSF_AT(char*, entry, 0x24);

    unsigned int resource = GfxScene_ResolveResource(
        provider, nullptr, resourceName);
    void* sector = reinterpret_cast<void*>(
        SceneLoader_GetSector(provider, nullptr, static_cast<int>(resource)));

    if (CSF_AT(std::uint32_t, sector, 0x28) != 0)
    {
        sector = reinterpret_cast<void*>(
            SceneLoader_GetSector(provider, nullptr, static_cast<int>(resource)));
        void* frameReference = CSF_AT(void*, sector, 0x20);
        int hierarchy = GetHAnimChildFrameCount(
            CSF_AT(int, frameReference, 0));

        RpHAnimHierarchyGetMatrixArray(hierarchy);
        int hierarchyData = GfxHAnim_CopyFirstDwordsToNewArray(hierarchy);
        int* parentIndices = HAnim_BuildParentIndexTable(
            reinterpret_cast<void*>(hierarchy));

        *parentIndices = -1;
        int nodeCount = CSF_AT(int, reinterpret_cast<void*>(hierarchy), 0x4);

        success = GfxIm3d_PrepareHierarchyData(hierarchyData, nodeCount);
        if (success)
        {
            GfxIm3d_ApplyHierarchyData(
                static_cast<int>(self->field_0004),
                hierarchyData,
                reinterpret_cast<int>(parentIndices),
                static_cast<unsigned int>(nodeCount));
        }
        else
        {
            success = 0;
        }

        HAnimForwardToRwEngineSlot67(reinterpret_cast<int>(parentIndices));
        HAnimForwardToRwEngineSlot67(hierarchyData);
    }

    if (success)
    {
        int secondArgument = CSF_AT(int, entry, 0x58);
        int firstArgument = CSF_AT(int, entry, 0x5C);
        self->vSlot14(firstArgument, secondArgument);
    }

    return success;
}

HOOK(0x006E4070, C_00861b40_ConfigureFromEntry, "auto");
