#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, MenuSaveGame_ClearSlotRange, 0x00544340, (int* first, int* last));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(char, __cdecl, CoreFiles_DestroyEntryRange, 0x0069EB70, (int a1, int a2));
CSF_FUNC(int, __cdecl, LogWindow_ReleaseRef, 0x006A3060, ());

static int* __fastcall CoreFiles_DestroySceneFileState(void* self, void* /*edx*/)
{
    const uint32_t objectCount = CSF_AT(uint32_t, self, 0x34);
    for (uint32_t index = 0; index < objectCount; ++index)
    {
        void* object = CSF_AT(void**, self, 0x2C)[index];
        if (object)
        {
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
            CSF_AT(void**, self, 0x2C)[index] = nullptr;
        }
    }

    void* logWindowState = CSF_AT(void*, self, 0x40);
    CSF_AT(uint32_t, self, 0x34) = 0;
    if (logWindowState)
    {
        LogWindow_ReleaseRef();
        CSF_AT(void*, self, 0x40) = nullptr;
    }

    if (CSF_AT(void*, self, 0x2C))
    {
        CoreMemory_Free(CSF_AT(void*, self, 0x2C));
        CSF_AT(void*, self, 0x2C) = nullptr;
    }

    if (CSF_AT(int*, self, 0x20))
    {
        MenuSaveGame_ClearSlotRange(
            CSF_AT(int*, self, 0x20),
            CSF_AT(int*, self, 0x24));
        if (CSF_AT(void*, self, 0x20))
            Mem_FreeDebug(CSF_AT(void*, self, 0x20));
    }
    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(uint32_t, self, 0x24) = 0;
    CSF_AT(uint32_t, self, 0x28) = 0;

    int* result = CSF_AT(int*, self, 0x10);
    if (result)
    {
        CoreFiles_DestroyEntryRange(
            reinterpret_cast<intptr_t>(result),
            reinterpret_cast<intptr_t>(CSF_AT(int*, self, 0x14)));
        result = CSF_AT(int*, self, 0x10);
        if (result)
            result = reinterpret_cast<int*>(Mem_FreeDebug(result));
    }
    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;
    return result;
}

HOOK(0x0069F2E0, CoreFiles_DestroySceneFileState, "auto");
