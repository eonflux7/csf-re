#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// CHandleTable::Resolve: __thiscall, 1 stack arg (handle), ret 4.
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

// Cdecl helper that takes one dword and returns a bool in al; name chosen here.
CSF_FUNC(int, __cdecl, QueryObjectStateFlag, 0x00755810, (unsigned int a1));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// __thiscall with 3 stack args (a2, a3, a4), ret 12.
static int __fastcall SceneCommand_CopyTargetPosition(void* self, void* edx, int a2, int a3, int* a4)
{
    (void)edx;
    (void)a2;
    (void)a3;

    uint8_t* commandRecord = reinterpret_cast<uint8_t*>(*a4);
    uint8_t* v5 = CSF_AT(uint8_t*, commandRecord, 4);
    uint8_t* v7 = v5 + 4;

    int32_t result = CSF_AT(int32_t, v5, 12);
    if (result >= 0)
    {
        int handle = static_cast<int>(reinterpret_cast<uintptr_t>(v7));
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, handle);

        uint8_t* target = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(result));
        if (target != nullptr)
        {
            result = CSF_AT(int32_t, target, 0x34);
            if (result != 0)
            {
                result = QueryObjectStateFlag(CSF_AT(uint32_t, target, 0x50));
                if ((result & 0xFF) != 0)
                {
                    if (CSF_AT(int32_t, target, 0x1C4) == 1)
                    {
                        void* scriptObj = CSF_AT(void*, target, 0x34);
                        int32_t commandId = 7;
                        result = ((int(__fastcall*)(void*, void*, int32_t*))
                                  CSF_AT(void**, scriptObj, 0)[42])(scriptObj, nullptr, &commandId);

                        CSF_AT(int32_t, self, 0x60) = CSF_AT(int32_t, v7, 4);
                        CSF_AT(int32_t, self, 0x64) = CSF_AT(int32_t, v7, 8);
                        CSF_AT(uint8_t, self, 0x82) = 0;
                    }
                }
            }
        }
    }
    return result;
}

HOOK(0x45EDF0, SceneCommand_CopyTargetPosition, "auto");
