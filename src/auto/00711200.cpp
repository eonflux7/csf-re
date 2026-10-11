#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Vtable slots used on the DirectSound buffer object held in field_0004:
//   slot 12 (+0x30) = IDirectSoundBuffer::Play(dwReserved, dwPriority, dwFlags), stdcall, 4 args incl. this
//   slot 20 (+0x50) = IDirectSoundBuffer::Restore(), stdcall, 1 arg (this)
using BufferPlayFn = int(__stdcall*)(void* buffer, uint32_t reserved, uint32_t priority, uint32_t flags);
using BufferRestoreFn = int(__stdcall*)(void* buffer);

static constexpr int kDsErrorBufferLost = static_cast<int>(0x88780096u);

static int __fastcall C_00863cd4_PlayWithBufferRestore(C_00863cd4* self, void* /*edx*/, char a2) {
    // a2 != 0 selects looping playback (DSBPLAY_LOOPING = 1)
    const uint32_t playFlags = (a2 != 0) ? 1u : 0u;

    void* buffer = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0004));

    int result = reinterpret_cast<BufferPlayFn>(CSF_AT(void**, buffer, 0)[0x30 / 4])(
        buffer, 0, 0, playFlags);

    if (result == kDsErrorBufferLost) {
        // Buffer was lost: try to restore it, then play again if the restore succeeded.
        result = reinterpret_cast<BufferRestoreFn>(CSF_AT(void**, buffer, 0)[0x50 / 4])(buffer);
        if (!result) {
            return reinterpret_cast<BufferPlayFn>(CSF_AT(void**, buffer, 0)[0x30 / 4])(
                buffer, 0, 0, playFlags);
        }
    }
    return result;
}

HOOK(0x711200, C_00863cd4_PlayWithBufferRestore, "auto");
