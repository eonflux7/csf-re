#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Returns the i-th entry of the app_config string table (g_psz_ESTATICO_app_config, 4-byte char* slots).
static char* __cdecl AppConfig_GetEstaticoStringByIndex(int index) {
    return CSF_AT(char*, CSF_ADDR_DATA(0x8A9690), index * 4);
}

HOOK(0x007613D0, AppConfig_GetEstaticoStringByIndex, "auto");
