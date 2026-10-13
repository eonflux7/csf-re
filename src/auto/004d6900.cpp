#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_0083c478_CommitIndexRequest, 0x004D6520, (void* self, void* edx, int a2, char a3));
CSF_FUNC(char, __fastcall, C_0083c478_CommitFloatRequest, 0x004D6620, (void* self, void* edx, float a2, char a3));

// Consumes a pending selection request stored in the request object at a2
// (+0xA4 holds a float-bit value, +0xA8 holds an index; -1 means "no request").
// Dispatches the request to the matching commit routine and clears the consumed slot.
static char __fastcall C_0083c478_ConsumePendingIndex(C_0083c478* self, void* /*edx*/, int a2)
{
    uint32_t result = GetSceneMenuContext(g_pCurrentScene, nullptr);
    if (result == 0) {
        return static_cast<char>(result);
    }

    C_00833908* sceneMenuContext = reinterpret_cast<C_00833908*>(result);
    if (sceneMenuContext->field_0044 == 0) {
        return static_cast<char>(result);
    }

    result = self->field_0014;
    if (result == 0) {
        return static_cast<char>(result);
    }

    if (a2 == 0) {
        return static_cast<char>(result);
    }

    uint8_t* request = reinterpret_cast<uint8_t*>(a2);
    int pendingIndex = CSF_AT(int, request, 0xa8);

    if (pendingIndex == -1) {
        uint32_t pendingFloatBits = CSF_AT(uint32_t, request, 0xa4);
        result = pendingFloatBits;
        if (result != 0xFFFFFFFFu) {
            float pendingValue;
            std::memcpy(&pendingValue, &pendingFloatBits, sizeof(pendingValue));
            char commitResult = C_0083c478_CommitFloatRequest(self, nullptr, pendingValue, static_cast<char>(self->field_0055));
            CSF_AT(int, request, 0xa4) = -1;
            return commitResult;
        }
        return static_cast<char>(result);
    }

    char commitResult = C_0083c478_CommitIndexRequest(self, nullptr, pendingIndex, static_cast<char>(self->field_0055));
    CSF_AT(int, request, 0xa8) = -1;
    return commitResult;
}

HOOK(0x4D6900, C_0083c478_ConsumePendingIndex, "auto");
