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

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());

// Forwards a notification (a2) to the handler object stored at GlobalState+0x1C,
// via its vtable slot 0x44 (17). Runs the forward only while this->b6 stays set.
static char __fastcall CSceneLoaderBase_ForwardToGlobalHandler(CSceneLoaderBase* self, void* /*edx*/, int a2)
{
    char result = CSF_AT(char, self, 0xB6);
    if (result)
    {
        char* globalState = reinterpret_cast<char*>(GetGlobalState_50D190());
        if (CSF_AT(void*, globalState, 0x1C))
        {
            globalState = reinterpret_cast<char*>(GetGlobalState_50D190());
            void* handler = CSF_AT(void*, globalState, 0x1C);
            void** handlerVtbl = CSF_AT(void**, handler, 0);
            ((void(__fastcall*)(void*, void*, int))handlerVtbl[17])(handler, nullptr, a2);
        }

        result = CSF_AT(char, self, 0xB6);
        if (result)
        {
            char* globalStateAgain = reinterpret_cast<char*>(GetGlobalState_50D190());
            result = static_cast<char>(reinterpret_cast<uintptr_t>(globalStateAgain));
            if (CSF_AT(void*, globalStateAgain, 0x1C))
            {
                char* globalStateForCall = reinterpret_cast<char*>(GetGlobalState_50D190());
                void* handler = CSF_AT(void*, globalStateForCall, 0x1C);
                void** handlerVtbl = CSF_AT(void**, handler, 0);
                result = ((char(__fastcall*)(void*, void*, int))handlerVtbl[17])(handler, nullptr, a2);
            }
        }
    }
    return result;
}

HOOK(0x004FE930, CSceneLoaderBase_ForwardToGlobalHandler, "auto");
