#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void**, __cdecl, Get_GlobalObject_008A6788, 0x0069D5B0, ());
CSF_FUNC(char, __fastcall, CWeaponDefTable_ParseDataFile, 0x0078B370, (void* self, void* edx, int* a3, int arg4, char* String1));

// CWeaponDefTable (vtable 0x8728A0): m_pRecords lives at +0x008.
static char __fastcall CWeaponDefTable_LoadVersionedDataFile(void* self, void* /*edx*/, int fileVersion) {
    char* globalObject = reinterpret_cast<char*>(Get_GlobalObject_008A6788());
    int* recordsArray = reinterpret_cast<int*>(CSF_AT(uint32_t, self, 0x008));
    return CWeaponDefTable_ParseDataFile(self, nullptr, recordsArray, fileVersion, globalObject);
}

HOOK(0x78B4C0, CWeaponDefTable_LoadVersionedDataFile, "auto");
