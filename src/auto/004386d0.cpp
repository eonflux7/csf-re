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

// Mem_FreeDebug: __cdecl, one stack arg, caller cleans up (ret 0).
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// __thiscall MenuObject_Refresh(this): ecx = this, edx unused, ret 0.
static void* __fastcall MenuObject_Refresh(void* self, void* /*edx*/)
{
    // result = this[1]
    void* result = CSF_AT(void*, self, 0x4);
    if (result)
    {
        // 0x18-byte stack buffer: Block[0] at +0x00, Block[4] at +0x10, v3 at +0x14.
        alignas(8) uint8_t blockBytes[0x18];

        CSF_AT(uint32_t, blockBytes, 0x14) = 7;                 // v3 = 7
        CSF_AT(void*, blockBytes, 0x10) = nullptr;              // Block[4] = nullptr
        CSF_AT(uint16_t, blockBytes, 0x00) = 0;                 // LOWORD(Block[0]) = 0

        // result = obj->vtbl[0x40 / 4](&Block)  (__stdcall, one argument)
        void* obj = result;
        result = ((void* (__stdcall*)(void*))CSF_AT(void**, obj, 0)[0x40 / 4])(blockBytes);

        if (CSF_AT(uint32_t, blockBytes, 0x14) >= 8)            // unsigned compare (jb)
        {
            void* ownedBlock = CSF_AT(void*, blockBytes, 0x00);
            if (ownedBlock)
            {
                result = reinterpret_cast<void*>(Mem_FreeDebug(ownedBlock));
            }
        }
    }
    return result;
}

HOOK(0x004386D0, MenuObject_Refresh, "auto");
