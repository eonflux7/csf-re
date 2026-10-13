#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, _vsprintf, 0x007AFCAA, (char*, const char*, __builtin_va_list));
CSF_FUNC(char*, __cdecl, DuplicateCStringUntagged, 0x0069ADC0, (const char*));
static char* __cdecl FormatLinkedStatus(void* self, const char* format, ...) {
    char buffer[5120];
    __builtin_va_list args;
    __builtin_va_start(args, format);
    _vsprintf(buffer, format, args);
    __builtin_va_end(args);
    char* result = DuplicateCStringUntagged(buffer);
    CSF_AT(char*, self, 0x38 + CSF_AT(uint32_t, self, 0x7c)*4) = result;
    return result;
}
HOOK(0x0069AE30, FormatLinkedStatus, "auto");
