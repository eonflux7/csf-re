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

CSF_FUNC(void, __stdcall, PrepareStaticAiState, 0x004DC3C0, (void* a1));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, C_00834fa8_RunStateHandlerAtOffset14, 0x004210C0, (C_00834fa8* self, void*));

// Runs the state handler of the object linked to this AI state.
static int __stdcall AiState_RunLinkedStateHandler(void* a1)
{
    PrepareStaticAiState(a1);

    int linkedObjectValue = GetLinkedObjectValue_4DB5C0(a1, nullptr);
    uintptr_t linkedBase = 0;
    if (linkedObjectValue != 0)
        linkedBase = static_cast<uintptr_t>(static_cast<uint32_t>(linkedObjectValue) - 4u);

    uint8_t* linkedObj = reinterpret_cast<uint8_t*>(linkedBase);
    uint8_t* levelData = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, linkedObj, 0x2C));
    uint8_t* gameData = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, levelData, 0x34));
    C_00834fa8* handlerOwner = reinterpret_cast<C_00834fa8*>(CSF_AT(uint32_t, gameData, 0x310));

    return C_00834fa8_RunStateHandlerAtOffset14(handlerOwner, nullptr);
}

HOOK(0x4AD560, AiState_RunLinkedStateHandler, "auto");
