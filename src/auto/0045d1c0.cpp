#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, IsField1BCNonNegative, 0x00492B10, (void* self, void*));
CSF_FUNC(int, __cdecl, CheckHandleEntryFlag, 0x00755810, (unsigned int a1));

static int ReadDword(int address) {
    return *reinterpret_cast<int*>(static_cast<std::uintptr_t>(address));
}

static uint8_t* ByteAtOffset(int base, int offset) {
    return reinterpret_cast<uint8_t*>(static_cast<std::uintptr_t>(base + offset));
}

static void* AsObjectPointer(int value) {
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(value));
}

// Resolves the handle entry referenced by a3 in the current scene and stores
// the result of the entry's flag/visibility test into the byte at a1->field_04 + 1.
static int __stdcall RefreshHandleEntryFlag(int a1, int a2, int a3) {
    int result = ReadDword(ReadDword(a3) + 4) + 4;
    *ByteAtOffset(ReadDword(a1 + 4), 1) = 0;
    if (ReadDword(result + 8) >= 0) {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
        int resolvedObject = result;
        if (resolvedObject != 0) {
            result = CheckHandleEntryFlag(static_cast<unsigned int>(ReadDword(resolvedObject + 0x50)));
            if (static_cast<uint8_t>(result) != 0) {
                result = IsField1BCNonNegative(AsObjectPointer(resolvedObject), nullptr);
                *ByteAtOffset(ReadDword(a1 + 4), 1) = static_cast<uint8_t>(result);
            }
        }
    }
    return result;
}

HOOK(0x45D1C0, RefreshHandleEntryFlag, "auto");
