#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// void __thiscall sub_53A690(float *this, int a2) — base menu setup called with this object.
CSF_FUNC(void, __fastcall, CMenuScreen_Setup, 0x53A690, (float* self, void* edx, int a2));

// char *__thiscall sub_543E60(std::uint32_t *this) — lazily builds the copyright static.
CSF_FUNC(char*, __fastcall, CMenuMainPC_CreateCopyRightStatic, 0x543E60, (CMenuMainPC* self, void* edx));

// Runs the base setup, then returns the copyright static, creating it if it does not exist yet.
static char* __fastcall CMenuMainPC_GetCopyRightStatic(CMenuMainPC* self, void* /*edx*/, int a2)
{
    CMenuScreen_Setup(reinterpret_cast<float*>(self), nullptr, a2);

    char* result = reinterpret_cast<char*>(self->field_0038);
    if (!result)
        return CMenuMainPC_CreateCopyRightStatic(self, nullptr);
    return result;
}

HOOK(0x543F20, CMenuMainPC_GetCopyRightStatic, "auto");
