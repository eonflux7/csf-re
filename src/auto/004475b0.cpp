#include <cstdint>
#include "globals.h"
#include "hook.h"

static int __fastcall WalkVisitorOverEmbeddedMembers(void* self, void* /*edx*/, uint8_t* state)
{
    const uint32_t savedContext = CSF_AT(uint32_t, state, 0x1C);
    bool done = false;
    if (CSF_AT(uint8_t, state, 0) == 0) {
        void* visitor = CSF_AT(void*, state, 4);
        done = !((uint8_t(__fastcall*)(void*, void*, int, uint8_t*))
            CSF_AT(void**, visitor, 0)[0x80 / 4])(visitor, nullptr, 0, state);
    }
    CSF_AT(uint8_t, state, 0) = done;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (!done) {
        void* visitor = CSF_AT(void*, state, 4);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            ((int(__fastcall*)(void*, void*, void*, uint8_t*))
                CSF_AT(void**, visitor, 0)[0x58 / 4])
                (visitor, nullptr, self, state) == -1;
    }

    bool visit = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (visit) {
        void* visitor = CSF_AT(void*, state, 4);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            ((int(__fastcall*)(void*, void*, void*, uint8_t*))
                CSF_AT(void**, visitor, 0)[0x38 / 4])
                (visitor, nullptr, static_cast<uint8_t*>(self) + 4, state) == -1;
    }

    visit = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (visit) {
        void* visitor = CSF_AT(void*, state, 4);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            ((int(__fastcall*)(void*, void*, void*, uint8_t*))
                CSF_AT(void**, visitor, 0)[0x54 / 4])
                (visitor, nullptr, static_cast<uint8_t*>(self) + 20, state) == -1;
    }

    visit = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (visit) {
        void* visitor = CSF_AT(void*, state, 4);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            ((int(__fastcall*)(void*, void*, void*, uint8_t*))
                CSF_AT(void**, visitor, 0)[0x54 / 4])
                (visitor, nullptr, static_cast<uint8_t*>(self) + 16, state) == -1;
    }

    visit = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (visit) {
        void* visitor = CSF_AT(void*, state, 4);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            ((int(__fastcall*)(void*, void*, void*, uint8_t*))
                CSF_AT(void**, visitor, 0)[0x54 / 4])
                (visitor, nullptr, static_cast<uint8_t*>(self) + 24, state) == -1;
    }

    visit = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (visit) {
        void* visitor = CSF_AT(void*, state, 4);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            ((int(__fastcall*)(void*, void*, void*, uint8_t*))
                CSF_AT(void**, visitor, 0)[0x54 / 4])
                (visitor, nullptr, static_cast<uint8_t*>(self) + 28, state) == -1;
    }

    CSF_AT(uint32_t, state, 0x1C) = savedContext;
    if (CSF_AT(uint8_t, state, 0) == 0) {
        void* visitor = CSF_AT(void*, state, 4);
        const int result = ((int(__fastcall*)(void*, void*, uint8_t*))
            CSF_AT(void**, visitor, 0)[0x7C / 4])(visitor, nullptr, state);
        if (static_cast<uint8_t>(result) != 0) {
            CSF_AT(uint8_t, state, 0) = 0;
            return result;
        }
    }
    CSF_AT(uint8_t, state, 0) = 1;
    return 1;
}

HOOK(0x004475B0, WalkVisitorOverEmbeddedMembers, "auto");
