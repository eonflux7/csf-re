#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __stdcall, GlobalInt_FindNameId, 0x006C1770, (uint8_t* a1));
CSF_FUNC(char, __fastcall, Water_UpdateEntry, 0x0070B960, (void* self, void* , int arg, int key));

static void* IntToPtr(int value) {
    return reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(value)));
}

// Looks up a name id from the string object (SSO-aware) and forwards it to the water table.
static char __fastcall C_00861988_UpdateWaterEntryFromName(C_00861988* self, void* /*edx*/, uint8_t* stringObj) {
    char result = 0;
    if (CSF_AT(uint32_t, self, 0x0C) != 0) {
        const char* name;
        if (CSF_AT(uint32_t, stringObj, 0x18) < 0x10u) {
            name = reinterpret_cast<const char*>(stringObj + 4);
        } else {
            name = CSF_AT(const char*, stringObj, 4);
        }

        int nameId = 0;
        if (std::strlen(name) != 0) {
            int globalInt = GetGlobalInt();
            void* accessor = IntToPtr(GlobalInt_ResolveAccessor(IntToPtr(globalInt), nullptr));
            reinterpret_cast<void(__fastcall*)(void*, void*)>(CSF_AT(void**, accessor, 0)[4])(accessor, nullptr);
            nameId = GlobalInt_FindNameId(reinterpret_cast<uint8_t*>(const_cast<char*>(name)));
        }

        result = Water_UpdateEntry(IntToPtr(CSF_AT(int, self, 0x0C)), nullptr,
                                   static_cast<int>(reinterpret_cast<uintptr_t>(stringObj)), nameId);
    }
    return result;
}

HOOK(0x007029D0, C_00861988_UpdateWaterEntryFromName, "auto");
