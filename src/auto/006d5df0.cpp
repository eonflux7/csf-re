#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneLoader_ResetRenderMember, 0x006D5BB0, (void* self, void* edx));

static int __fastcall GfxScene_DetachFromWorld(C_0085e7b8* self, void* /*edx*/)
{
    int result = 0;

    if (CSF_AT(uint8_t, self, 0) != 0)
    {
        void* renderObject = CSF_AT(void*, self, 0x4C);
        if (renderObject != nullptr)
        {
            // Virtual slot 1 (+0x4) of the render object, called with arg 1.
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, renderObject, 0)[1])(renderObject, nullptr, 1);
            CSF_AT(uint32_t, self, 0x4C) = 0;
        }
        result = SceneLoader_ResetRenderMember(self, nullptr);
    }

    CSF_AT(uint8_t, self, 0) = 0;
    return result;
}

HOOK(0x006D5DF0, GfxScene_DetachFromWorld, "auto");
