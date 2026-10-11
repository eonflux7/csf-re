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

// Preconditions check on the char task (original sub_4D7670, thiscall, no stack args).
CSF_FUNC(char, __fastcall, CheckCharTaskPrecondition, 0x004D7670, (void* self, void* edx));

// Returns the path/movement status of an object (original sub_4AEB90, thiscall, one ecx arg).
CSF_FUNC(int, __fastcall, GetPathStatus, 0x004AEB90, (void* self, void* edx));

// Starts movement for the char task toward its sub-object at +0x164 (original CharTask_StartMovement).
CSF_FUNC(int, __fastcall, CharTask_StartMovement, 0x004DB140, (void* self, void* edx, int a2));

static char __fastcall C_0082f9f8_TryStartMovement(C_0082f9f8* self, void* /*edx*/)
{
    char result = CheckCharTaskPrecondition(self, nullptr);
    if (result) {
        int pathStatus = GetPathStatus(CSF_AT(void*, self, 0x30C), nullptr);
        result = (char)pathStatus;
        if (pathStatus != -1) {
            if (pathStatus != 0) {
                result = (char)CharTask_StartMovement(self, nullptr, (int)((uint8_t*)self + 0x164));
            } else {
                result = CSF_AT(char, self, 0x33C);
                if (!result) {
                    result = (char)CharTask_StartMovement(self, nullptr, (int)((uint8_t*)self + 0x164));
                }
            }
        }
    }
    return result;
}

HOOK(0x004D7730, C_0082f9f8_TryStartMovement, "auto");
