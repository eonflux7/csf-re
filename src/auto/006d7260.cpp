#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(char, __fastcall, C_00863018_ReleaseBuffers, 0x006FE470, (C_00863018* self, void*));
CSF_FUNC(int, __cdecl, Gfx2d_ReleaseEntry, 0x0081CAA0, (int a1));

static int __fastcall Gfx2d_ResetSceneRenderHandle(void* self, void* /*edx*/)
{
    C_00863018_ReleaseBuffers(
        reinterpret_cast<C_00863018*>(
            reinterpret_cast<std::uint8_t*>(self) + 0x74),
        nullptr);

    for (std::uint32_t index = 0;
         index < CSF_AT(std::uint32_t, self, 0x5C);
         ++index)
    {
        void* object = CSF_AT(void**, self, 0x54)[index];
        if (object)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, object, 0)[1])(object, nullptr, 1);
            CSF_AT(void**, self, 0x54)[index] = nullptr;
        }
    }

    void* objects = CSF_AT(void*, self, 0x54);
    CSF_AT(std::uint32_t, self, 0x58) = 0;
    CSF_AT(std::uint32_t, self, 0x5C) = 0;
    if (objects)
    {
        Mem_Free(objects);
        CSF_AT(void*, self, 0x54) = nullptr;
    }

    for (std::uint32_t groupIndex = 0;
         groupIndex < CSF_AT(std::uint32_t, self, 0x6C);
         ++groupIndex)
    {
        void* group = CSF_AT(void**, self, 0x64)[groupIndex];
        for (std::uint32_t entryIndex = 0;
             entryIndex < CSF_AT(std::uint32_t, group, 0x0C);
             ++entryIndex)
        {
            Gfx2d_ReleaseEntry(CSF_AT(int*, group, 0x04)[entryIndex]);
        }

        void* entries = CSF_AT(void*, group, 0x04);
        if (entries)
        {
            Mem_Free(entries);
            CSF_AT(void*, group, 0x04) = nullptr;
        }
        Mem_Free(group);
    }

    void* groups = CSF_AT(void*, self, 0x64);
    CSF_AT(std::uint32_t, self, 0x68) = 0;
    CSF_AT(std::uint32_t, self, 0x6C) = 0;
    if (groups)
    {
        int result = Mem_Free(groups);
        CSF_AT(void*, self, 0x64) = nullptr;
        return result;
    }
    return 0;
}

HOOK(0x006D7260, Gfx2d_ResetSceneRenderHandle, "auto");
