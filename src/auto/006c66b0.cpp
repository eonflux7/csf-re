#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(int, g_Value_8BB4B8, 0x008BB4B8);
CSF_GLOBAL(uint32_t, g_PassthroughTarget_8BB4B4, 0x008BB4B4);

CSF_FUNC(int, __cdecl, SceneLoader_GetObjectAttachment, 0x006B87A0, (int a1));
CSF_FUNC(char, __fastcall, sub_6C63D0, 0x006C63D0, (void* self, void* edx, int a2));

// Forwards to the function pointer stored at 0x8BB4B4 when the flag at
// g_Value_8BB4B8+0xF5 is set; otherwise refreshes the attachment and returns a1.
static hook::Tail __fastcall GfxSkin_RefreshAttachmentOrForward(void* self, const uint32_t* args)
{
    (void)self;
    const int a1 = static_cast<int>(args[0]);

    void* sceneObj = reinterpret_cast<void*>(static_cast<uintptr_t>(g_Value_8BB4B8));
    const uint8_t flag = CSF_AT(uint8_t, sceneObj, 0xF5);

    if (flag != 0) {
        // Original: `mov cl, byte ptr [eax+0xf5]` only replaces the low byte of ecx, so the
        // upper 24 bits of the entry ecx reach `jmp dword ptr [0x8bb4b4]` unchanged.
        // The target is the function pointer value stored at 0x8BB4B4.
        const uint32_t entryEcx = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(self));
        const uint32_t targetEcx = (entryEcx & 0xFFFFFF00u) | flag;
        void* passthroughTarget = reinterpret_cast<void*>(static_cast<uintptr_t>(g_PassthroughTarget_8BB4B4));
        return hook::tail_jump(reinterpret_cast<void*>(static_cast<uintptr_t>(targetEcx)),
                               passthroughTarget);
    }

    void* attachment = reinterpret_cast<void*>(static_cast<uintptr_t>(SceneLoader_GetObjectAttachment(a1)));
    const int attachedValue = static_cast<int>(CSF_AT(uint32_t, attachment, 0x08));
    sub_6C63D0(sceneObj, nullptr, attachedValue);

    return hook::tail_ret(static_cast<uint32_t>(a1));
}

HOOK_TAIL(0x006C66B0, GfxSkin_RefreshAttachmentOrForward, "gfx_skin");
