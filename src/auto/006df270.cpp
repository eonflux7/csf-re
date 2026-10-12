#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, LoadTextureWithOptions, 0x006DF1F0,
         (void* self, void* edx, unsigned int a2, char a3, void* Block, int a5, int a6));

static char __fastcall S_006deed0_LoadTexture(S_006deed0* self, void* /*edx*/,
                                             unsigned int textureId)
{
    return LoadTextureWithOptions(self, nullptr, textureId, 0, nullptr, 0, 0);
}

HOOK(0x006DF270, S_006deed0_LoadTexture, "auto");
