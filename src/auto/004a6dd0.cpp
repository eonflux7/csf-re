#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_4A6D30, 0x004A6D30, (void* self, void* edx, int a2));

// Runs a sequence of checks via the object's virtual methods (slots 32, 18, 31) and
// stores the final success flag into the first byte of the request structure.
static int __fastcall S_004a6d30_RunChecks(void* self, void* /*edx*/, int request)
{
    uint8_t* req = reinterpret_cast<uint8_t*>(request);
    uint8_t* thisBytes = reinterpret_cast<uint8_t*>(self);

    int savedField = CSF_AT(int, req, 0x1C);

    void** target = CSF_AT(void**, req, 4);
    uint8_t flag;
    if (req[0] == 0) {
        typedef bool(__fastcall * Slot32Fn)(void*, void*, int, int);
        bool r = ((Slot32Fn)CSF_AT(void**, target, 0)[0x80 / 4])(target, nullptr, 0, request);
        flag = r ? 0 : 1;
    } else {
        flag = 0;
    }
    req[0] = flag;
    CSF_AT(int, req, 0x1C) = 0;

    if (req[0] == 0) {
        req[0x15] = 0;
        req[0x16] = 0;
        sub_4A6D30(self, nullptr, request);
    }

    CSF_AT(int, req, 0x1C) = 0;
    if (req[0] == 0) {
        void** obj = CSF_AT(void**, req, 4);
        req[0x15] = 0;
        req[0x16] = 0;
        typedef int(__fastcall * Slot18Fn)(void*, void*, void*, int);
        int r = ((Slot18Fn)CSF_AT(void**, obj, 0)[0x48 / 4])(obj, nullptr, thisBytes + 8, request);
        req[0] = (r == -1) ? 1 : 0;
    }

    CSF_AT(int, req, 0x1C) = 0;
    if (req[0] == 0) {
        void** obj = CSF_AT(void**, req, 4);
        req[0x15] = 0;
        req[0x16] = 0;
        typedef int(__fastcall * Slot18Fn)(void*, void*, void*, int);
        int r = ((Slot18Fn)CSF_AT(void**, obj, 0)[0x48 / 4])(obj, nullptr, thisBytes + 0xC, request);
        req[0] = (r == -1) ? 1 : 0;
    }

    CSF_AT(int, req, 0x1C) = savedField;
    if (req[0] == 0) {
        void** obj = CSF_AT(void**, req, 4);
        typedef int(__fastcall * Slot31Fn)(void*, void*, int);
        int r = ((Slot31Fn)CSF_AT(void**, obj, 0)[0x7C / 4])(obj, nullptr, request);
        if ((uint8_t)r != 0) {
            req[0] = 0;
            return r;
        }
    }
    req[0] = 1;
    return 1;
}

HOOK(0x004A6DD0, S_004a6d30_RunChecks, "auto");
