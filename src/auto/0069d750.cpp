#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x00698350, __cdecl, pops 0. Opens a scene/config file by name.
CSF_FUNC(void*, __cdecl, CoreFiles_OpenSceneFile, 0x00698350,
         (const char* filename, char* mode, int openFlags));

// a2 is a string object: when its capacity field (+0x18) is below 16 the characters
// are stored inline at +4, otherwise +4 holds a pointer to the heap buffer.
static const char* CErrorLog_GetLogFileNameText(int stringObject) {
    uint32_t capacity = CSF_AT(uint32_t, reinterpret_cast<void*>(stringObject), 0x18);
    if (capacity < 0x10u) {
        return reinterpret_cast<const char*>(stringObject + 4);
    }
    return reinterpret_cast<const char*>(CSF_AT(uint32_t, reinterpret_cast<void*>(stringObject), 4));
}

// __thiscall bool C_0085ea2c::OpenReadFileIfClosed(int fileNameString)
// Opens the file for reading only if no handle is currently held.
static bool __fastcall CErrorLog_OpenReadFileIfClosed(C_0085ea2c* self, void* /*edx*/, int fileNameString) {
    if (self->field_0004 != 0) {
        return false;
    }

    const char* fileName = CErrorLog_GetLogFileNameText(fileNameString);
    void* fileHandle = CoreFiles_OpenSceneFile(
        fileName,
        const_cast<char*>(CSF_STR(0x83E130, "r")),
        0);

    self->field_0004 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(fileHandle));
    return self->field_0004 != 0;
}

HOOK(0x0069D750, CErrorLog_OpenReadFileIfClosed, "auto");
