#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callback passed to sub_7DE550 (enumeration callback, int __cdecl(int*, int)).
using AudioEnumCallback = int(__cdecl*)(int*, int);

CSF_FUNC(uint32_t*, __fastcall, Audio_ReleaseChannelGroup, 0x00712050, (void* self, void* edx, int a2));
CSF_FUNC(void, __cdecl, Audio_ReleaseSharedState, 0x007C6300, ());
CSF_FUNC(int, __cdecl, RwResourcesEmptyArena, 0x007DB8B0, ());
CSF_FUNC(void, __cdecl, Audio_EnumerateDirectSoundDevices, 0x007DE550, (AudioEnumCallback callback, int context));

static int __fastcall C_00863ba8_ResetAudioEffects(C_00863ba8* self, void* /*edx*/, int a2)
{
    // Slot 0xC4 / 4 = 49 has no typed wrapper, so call it through the vtable.
    int result = reinterpret_cast<int(__fastcall*)(void*, void*)>(CSF_AT(void**, self, 0)[49])(self, nullptr);
    if (!static_cast<uint8_t>(result))
    {
        Audio_ReleaseSharedState();
        result = RwResourcesEmptyArena();
        if (self->field_0014)
        {
            // Original passes ecx = field_0014 as the fastcall "this" for the callee.
            result = static_cast<int>(reinterpret_cast<uintptr_t>(
                Audio_ReleaseChannelGroup(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0014)),
                                          nullptr, a2)));
        }
        uint32_t v4 = self->field_0024;
        if (v4)
        {
            int v5[2];
            v5[0] = static_cast<int>(v4);
            v5[1] = 0;
            Audio_EnumerateDirectSoundDevices(reinterpret_cast<AudioEnumCallback>(CSF_ADDR_FN(0x0070FBB0)),
                                              reinterpret_cast<int>(v5));
        }
    }
    return result;
}

HOOK(0x00710DF0, C_00863ba8_ResetAudioEffects, "auto");
