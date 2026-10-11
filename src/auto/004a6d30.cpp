#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));

// Reads/validates a stream-state record: runs the owner's virtual check/read/finish callbacks
static int __fastcall S_004a6d30_ProcessStreamState(S_004a6d30* self, void* /*edx*/, void* a2)
{
    uint8_t* state = static_cast<uint8_t*>(a2);
    int savedField = CSF_AT(int, state, 0x1C);

    // First: skip the check when already flagged, otherwise ask the owner (slot 0x80 / 4)
    bool flag;
    if (CSF_AT(uint8_t, state, 0) != 0) {
        flag = false;
    } else {
        void* owner = CSF_AT(void*, state, 4);
        void** vtbl = CSF_AT(void**, owner, 0);
        unsigned char r = ((unsigned char(__fastcall*)(void*, void*, int, void*))vtbl[0x80 / 4])(owner, nullptr, 0, a2);
        flag = (r == 0);
    }
    CSF_AT(uint8_t, state, 0) = flag ? 1 : 0;
    CSF_AT(int, state, 0x1C) = 0;

    if (CSF_AT(uint8_t, state, 0) == 0) {
        void* owner = CSF_AT(void*, state, 4);
        void** vtbl = CSF_AT(void**, owner, 0);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        int r = ((int(__fastcall*)(void*, void*, void*, void*))vtbl[0x48 / 4])(
            owner, nullptr, reinterpret_cast<uint8_t*>(self) + 4, a2);
        CSF_AT(uint8_t, state, 0) = (r == -1) ? 1 : 0;
    }

    bool failed = (CSF_AT(uint8_t, state, 0) != 0);
    CSF_AT(int, state, 0x1C) = 0;
    if (!failed) {
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        Archive_SerializeObjectRef(self, reinterpret_cast<bool*>(a2));
    }

    CSF_AT(int, state, 0x1C) = savedField;

    if (CSF_AT(uint8_t, state, 0) == 0) {
        void* owner = CSF_AT(void*, state, 4);
        void** vtbl = CSF_AT(void**, owner, 0);
        // Full EAX is returned; only AL is tested
        int r = ((int(__fastcall*)(void*, void*, void*))vtbl[0x7C / 4])(owner, nullptr, a2);
        if ((r & 0xFF) != 0) {
            CSF_AT(uint8_t, state, 0) = 0;
            return r;
        }
    }
    CSF_AT(uint8_t, state, 0) = 1;
    return 1;
}

HOOK(0x004A6D30, S_004a6d30_ProcessStreamState, "auto");
