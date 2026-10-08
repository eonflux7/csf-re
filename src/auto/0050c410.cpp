#include <cstdint>
#include "hook.h"
#include "globals.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

// Resets a CMenuScreenFader's fade state to idle and releases its owned block (+0x48).
// The incoming edx is forwarded unchanged to the string-assign callee, as in the original.
static int __fastcall CMenuScreenFader_ResetState(CMenuScreenFader* self, void* edx)
{
    AssignStringRange_429B60(reinterpret_cast<uint8_t*>(self) + 4, edx,
                         reinterpret_cast<char*>(&g_byte_82E65C), 0u);

    int result = 0;
    void* ownedBlock = reinterpret_cast<void*>(self->field_0048);
    self->m_bIdle = 1;
    self->field_0030 = 0.0f;
    self->m_state = 0;
    self->m_fadeProgress = 0.0f;

    if (ownedBlock != nullptr)
    {
        // eax after the call is the callee's residual return value, as in the original.
        result = Mem_Free(ownedBlock);
        self->field_0048 = 0;
    }
    return result;
}

HOOK(0x0050C410, CMenuScreenFader_ResetState, "auto");
