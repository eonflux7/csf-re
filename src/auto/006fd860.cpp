#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, RpClumpClone, 0x0058FC60, (int a1));
CSF_FUNC(int*, __cdecl, RpClumpStreamGetSize, 0x0058FD10, (int a1));
CSF_FUNC(int, __cdecl, RpClumpStreamRead, 0x0058FFC0, (int a1));
CSF_FUNC(int, __cdecl, RwStreamClose, 0x00629810, (uint32_t* a1, uint32_t* a2));
CSF_FUNC(uint32_t*, __cdecl, RwStreamOpen, 0x006298E0, (int a3, int a4, int a5));
CSF_FUNC(int, __cdecl, RwStreamFindChunk, 0x00629AE0, (uint32_t* a1, int a2, uint32_t* a3, unsigned int* a4));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(char, __cdecl, Path_ReplaceExtension, 0x006947D0, (char* Str, int a2, char* a3));
CSF_FUNC(unsigned int, __cdecl, ResourceName_Transform, 0x00694B20, (char* a1, char* a2, char* a3, unsigned int a4));
CSF_FUNC(int, __cdecl, CoreFiles_CheckPageAvailability, 0x00698B20, (const char* path, char bypassList));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GfxObjectProps_VisitAtomicsThenProcessMaterials, 0x006BBF30, (int clumpHandle));
CSF_FUNC(void, __stdcall, ClumpLoader_RegisterClump, 0x006D4380, (int a1, int a2));
CSF_FUNC(int, __stdcall, ClumpLoader_BuildClumpMetadata, 0x006D43C0, (int a1, int a2, char a3));

static int __fastcall C_00862fcc_LoadClumpAndCreateInstances(
    C_00862fcc* self, void* /*edx*/, int resourceHandle)
{
    auto* resource = reinterpret_cast<uint8_t*>(resourceHandle);
    int result = CSF_AT(int, resource, 0x28);

    // Address classified as a vtable reference in the supplied packet.
    (void)CSF_VTBL(0x0041E5F0);

    if (result == 0)
    {
        alignas(8) char texturePath[0x200];
        alignas(8) char clumpPath[0x208];

        char* name = CSF_AT(uint32_t, resource, 0x18) < 0x10u
            ? reinterpret_cast<char*>(resource + 4)
            : CSF_AT(char*, resource, 4);
        ResourceName_Transform(
            name, const_cast<char*>(CSF_STR(0x00860C74, "Textures/")),
            texturePath, 512);

        void* accessor = reinterpret_cast<void*>(
            GlobalInt_ResolveAccessor(
                reinterpret_cast<void*>(GetGlobalInt()), nullptr));
        void* textureManager =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, accessor, 0)[0x10 / 4])(accessor, nullptr);
        reinterpret_cast<void (__fastcall*)(void*, void*, char*)>(
            CSF_AT(void**, textureManager, 0)[0x10 / 4])(
                textureManager, nullptr, texturePath);

        name = CSF_AT(uint32_t, resource, 0x18) < 0x10u
            ? reinterpret_cast<char*>(resource + 4)
            : CSF_AT(char*, resource, 4);
        std::strcpy(clumpPath, name);
        Path_ReplaceExtension(
            clumpPath, 512, const_cast<char*>(CSF_STR(0x0086294C, ".rpc")));

        int clump = 0;
        uint32_t* stream = nullptr;
        if (CSF_AT(char, reinterpret_cast<void*>(GetGlobalInt()), 4))
        {
            CoreFiles_CheckPageAvailability(clumpPath, 0);
        }
        else
        {
            stream = RwStreamOpen(2, 1, reinterpret_cast<int>(clumpPath));
        }

        if (!stream)
        {
            name = CSF_AT(uint32_t, resource, 0x18) < 0x10u
                ? reinterpret_cast<char*>(resource + 4)
                : CSF_AT(char*, resource, 4);
            stream = RwStreamOpen(2, 1, reinterpret_cast<int>(name));
            result = reinterpret_cast<int>(stream);
        }

        if (stream)
        {
            if (RwStreamFindChunk(stream, 16, nullptr, nullptr))
                clump = RpClumpStreamRead(reinterpret_cast<int>(stream));
            result = RwStreamClose(stream, nullptr);
        }

        if (!clump)
        {
            CSF_AT(int, resource, 0x1C) = 0;
        }
        else
        {
            GfxObjectProps_VisitAtomicsThenProcessMaterials(clump);

            if (CSF_AT(uint32_t, resource, 0x28) >=
                CSF_AT(uint32_t, resource, 0x24))
            {
                S_0041e5f0_GrowArrayCapacity(
                    reinterpret_cast<S_0041e5f0*>(resource + 0x20),
                    nullptr, CSF_AT(int, resource, 0x2C));
            }

            uint32_t count = CSF_AT(uint32_t, resource, 0x28);
            int* slot = CSF_AT(int*, resource, 0x20) + count;
            CSF_AT(uint32_t, resource, 0x28) = count + 1;
            *slot = clump;

            ClumpLoader_RegisterClump(resourceHandle, clump);
            char pagedFiles = CSF_AT(
                char, reinterpret_cast<void*>(GetGlobalInt()), 4);
            CSF_AT(int, resource, 0x34) =
                ClumpLoader_BuildClumpMetadata(resourceHandle, clump, pagedFiles);

            ++CSF_AT(uint32_t, self, 0x18);
            result = reinterpret_cast<int>(RpClumpStreamGetSize(clump));
            CSF_AT(uint32_t, self, 0x14) += static_cast<uint32_t>(result);
        }
    }

    while (CSF_AT(int, resource, 0x1C) > CSF_AT(int, resource, 0x28))
    {
        int* clumps = CSF_AT(int*, resource, 0x20);
        int clone = RpClumpClone(*clumps);
        uint32_t count = CSF_AT(uint32_t, resource, 0x28);
        uint32_t capacity = CSF_AT(uint32_t, resource, 0x24);

        if (count >= capacity)
        {
            uint32_t growth = CSF_AT(uint32_t, resource, 0x2C);
            void* expandedArray = Mem_Alloc(4 * (growth + capacity));

            count = CSF_AT(uint32_t, resource, 0x28);
            if (count)
                std::memcpy(
                    expandedArray, CSF_AT(void*, resource, 0x20), 4 * count);

            void* oldArray = CSF_AT(void*, resource, 0x20);
            CSF_AT(uint32_t, resource, 0x24) += growth;
            if (oldArray)
            {
                Mem_Free(oldArray);
                CSF_AT(void*, resource, 0x20) = nullptr;
            }
            CSF_AT(void*, resource, 0x20) = expandedArray;
        }

        count = CSF_AT(uint32_t, resource, 0x28);
        int* slot = CSF_AT(int*, resource, 0x20) + count;
        CSF_AT(uint32_t, resource, 0x28) = count + 1;
        *slot = clone;
        result = CSF_AT(int, resource, 0x1C);
    }

    return result;
}

HOOK(0x006FD860, C_00862fcc_LoadClumpAndCreateInstances, "auto");
