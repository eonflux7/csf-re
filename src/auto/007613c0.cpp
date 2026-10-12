#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Returns the message string at index `index` of the std future-error message table.
// Original: char* __cdecl _Get_future_error_what(int index) { return table[index]; }
// The table holds `char*` entries, so the element stride is sizeof(char*), not 1.
static char* __cdecl StdFutureError_GetWhatString(int index) {
    char** messageTable = reinterpret_cast<char**>(CSF_ADDR_DATA(0x008A9688));
    return messageTable[index];
}

HOOK(0x007613C0, StdFutureError_GetWhatString, "auto");
