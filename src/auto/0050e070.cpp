#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void, __cdecl, SceneLoader_SelectArchive, 0x00698960, (char* String1));
CSF_FUNC(int*, __cdecl, GetGlobalIntPtr, 0x0069C580, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_GLOBAL(const char*, g_pszMenu_P_Idioma, 0x00899AC0);

// Global state blocks are returned as 32-bit integers; turn them into byte-addressable pointers.
static void* AsGlobalBlockPointer(int value) {
    return reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(value)));
}

static int __fastcall BddLoader_RunLoadPhase(void* self, void* /*edx*/) {
    // If the flag at global block +0xA5C is set, select the archive named by the int* global.
    if (CSF_AT(uint8_t, AsGlobalBlockPointer(GetGlobalValue_8BC1E0()), 0xA5C)) {
        SceneLoader_SelectArchive(reinterpret_cast<char*>(GetGlobalIntPtr()));
    }

    // Virtual call: this->vtbl[0x24 / 4](this)
    using VoidMethodFn = void(__fastcall*)(void*, void*);
    reinterpret_cast<VoidMethodFn>(CSF_AT(void**, self, 0)[0x24 / 4])(self, nullptr);

    int result = GetGlobalValue_8B0568();
    if (!CSF_AT(uint8_t, AsGlobalBlockPointer(result), 0xB4)) {
        result = GetGlobalValue_8BC1E0();
        if (!CSF_AT(uint8_t, AsGlobalBlockPointer(result), 0xA6C)) {
            // Virtual call: this->vtbl[0x2C / 4](this, this->field_0xEC, g_pszMenu_P_Idioma)
            using LoadMenuFn = int(__fastcall*)(void*, void*, uint32_t, const char*);
            return reinterpret_cast<LoadMenuFn>(CSF_AT(void**, self, 0)[0x2C / 4])(
                self, nullptr, CSF_AT(uint32_t, self, 0xEC), g_pszMenu_P_Idioma);
        }
    }
    return result;
}

HOOK(0x0050E070, BddLoader_RunLoadPhase, "auto");
