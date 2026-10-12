#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(char, __fastcall, RegisterPreparedScene, 0x006ECA90,
               (void* self, void* edx, void* a2));
static CSF_FUNC(unsigned int, __fastcall, ClearPreparedScene, 0x006ECB70,
               (void* self, void* edx));

static char __fastcall C_008605d8_RegisterPreparedScene(
    C_008605d8* self, void* /*edx*/, C_00833708* scene, char enabled)
{
    if (enabled)
        return RegisterPreparedScene(self, nullptr, scene);

    return static_cast<char>(ClearPreparedScene(self, nullptr));
}

HOOK(0x006ECBE0, C_008605d8_RegisterPreparedScene, "auto");
