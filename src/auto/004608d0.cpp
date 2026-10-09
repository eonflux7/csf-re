#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, IsStateCodeAllowed, 0x00755810, (unsigned int a1));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __stdcall ScriptCommands_DispatchResolvedHandleCommand(int a1, int a2, uint32_t* a3)
{
    (void)a1;
    (void)a2;

    uint32_t v3 = CSF_AT(uint32_t, reinterpret_cast<void*>(a3[0]), 4);
    int32_t handleValue = CSF_AT(int32_t, reinterpret_cast<void*>(v3), 12);
    int result = static_cast<int>(v3 + 4);

    if (handleValue >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(v3 + 4));
        int entityHandle = result;
        if (entityHandle != 0)
        {
            void* entity = reinterpret_cast<void*>(static_cast<uintptr_t>(entityHandle));
            result = IsStateCodeAllowed(CSF_AT(unsigned int, entity, 80));
            if (static_cast<uint8_t>(result) != 0)
            {
                int valueRecord = GetValue8B0554();
                void* valueBase = reinterpret_cast<void*>(static_cast<uintptr_t>(valueRecord));
                void* manager = CSF_AT(void*, valueBase, 8128);

                uint32_t argRecord = CSF_AT(uint32_t, reinterpret_cast<void*>(a3[1]), 4);
                int commandArg = CSF_AT(int, reinterpret_cast<void*>(static_cast<uintptr_t>(argRecord)), 4);

                auto managerLookup = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, manager, 0)[3]);
                result = managerLookup(manager, nullptr, commandArg);

                if (result != -1)
                {
                    void* controller = CSF_AT(void*, entity, 68);
                    auto controllerCommand = reinterpret_cast<int(__fastcall*)(void*, void*, int, int)>(
                        CSF_AT(void**, controller, 0)[42]);
                    result = controllerCommand(controller, nullptr, result, 0);
                }
            }
        }
    }

    return result;
}

HOOK(0x004608D0, ScriptCommands_DispatchResolvedHandleCommand, "auto");
