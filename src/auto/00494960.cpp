#include "hook.h"
#include "types/auto_classes.h"

// Returns field_039c when field_0398 is set and field_039c is non-zero;
// otherwise falls back to field_022c.
static int __fastcall CPlayerCharacter_ResolveActiveOrDefaultValue(CPlayerCharacter* self, void* /*edx*/)
{
    if (!self->field_0398)
        return static_cast<int>(self->field_022c);

    int result = static_cast<int>(self->field_039c);
    if (!result)
        return static_cast<int>(self->field_022c);

    return result;
}

HOOK(0x494960, CPlayerCharacter_ResolveActiveOrDefaultValue, "auto");
