#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void**, __cdecl, Get_GlobalObject_008A6788, 0x0069D5B0, ());
CSF_FUNC(char, __fastcall, C_0086f128_ParseDataFile, 0x00768460, (void* self, void* edx, int* a2, int a3, char* String1));

// C_0086f128 (vtable 0x86F128): the record pointer lives at +0x008.
static char __fastcall C_0086f128_LoadDataFileWithGlobal(void* self, void* /*edx*/, int fileVersion) {
    char* globalObject = reinterpret_cast<char*>(Get_GlobalObject_008A6788());
    int* recordsArray = reinterpret_cast<int*>(CSF_AT(uint32_t, self, 0x008));
    return C_0086f128_ParseDataFile(self, nullptr, recordsArray, fileVersion, globalObject);
}

HOOK(0x768580, C_0086f128_LoadDataFileWithGlobal, "auto");
