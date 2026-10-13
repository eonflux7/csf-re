#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Formats a record with a prefix. Stack args: a2, a3, a4, a5, a6 (ret 20).
CSF_FUNC(int, __fastcall, CoreLog_FormatPrefixedRecord, 0x00695030,
         (void* self, void* edx, int a2, char* a3, char a4, char a5, int a6));

// Begins a zlib log record: forwards to the formatter with a null first argument
// and the "[" prefix literal, then the three record arguments in their original order.
static int __fastcall S_00694cd0_BeginRecord(S_00694cd0* self, void* /*edx*/, int a2, int a3, int a4) {
    return CoreLog_FormatPrefixedRecord(
        self,
        nullptr,
        0,
        (char*)CSF_STR(0x008446C4, "["),
        static_cast<char>(a2),
        static_cast<char>(a3),
        a4);
}

HOOK(0x006951B0, S_00694cd0_BeginRecord, "auto");
