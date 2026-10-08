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

// Callee: unknown prototype, __thiscall-style with ecx=this and no stack args (ret 0).
CSF_FUNC(int, __fastcall, GetReferencedObject, 0x6683F0, (void* self, void* edx));

// Global: pointer value stored at 0x899B40 (first element of the menu label-name table).
CSF_GLOBAL(const char*, g_pszMenu_P_LbMePr, 0x899B40);

// Hook bound to 0x565E20.
// Handles menu input events: a2 == 1 or 14 refreshes the menu's sub-object state,
// a2 == 50 returns the result of a child's virtual call when a3's flag byte at +0x38 is set,
// otherwise returns a2 - 50 (or a3 for the code-50 path with the flag clear).
static int __fastcall MenuAccounts_HandleInputEvent(char* self, void* /*edx*/, int a2, int a3)
{
    if (a2 == 1 || a2 == 14)
    {
        char* parentBase = self - 4;

        // parentBase->+0x14 object, virtual slot 0x60 (96) called with (parentBase, 1).
        void* firstObj = *CSF_AT(void**, parentBase, 0x14);
        {
            void** vtbl = CSF_AT(void**, firstObj, 0);
            ((void(__fastcall*)(void*, void*, void*, int))vtbl[96 / 4])(firstObj, nullptr, parentBase, 1);
        }

        // Second object at +0x14, virtual slot 0x5C (92) called with the menu name string.
        void* labelObj = *CSF_AT(void**, parentBase, 0x14);
        void* labelResult;
        {
            void** vtbl = CSF_AT(void**, labelObj, 0);
            labelResult = ((void*(__fastcall*)(void*, void*, const char*))vtbl[92 / 4])(
                labelObj, nullptr, g_pszMenu_P_LbMePr);
        }

        // labelResult->vtbl[0x20] (32) with no args.
        void* innerObj;
        {
            void** vtbl = CSF_AT(void**, labelResult, 0);
            innerObj = ((void*(__fastcall*)(void*, void*))vtbl[32 / 4])(labelResult, nullptr);
        }

        // innerObj->vtbl[0xA8] (168) called with 0.
        {
            void** vtbl = CSF_AT(void**, innerObj, 0);
            return ((int(__fastcall*)(void*, void*, int))vtbl[168 / 4])(innerObj, nullptr, 0);
        }
    }

    if (a2 == 50)
    {
        char* argBlock = reinterpret_cast<char*>(a3);
        if (argBlock[0x38] != 0)
        {
            void* child = reinterpret_cast<void*>(GetReferencedObject(self, nullptr));
            void** vtbl = CSF_AT(void**, child, 0);
            return ((int(__fastcall*)(void*, void*))vtbl[28 / 4])(child, nullptr);
        }
        return a3;
    }

    return a2 - 50;
}

HOOK(0x565E20, MenuAccounts_HandleInputEvent, "auto");
