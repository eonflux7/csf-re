#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
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
CSF_FUNC(unsigned int, __cdecl, ResourceName_Transform, 0x00694B20, (char* a1, char* a2, char* a3, unsigned int a4));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GfxObjectProps_VisitAtomicsThenProcessMaterials, 0x006BBF30, (int clumpHandle));
CSF_FUNC(int, __cdecl, GfxClumpDict_ProcessLoadedClumpFrame, 0x006D4320, (int a1, int a2));
CSF_FUNC(int, __stdcall, GfxClumpDict_BuildClumpMetadata, 0x006D43C0, (int a1, int a2, char a3));
CSF_FUNC(int, __cdecl, GfxHAnim_CreateBindPoseAnimation, 0x006E81A0, (int object));

static int __fastcall C_00861068_LoadAndCloneClumps(
    C_00861068* self, void* /*edx*/, int entryHandle)
{
    void* entry = reinterpret_cast<void*>(entryHandle);
    int result = CSF_AT(int, entry, 0x28);

    if (!result)
    {
        alignas(8) uint8_t texturePathBytes[0x208];
        char* texturePath = reinterpret_cast<char*>(texturePathBytes);
        char* inlineName = reinterpret_cast<char*>(entry) + 4;
        char* resourceName = CSF_AT(uint32_t, entry, 0x18) < 0x10u
            ? inlineName
            : CSF_AT(char*, entry, 4);

        ResourceName_Transform(
            resourceName,
            const_cast<char*>(CSF_STR(0x00860C74, "Textures/")),
            texturePath, 512);

        void* accessor = reinterpret_cast<void*>(
            GlobalInt_ResolveAccessor(
                reinterpret_cast<void*>(GetGlobalInt()), nullptr));
        void* textureManager = reinterpret_cast<void*>(
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, accessor, 0)[0x10 / 4])(accessor, nullptr));
        reinterpret_cast<void(__fastcall*)(void*, void*, char*)>(
            CSF_AT(void**, textureManager, 0)[0x10 / 4])(
                textureManager, nullptr, texturePath);

        int clump = 0;
        resourceName = CSF_AT(uint32_t, entry, 0x18) < 0x10u
            ? inlineName
            : CSF_AT(char*, entry, 4);
        uint32_t* stream = RwStreamOpen(
            2, 1, reinterpret_cast<int>(resourceName));
        result = reinterpret_cast<int>(stream);

        if (stream)
        {
            if (RwStreamFindChunk(stream, 16, nullptr, nullptr))
                clump = RpClumpStreamRead(reinterpret_cast<int>(stream));
            result = RwStreamClose(stream, nullptr);
        }

        if (clump)
        {
            GfxObjectProps_VisitAtomicsThenProcessMaterials(clump);

            if (CSF_AT(uint32_t, entry, 0x28) >=
                CSF_AT(uint32_t, entry, 0x24))
            {
                (void)CSF_VTBL(0x0041E5F0);
                S_0041e5f0_GrowArrayCapacity(
                    reinterpret_cast<S_0041e5f0*>(
                        reinterpret_cast<uint8_t*>(entry) + 0x20),
                    nullptr, CSF_AT(int, entry, 0x2C));
            }

            uint32_t count = CSF_AT(uint32_t, entry, 0x28);
            int* destination = CSF_AT(int*, entry, 0x20) + count;
            CSF_AT(uint32_t, entry, 0x28) = count + 1;
            *destination = clump;

            CSF_AT(int, entry, 0x30) =
                GfxHAnim_CreateBindPoseAnimation(clump);

            void* global = reinterpret_cast<void*>(GetGlobalInt());
            if (!CSF_AT(uint8_t, global, 4))
            {
                GfxClumpDict_ProcessLoadedClumpFrame(
                    CSF_AT(int, reinterpret_cast<void*>(clump), 4), clump);
            }

            global = reinterpret_cast<void*>(GetGlobalInt());
            CSF_AT(int, entry, 0x34) = GfxClumpDict_BuildClumpMetadata(
                entryHandle, clump, CSF_AT(char, global, 4));

            ++self->field_0018;
            result = reinterpret_cast<int>(RpClumpStreamGetSize(clump));
            self->field_0014 += static_cast<uint32_t>(result);
        }
        else
        {
            CSF_AT(int, entry, 0x1C) = 0;
        }
    }

    if (CSF_AT(int, entry, 0x1C) > CSF_AT(int, entry, 0x28))
    {
        do
        {
            int clone = RpClumpClone(CSF_AT(int*, entry, 0x20)[0]);
            uint32_t count = CSF_AT(uint32_t, entry, 0x28);
            uint32_t capacity = CSF_AT(uint32_t, entry, 0x24);

            if (count >= capacity)
            {
                uint32_t growth = CSF_AT(uint32_t, entry, 0x2C);
                void* replacement = Mem_Alloc(4 * (growth + capacity));

                uint32_t copyCount = CSF_AT(uint32_t, entry, 0x28);
                if (copyCount)
                {
                    std::memcpy(
                        replacement, CSF_AT(void*, entry, 0x20),
                        4 * copyCount);
                }

                capacity = CSF_AT(uint32_t, entry, 0x24);
                void* oldStorage = CSF_AT(void*, entry, 0x20);
                CSF_AT(uint32_t, entry, 0x24) = capacity + growth;
                if (oldStorage)
                {
                    Mem_Free(oldStorage);
                    CSF_AT(void*, entry, 0x20) = nullptr;
                }
                CSF_AT(void*, entry, 0x20) = replacement;
            }

            count = CSF_AT(uint32_t, entry, 0x28);
            int* destination = CSF_AT(int*, entry, 0x20) + count;
            CSF_AT(uint32_t, entry, 0x28) = count + 1;
            *destination = clone;
            result = CSF_AT(int, entry, 0x1C);
        }
        while (result > CSF_AT(int, entry, 0x28));
    }

    return result;
}

HOOK(0x006D47E0, C_00861068_LoadAndCloneClumps, "auto");
