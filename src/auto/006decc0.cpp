#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GfxTextureLoader_ProcessTextureEntry, 0x006DE8D0, (void* self, void* edx, int a2));

static int __stdcall GfxTextureLoader_ForwardTextureEntryIfNonNull(S_0046bea0* entry, int argument)
{
    if (entry)
        return GfxTextureLoader_ProcessTextureEntry(entry, nullptr, argument);
    return 0;
}

HOOK(0x006DECC0, GfxTextureLoader_ForwardTextureEntryIfNonNull, "auto");
