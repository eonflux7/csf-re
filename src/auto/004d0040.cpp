#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, C_0083b580_SetActiveFlag, 0x004CF810, (void* self, void* edx, char a2));
CSF_FUNC(uint32_t*, __fastcall, C_0083b580_UpdateSubState, 0x004CFDA0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_0083b580_RunPostUpdate, 0x004CFE50, (void* self, void* edx));

// __thiscall(C_0083b580* this, int a2), ret 4
static char __fastcall C_0083b580_ApplyLinkedState(C_0083b580* self, void* /*edx*/, int /*unusedArg*/)
{
    C_00833908* linkedData = reinterpret_cast<C_00833908*>(self->field_0014);
    uintptr_t stateBase = 0;
    if (linkedData != nullptr) {
        uint32_t linkedWord = CSF_AT(uint32_t, linkedData, 0x18);
        if (linkedWord != 0) {
            stateBase = static_cast<uintptr_t>(linkedWord) - 0x80;
        }
    }

    int32_t newState = CSF_AT(int8_t, reinterpret_cast<uint8_t*>(stateBase), 0x4A);

    if (newState != static_cast<int32_t>(self->field_0044)) {
        switch (newState) {
        case 0:
            self->field_0044 = 0;
            break;
        case 1:
            C_0083b580_SetActiveFlag(self, nullptr, static_cast<char>(1));
            break;
        case 2:
            C_0083b580_SetActiveFlag(self, nullptr, static_cast<char>(0));
            break;
        case 3:
            self->field_0044 = 3;
            break;
        default:
            break;
        }
    }

    C_0083b580_UpdateSubState(self, nullptr);
    return C_0083b580_RunPostUpdate(self, nullptr);
}

HOOK(0x004D0040, C_0083b580_ApplyLinkedState, "auto");
