#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, IsHandleStateSet, 0x00755810, (unsigned int a1));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

// __stdcall, 3 stack args (12 bytes popped by callee).
static int __stdcall DispatchResolvedHandleCommand(int a1, int a2, uint32_t* a3)
{
    (void)a1;
    (void)a2;

    uint8_t* commandNode = reinterpret_cast<uint8_t*>(*a3);
    uint8_t* v3 = *CSF_AT(uint8_t**, commandNode, 4);
    int32_t v4 = *CSF_AT(int32_t*, v3, 12);
    int result = reinterpret_cast<int>(v3 + 4);

    if (v4 >= 0)
    {
        int entity = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
        result = entity;
        if (entity != 0)
        {
            uint8_t* entityObj = reinterpret_cast<uint8_t*>(entity);
            result = IsHandleStateSet(*CSF_AT(uint32_t*, entityObj, 0x50));
            if (static_cast<uint8_t>(result))
            {
                int valueBase = GetValue8B0554();
                void* manager = *CSF_AT(void**, reinterpret_cast<uint8_t*>(valueBase), 0x1FC0);
                void** managerVtbl = *reinterpret_cast<void***>(manager);
                auto managerLookup = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(managerVtbl[3]);

                uint8_t* inputBlock = reinterpret_cast<uint8_t*>(a3[1]);
                uint8_t* inputInner = *CSF_AT(uint8_t**, inputBlock, 4);
                int inputValue = *CSF_AT(int*, inputInner, 4);

                result = managerLookup(manager, nullptr, inputValue);
                if (result != -1)
                {
                    uint8_t* controller = *CSF_AT(uint8_t**, entityObj, 0x44);
                    void** controllerVtbl = *reinterpret_cast<void***>(controller);
                    auto controllerCommand = reinterpret_cast<int(__fastcall*)(void*, void*, int, int)>(controllerVtbl[42]);
                    return controllerCommand(controller, nullptr, result, 0);
                }
            }
        }
    }
    return result;
}

HOOK(0x4608D0, DispatchResolvedHandleCommand, "auto");
