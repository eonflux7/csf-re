#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, Cheats_SetBinding, 0x004287D0, (int a1, int a2, int a3, int a4));
CSF_FUNC(uint32_t*, __cdecl, Cheats_InitializeBindings, 0x00428A70, (uint32_t* a1, int a2));
CSF_FUNC(unsigned int, __fastcall, C_008300c0_FinalizeBindings, 0x004291C0, (void* self, void* edx, int a2));

static unsigned int __fastcall C_008300c0_InitializeDefaultBindings(
    C_008300c0* self, void* /*edx*/, uint32_t* bindings)
{
    if (!bindings)
        return 0;

    Cheats_InitializeBindings(bindings, 93);
    const int bindingsAddress = reinterpret_cast<int>(bindings);

    Cheats_SetBinding(bindingsAddress, 25, 0, 1);
    Cheats_SetBinding(bindingsAddress, 0, 0, 17);
    Cheats_SetBinding(bindingsAddress, 1, 0, 31);
    Cheats_SetBinding(bindingsAddress, 2, 0, 30);
    Cheats_SetBinding(bindingsAddress, 3, 0, 32);
    Cheats_SetBinding(bindingsAddress, 4, 0, 200);
    Cheats_SetBinding(bindingsAddress, 5, 0, 208);
    Cheats_SetBinding(bindingsAddress, 6, 0, 203);
    Cheats_SetBinding(bindingsAddress, 7, 0, 205);
    Cheats_SetBinding(bindingsAddress, 10, 0, 42);
    Cheats_SetBinding(bindingsAddress, 11, 0, 46);
    Cheats_SetBinding(bindingsAddress, 12, 0, 56);
    Cheats_SetBinding(bindingsAddress, 13, 1, 0);
    Cheats_SetBinding(bindingsAddress, 14, 0, 57);
    Cheats_SetBinding(bindingsAddress, 15, 0, 33);
    Cheats_SetBinding(bindingsAddress, 16, 1, 2);
    Cheats_SetBinding(bindingsAddress, 17, 0, 19);
    Cheats_SetBinding(bindingsAddress, 19, 0, 2);
    Cheats_SetBinding(bindingsAddress, 21, 0, 3);
    Cheats_SetBinding(bindingsAddress, 22, 0, 15);
    Cheats_SetBinding(bindingsAddress, 23, 0, 28);
    Cheats_SetBinding(bindingsAddress, 24, 2, 1);
    Cheats_SetBinding(bindingsAddress, 27, 2, 0);
    Cheats_SetBinding(bindingsAddress, 26, 0, 35);
    Cheats_SetBinding(bindingsAddress, 32, 0, 20);
    Cheats_SetBinding(bindingsAddress, 8, 0, 16);
    Cheats_SetBinding(bindingsAddress, 9, 0, 18);
    Cheats_SetBinding(bindingsAddress, 92, 0, 1);
    Cheats_SetBinding(bindingsAddress, 28, 1, 1);
    Cheats_SetBinding(bindingsAddress, 36, 0, 63);
    Cheats_SetBinding(bindingsAddress, 37, 0, 67);
    Cheats_SetBinding(bindingsAddress, 29, 0, 59);
    Cheats_SetBinding(bindingsAddress, 30, 0, 60);
    Cheats_SetBinding(bindingsAddress, 31, 0, 61);
    Cheats_SetBinding(bindingsAddress, 40, 0, 47);
    Cheats_SetBinding(bindingsAddress, 41, 0, 68);
    Cheats_SetBinding(bindingsAddress, 42, 0, 68);
    Cheats_SetBinding(bindingsAddress, 43, 0, 68);
    Cheats_SetBinding(bindingsAddress, 45, 0, 25);
    Cheats_SetBinding(bindingsAddress, 46, 0, 50);
    Cheats_SetBinding(bindingsAddress, 44, 0, 157);
    Cheats_SetBinding(bindingsAddress, 47, 0, 37);
    Cheats_SetBinding(bindingsAddress, 33, 0, 29);
    Cheats_SetBinding(bindingsAddress, 34, 0, 56);
    Cheats_SetBinding(bindingsAddress, 35, 0, 42);
    Cheats_SetBinding(bindingsAddress, 48, 0, 23);
    Cheats_SetBinding(bindingsAddress, 49, 0, 48);
    Cheats_SetBinding(bindingsAddress, 50, 0, 44);
    Cheats_SetBinding(bindingsAddress, 51, 0, 11);
    Cheats_SetBinding(bindingsAddress, 52, 0, 82);
    Cheats_SetBinding(bindingsAddress, 53, 0, 79);
    Cheats_SetBinding(bindingsAddress, 54, 0, 80);
    Cheats_SetBinding(bindingsAddress, 55, 0, 71);
    Cheats_SetBinding(bindingsAddress, 56, 0, 38);
    Cheats_SetBinding(bindingsAddress, 57, 0, 21);
    Cheats_SetBinding(bindingsAddress, 59, 0, 34);
    Cheats_SetBinding(bindingsAddress, 61, 0, 22);
    Cheats_SetBinding(bindingsAddress, 62, 0, 36);
    Cheats_SetBinding(bindingsAddress, 64, 0, 47);
    Cheats_SetBinding(bindingsAddress, 65, 0, 49);
    Cheats_SetBinding(bindingsAddress, 66, 0, 81);
    Cheats_SetBinding(bindingsAddress, 67, 0, 83);

    return C_008300c0_FinalizeBindings(self, nullptr, bindingsAddress);
}

HOOK(0x004294F0, C_008300c0_InitializeDefaultBindings, "auto");
