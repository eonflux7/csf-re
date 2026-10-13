#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_00861988_UpdateNamedEmitterEntry, 0x00702A40, (C_00861988* self, void* /*edx*/, int emitterKey, int entryArgument));

static char __fastcall C_00861988_UpdateNamedEmitterEntryIfReady(C_00861988* self, void* /*edx*/, int a2, int a3)
{
    char result = static_cast<char>(self->field_004c);
    if (result)
    {
        C_00861988* target = reinterpret_cast<C_00861988*>(self->field_0044);
        if (target)
            return C_00861988_UpdateNamedEmitterEntry(target, nullptr, a2, a3);
    }
    return result;
}

HOOK(0x006DF8E0, C_00861988_UpdateNamedEmitterEntryIfReady, "auto");
