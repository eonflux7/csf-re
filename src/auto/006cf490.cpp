#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(std::uint32_t, __cdecl, CloseSceneFile, 0x00697980, (void* file));
CSF_FUNC(void*, __cdecl, CoreFiles_OpenSceneFile, 0x00698350, (const char* filename, char* mode, int openFlags));
CSF_FUNC(int, __fastcall, GfxCollisionShapes_FindWorldObject, 0x006CDF20, (void* self, void* edx, char* a2));
CSF_FUNC(unsigned int, __fastcall, GfxScene_PrepareWorldObjectCollection, 0x006CE180, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GfxCollisionShapes_DestroyObjectRange, 0x006CE270, (int a1, int a2));
CSF_FUNC(int, __fastcall, GfxCollisionShapes_ClearWorldObjects, 0x006CEA20, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxCollisionShapes_SwapObjectCollections, 0x006CEAC0, (void* self, void* edx, void* a4a));
CSF_FUNC(uint32_t*, __fastcall, GfxCollisionShapes_AppendWorldObject, 0x006CED70, (void* self, void* edx, uint32_t* a3));
CSF_FUNC(bool, __stdcall, GfxCollisionShapes_ReadVersionOneObjects, 0x006CEF70, (int* a1, void* a2));
CSF_FUNC(bool, __stdcall, GfxCollisionShapes_ReadLegacyObjects, 0x006CF270, (int* a1, void* a2));

CSF_GLOBAL(std::uint8_t, g_byte_82E65C, 0x0082E65C);

static bool __fastcall C_00860c88_ProcessWorldObjectPath(
    C_00860c88* self, void* /*edx*/, const char* filename)
{
    int* file = static_cast<int*>(CoreFiles_OpenSceneFile(
        filename, const_cast<char*>(CSF_STR(0x0083E130, "r")), 0));
    if (!file)
        return false;

    alignas(8) std::uint8_t collectionBytes[0x10];
    std::memset(collectionBytes + 4, 0, 12);

    std::uint32_t objectOffset = 0;
    for (std::uint32_t index = 0; ; ++index, objectOffset += 100)
    {
        const std::uint32_t begin = CSF_AT(std::uint32_t, self, 0x8);
        if (!begin ||
            index >= static_cast<std::uint32_t>(
                static_cast<std::int32_t>(
                    CSF_AT(std::uint32_t, self, 0xC) - begin) / 100))
            break;

        auto* object = reinterpret_cast<std::uint8_t*>(
            CSF_AT(std::uint32_t, self, 0x8) + objectOffset);
        if (!CompareStringSubstring(
                object + 4, nullptr, 0,
                CSF_AT(unsigned int, object, 0x18),
                reinterpret_cast<const char*>(&g_byte_82E65C), 0))
        {
            GfxCollisionShapes_AppendWorldObject(
                collectionBytes, nullptr,
                reinterpret_cast<uint32_t*>(
                    CSF_AT(std::uint32_t, self, 0x8) + objectOffset));
        }
    }

    alignas(8) std::uint8_t magicBytes[0x8];
    CSF_AT(int, magicBytes, 0) = 0;
    ((int(__fastcall*)(void*, void*, int*, int))
        CSF_AT(void**, file, 0)[0x8 / 4])(
            file, nullptr, reinterpret_cast<int*>(magicBytes), 4);

    bool replaceCollection = true;
    if (CSF_AT(int, magicBytes, 0) == -50529028)
    {
        GfxCollisionShapes_ReadLegacyObjects(file, collectionBytes);
    }
    else if (CSF_AT(int, magicBytes, 0) == -50529027)
    {
        alignas(8) std::uint8_t versionBytes[0x4];
        CSF_AT(int, versionBytes, 0) = 0;
        const int bytesRead =
            ((int(__fastcall*)(void*, void*, int*, int))
                CSF_AT(void**, file, 0)[0x8 / 4])(
                    file, nullptr, reinterpret_cast<int*>(versionBytes), 4);
        replaceCollection = bytesRead != 4;
        if (bytesRead == 4)
        {
            if (CSF_AT(int, versionBytes, 0) == 1)
                replaceCollection =
                    GfxCollisionShapes_ReadVersionOneObjects(file, collectionBytes);
            else
                replaceCollection = true;
        }
    }

    CloseSceneFile(file);

    if (replaceCollection)
    {
        std::uint32_t begin = CSF_AT(std::uint32_t, collectionBytes, 0x4);
        std::uint32_t cursor = begin;
        std::uint32_t index = 0;
        while (begin &&
               index < static_cast<std::uint32_t>(
                   static_cast<std::int32_t>(
                       CSF_AT(std::uint32_t, collectionBytes, 0x8) - begin) / 100))
        {
            const int oldIndex = GfxCollisionShapes_FindWorldObject(
                self, nullptr, reinterpret_cast<char*>(cursor));
            if (oldIndex != -1)
            {
                const std::uint32_t offset =
                    static_cast<std::uint32_t>(oldIndex) * 100;
                CSF_AT(std::uint32_t, reinterpret_cast<void*>(cursor), 0x60) =
                    CSF_AT(std::uint32_t,
                        reinterpret_cast<void*>(
                            CSF_AT(std::uint32_t, self, 0x8) + offset), 0x60);
                CSF_AT(std::uint32_t,
                    reinterpret_cast<void*>(
                        CSF_AT(std::uint32_t, self, 0x8) + offset), 0x60) = 0;
            }
            begin = CSF_AT(std::uint32_t, collectionBytes, 0x4);
            ++index;
            cursor += 100;
        }

        GfxCollisionShapes_ClearWorldObjects(self, nullptr);
        GfxCollisionShapes_SwapObjectCollections(
            reinterpret_cast<std::uint8_t*>(self) + 4, nullptr, collectionBytes);
        GfxScene_PrepareWorldObjectCollection(self, nullptr);

        std::uint32_t count = CSF_AT(std::uint32_t, self, 0x8);
        if (count)
            count = static_cast<std::uint32_t>(
                static_cast<std::int32_t>(
                    CSF_AT(std::uint32_t, self, 0xC) - count) / 100);
        CSF_AT(std::uint32_t, self, 0x14) = count;
    }

    void* remainingObjects = reinterpret_cast<void*>(
        CSF_AT(std::uint32_t, collectionBytes, 0x4));
    if (remainingObjects)
    {
        GfxCollisionShapes_DestroyObjectRange(
            CSF_AT(int, collectionBytes, 0x4),
            CSF_AT(int, collectionBytes, 0x8));
        Mem_FreeDebug(remainingObjects);
    }
    return !replaceCollection;
}

HOOK(0x006CF490, C_00860c88_ProcessWorldObjectPath, "auto");
