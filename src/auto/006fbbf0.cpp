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

CSF_FUNC(char, __fastcall, C_00862d48_ReleaseResources, 0x006FBB30, (C_00862d48* self, void* edx));
CSF_FUNC(char, __fastcall, sub_6FBB80, 0x006FBB80, (void* self, void* edx));
CSF_FUNC(char, __fastcall, InitializeScreenEffect, 0x007091D0, (void* self, void* edx, int a2));

static char __fastcall C_00862d48_StartEffect(C_00862d48* self, void* /*edx*/, int a2)
{
    C_00862d48_ReleaseResources(self, nullptr);

    char initOk = InitializeScreenEffect(self, nullptr, a2);

    int slotIndex = 0;
    uint32_t* slot = &self->field_0060;
    do {
        if (!initOk) {
            break;
        }
        *slot = 0;
        ++slotIndex;
        ++slot;
    } while (slotIndex < 5);

    // Virtual slot at byte offset 0x2C (index 11), no return value used.
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x2C / 4])(self, nullptr);

    char effectReady = sub_6FBB80(self, nullptr);
    CSF_AT(uint8_t, self, 0x8) = 1;
    if (!effectReady) {
        C_00862d48_ReleaseResources(self, nullptr);
    }
    return effectReady;
}

HOOK(0x006FBBF0, C_00862d48_StartEffect, "auto");
