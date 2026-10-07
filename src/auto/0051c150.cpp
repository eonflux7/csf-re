#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static std::uint8_t __fastcall C_00840ea4_TryExecuteOperation(
    C_00840ea4* self, void* /*edx*/, int argument, int operationArgument)
{
    if (!((std::uint8_t(__fastcall*)(void*, void*, int))self->vtbl[5])(
            self, nullptr, argument))
    {
        return 0;
    }

    ((void(__fastcall*)(void*, void*, int, int))self->vtbl[6])(
        self, nullptr, argument, operationArgument);
    return 1;
}

HOOK(0x0051C150, C_00840ea4_TryExecuteOperation, "auto");
