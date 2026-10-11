#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee at 0x6A1910: generic log-line appender. The literal 13 is the
// message-kind selector this wrapper always passes.
CSF_FUNC(int, __cdecl, DlgLog_AppendMessageKind, 0x006A1910, (uint32_t* a1, int a2, uint8_t* a3, int a4));

static int __cdecl DlgLog_AppendMessage(uint32_t* logWindow, uint8_t* message, int messageLength) {
    return DlgLog_AppendMessageKind(logWindow, 13, message, messageLength);
}

HOOK(0x006A1A10, DlgLog_AppendMessage, "auto");
