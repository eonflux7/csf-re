#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, sub_551680, 0x00551680, (void* self, void* edx, int a2, int a3, int a4, int a5));
CSF_FUNC(int, __fastcall, sub_73BE00, 0x0073BE00, (void* self, void* edx, char a2));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, sub_73CD30, 0x0073CD30, (void* self, void* edx));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static C_00833908* __fastcall C_00844694_RefreshSpawnPointFromHandle(C_00844694* self, void* /*edx*/, int a2)
{
    uint32_t result = static_cast<uint32_t>(a2);
    uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a2));

    if (CSF_AT(int, entry, 0x10) >= 0)
    {
        int resolved = CHandleTable_Resolve(g_pCurrentScene, nullptr, a2 + 8);
        result = static_cast<uint32_t>(resolved);
        if (result != 0)
        {
            uint8_t* resolvedEntry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(result));
            result = CSF_AT(uint32_t, resolvedEntry, 0x18);
            if (result != 0)
            {
                if (result != 0x80)
                {
                    sub_73BE00(reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(result)) - 0x80, nullptr, 0);

                    result = self->field_0c8c;
                    if (result != 0)
                    {
                        uint8_t* stateEntry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(result));
                        result = CSF_AT(uint32_t, stateEntry, 0x18);
                        if (result != 0)
                        {
                            result += 0xFFFFF950u; // -1712
                            if (result != 0)
                            {
                                uint8_t* sceneState = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(result));
                                uint8_t flagByte = CSF_AT(uint8_t, sceneState, 0x443);

                                int globalValue = GetGlobalDword_73C020();
                                int v6 = sub_73CD30(reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(globalValue))), nullptr);

                                int flagArg = static_cast<int>((flagByte != 1) + 0x53423034);
                                return reinterpret_cast<C_00833908*>(
                                    static_cast<uintptr_t>(
                                        static_cast<uint32_t>(sub_551680(self, nullptr, flagArg, v6, -1, -1))));
                            }
                        }
                    }
                }
            }
        }
    }

    return reinterpret_cast<C_00833908*>(static_cast<uintptr_t>(result));
}

HOOK(0x00552530, C_00844694_RefreshSpawnPointFromHandle, "auto");
