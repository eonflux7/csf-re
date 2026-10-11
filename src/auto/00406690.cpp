#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, BaseObjectUpdate, 0x00405BF0, (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, AuxObjectCreate, 0x004064E0, (void* self, void* edx, int a2, int a3, char a4));

CSF_GLOBAL(int, g_AuxObjectCreateArg, 0x0082D564);

static void __fastcall C_0082d70c_EnsureAuxObjectThenUpdate(C_0082d70c* self, void* /*edx*/, float a2)
{
    if (CSF_AT(int, self, 0x40) == 0)
    {
        char auxFlag = static_cast<char>(CSF_AT(uint8_t, self, 0x38));
        CSF_AT(int, self, 0x40) = AuxObjectCreate(self, nullptr, 100, 0x15, auxFlag);
    }
    BaseObjectUpdate(self, nullptr, a2);
}

HOOK(0x00406690, C_0082d70c_EnsureAuxObjectThenUpdate, "auto");
