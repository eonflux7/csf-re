#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_GLOBAL(const char*, g_pszMenu_P_LbMePr, 0x00899B40);

static int __fastcall C_0085ec18_HandleInputEvent(C_0085ec18* self, void* /*edx*/, int a2, int a3)
{
    if (a2 == 1 || a2 == 14)
    {
        uint8_t* selfMinus4 = reinterpret_cast<uint8_t*>(self) - 4;

        // Child object at (this - 4) + 0x14 == this + 0x10.
        void* child = CSF_AT(void*, selfMinus4, 0x14);
        reinterpret_cast<void(__fastcall*)(void*, void*, void*, int)>(CSF_AT(void**, child, 0)[24])(
            child, nullptr, selfMinus4, 1);

        child = CSF_AT(void*, selfMinus4, 0x14);
        void* labelControl = reinterpret_cast<void*(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, child, 0)[23])(child, nullptr, g_pszMenu_P_LbMePr);

        void* labelItem = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, labelControl, 0)[8])(labelControl, nullptr);

        return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, labelItem, 0)[42])(labelItem, nullptr, 0);
    }

    int result = a2 - 50;
    if (a2 == 50)
    {
        result = a3;
        void* argObject = reinterpret_cast<void*>(static_cast<uintptr_t>(a3));
        if (CSF_AT(uint8_t, argObject, 0x38) != 0)
        {
            void* referenced = GetReferencedObject(self, nullptr);
            return reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, referenced, 0)[7])(referenced, nullptr);
        }
    }
    return result;
}

HOOK(0x00565E20, C_0085ec18_HandleInputEvent, "auto");
