#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callees, bound by address (signatures per packet "bind as" lines).
CSF_FUNC(unsigned int*, __fastcall, SessionFlagBuffer_Init, 0x006530D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SessionFlagBuffer_Evaluate, 0x0065E200, (void* self, void* edx));
CSF_FUNC(void, __fastcall, SessionFlagBuffer_SetMode, 0x0065E130, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SessionScope_Begin, 0x00651C70, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(void, __fastcall, SessionScope_End, 0x00651B40, (void* self, void* edx));

// Returns 1 when the global session flag at this+0x98 is set and the
// flag buffer evaluates true; otherwise 0. The flag buffer and scope
// objects live on this function's stack.
static char __fastcall MenuMpPause_QueryGlobalFlagIsOne(void* self, void* /*edx*/)
{
    // Flag at this+0x98 (dword index 38). Zero means the query fails immediately.
    if (CSF_AT(uint32_t, self, 0x98) == 0)
        return 0;

    // Stack buffers, sized to the original frame: [ebp-0x28] = 4 bytes,
    // [ebp-0x24] = 0x20 bytes, [ebp-0x4] = dword.
    alignas(4) uint8_t flagBufferBytes[4];
    alignas(8) uint8_t flagScopeBytes[0x20];
    int scopeToken = 0;

    SessionFlagBuffer_Init(flagBufferBytes, nullptr);
    char evaluatedFlag = static_cast<char>(SessionFlagBuffer_Evaluate(flagBufferBytes, nullptr));
    SessionFlagBuffer_SetMode(flagBufferBytes, nullptr, 0);

    scopeToken = 0;
    SessionScope_Begin(flagScopeBytes, nullptr, reinterpret_cast<uint32_t*>(&scopeToken));
    SessionScope_End(flagScopeBytes, nullptr);

    return evaluatedFlag;
}

HOOK(0x0073E040, MenuMpPause_QueryGlobalFlagIsOne, "auto");
