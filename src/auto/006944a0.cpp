#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(void, __cdecl, CoreParser_ReleaseAllocation, 0x00692A70, (S_0069b1c0* allocation));
CSF_FUNC(char, __fastcall, CorePaths_ClearAllocationContents, 0x00693370, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CorePaths_BuildSerializedTables, 0x00694420, (void* self, void* edx, int a2, uint32_t* a3));
CSF_FUNC(std::uint32_t, __cdecl, CloseSceneFile, 0x00697980, (void* file));
CSF_FUNC(void*, __cdecl, CoreFiles_OpenSceneFile, 0x00698350, (const char* filename, char* mode, int openFlags));

static char __fastcall S_00694180_ParseLoadedFile(
    S_00694180* self, void* /*edx*/, unsigned int filename)
{
    alignas(8) std::uint8_t headerBytes[0x18];
    alignas(8) std::uint8_t tableBytes[0x10];

    CSF_AT(std::uint32_t, tableBytes, 0x0) = 0;
    CSF_AT(std::uint32_t, tableBytes, 0x4) = 0;
    CSF_AT(std::uint32_t, tableBytes, 0x8) = 0;
    CSF_AT(std::uint32_t, tableBytes, 0xC) = 10;

    CorePaths_BuildSerializedTables(
        self, nullptr, reinterpret_cast<int>(headerBytes),
        reinterpret_cast<std::uint32_t*>(tableBytes));

    void* file = CoreFiles_OpenSceneFile(
        reinterpret_cast<const char*>(filename),
        const_cast<char*>(CSF_STR(0x008579FC, "wb")), 0);

    if (!file)
    {
        CloseSceneFile(nullptr);
        if (CSF_AT(void*, tableBytes, 0x0))
            CoreMemory_Free(CSF_AT(void*, tableBytes, 0x0));
        return 0;
    }

    ((void(__fastcall*)(void*, void*, const void*, unsigned int))
        CSF_AT(void**, file, 0)[3])(file, nullptr, headerBytes, 24);

    filename = CSF_AT(std::uint32_t, headerBytes, 0xC);
    for (unsigned int i = 0; i < filename; ++i)
    {
        void* record = CSF_AT(void**, tableBytes, 0x0)[i];
        CSF_AT(std::uint8_t, record, 0xB) = 0;
        ((void(__fastcall*)(void*, void*, const void*, unsigned int))
            CSF_AT(void**, file, 0)[3])(file, nullptr, record, 12);
    }

    for (unsigned int i = 0; i < self->field_0024; ++i)
    {
        filename = static_cast<unsigned int>(
            std::strlen(CSF_AT(const char**, self, 0x1C)[i]));
        if (filename)
        {
            ++filename;
            ((void(__fastcall*)(void*, void*, const void*, unsigned int))
                CSF_AT(void**, file, 0)[3])(file, nullptr, &filename, 4);
            const unsigned int length = filename;
            const char* text = CSF_AT(const char**, self, 0x1C)[i];
            ((void(__fastcall*)(void*, void*, const void*, unsigned int))
                CSF_AT(void**, file, 0)[3])(file, nullptr, text, length);
        }
        else
        {
            ((void(__fastcall*)(void*, void*, const void*, unsigned int))
                CSF_AT(void**, file, 0)[3])(file, nullptr, &filename, 4);
        }
    }

    for (unsigned int i = 0; i < self->field_0034; ++i)
    {
        filename = static_cast<unsigned int>(
            std::strlen(CSF_AT(const char**, self, 0x2C)[i]));
        if (filename)
        {
            ++filename;
            ((void(__fastcall*)(void*, void*, const void*, unsigned int))
                CSF_AT(void**, file, 0)[3])(file, nullptr, &filename, 4);
            const unsigned int length = filename;
            const char* text = CSF_AT(const char**, self, 0x2C)[i];
            ((void(__fastcall*)(void*, void*, const void*, unsigned int))
                CSF_AT(void**, file, 0)[3])(file, nullptr, text, length);
        }
        else
        {
            ((void(__fastcall*)(void*, void*, const void*, unsigned int))
                CSF_AT(void**, file, 0)[3])(file, nullptr, &filename, 4);
        }
    }

    CloseSceneFile(file);

    for (unsigned int i = 0;
         i < CSF_AT(std::uint32_t, tableBytes, 0x8); ++i)
    {
        void* record = CSF_AT(void**, tableBytes, 0x0)[i];
        if (record)
        {
            if (CSF_AT(std::uint8_t, record, 0xB) == 0xFF)
            {
                S_0069b1c0* allocation =
                    CSF_AT(S_0069b1c0*, record, 0x0);
                if (allocation)
                {
                    CorePaths_ClearAllocationContents(allocation, nullptr);
                    CoreParser_ReleaseAllocation(allocation);
                    CSF_AT(S_0069b1c0*, record, 0x0) = nullptr;
                }
            }
            CSF_AT(std::uint8_t, record, 0xB) = 0xFF;
            CoreMemory_Free(record);
            CSF_AT(void**, tableBytes, 0x0)[i] = nullptr;
        }
    }

    if (CSF_AT(void*, tableBytes, 0x0))
        CoreMemory_Free(CSF_AT(void*, tableBytes, 0x0));
    return 1;
}

HOOK(0x006944A0, S_00694180_ParseLoadedFile, "auto");
