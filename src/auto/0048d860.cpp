#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x8AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, BddLoader_ReleasePendingHandle, 0x0050E250, (void* self, void*));
CSF_FUNC(char, __fastcall, C_00858180_ResetTargetIfMatches, 0x0073BD60, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, GetSpawnFlagFromGlobal, 0x0073CD30, (void* self, void*));

static int __fastcall ClearTargetBindingAndRelease(void* self, void* /*edx*/)
{
    int result = CSF_AT(int, self, 0x2C);
    if (result >= 0)
    {
        result = CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            static_cast<int>(reinterpret_cast<uintptr_t>(self) + 0x24));
        if (result)
        {
            void* entry = reinterpret_cast<void*>(static_cast<uintptr_t>(result));
            if (CSF_AT(int, entry, 0x50) == 0x20)
            {
                result = CSF_AT(int, entry, 0x18);
                if (result)
                {
                    void* targetObj = reinterpret_cast<void*>(
                        static_cast<uintptr_t>(result) - 0x80);
                    if (result != 128)
                    {
                        int globalDword = GetGlobalDword_73C020();
                        int spawnFlag = GetSpawnFlagFromGlobal(
                            reinterpret_cast<void*>(static_cast<uintptr_t>(globalDword)), nullptr);
                        C_00858180_ResetTargetIfMatches(targetObj, nullptr, spawnFlag);

                        int globalState = GetGlobalState_50D190();
                        BddLoader_ReleasePendingHandle(
                            reinterpret_cast<void*>(static_cast<uintptr_t>(globalState)), nullptr);

                        CSF_AT(uint8_t, self, 0x30) = 0;
                        CSF_AT(int, self, 0x28) = -1;
                        CSF_AT(int, self, 0x2C) = -1;
                        return -1;
                    }
                }
            }
        }
    }
    return result;
}

HOOK(0x0048D860, ClearTargetBindingAndRelease, "auto");
