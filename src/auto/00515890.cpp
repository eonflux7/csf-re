#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, OpenSceneFile, 0x698350, (const char* path, const char* mode, int flags));
CSF_FUNC(std::uint32_t, __cdecl, CloseSceneFile, 0x697980, (void* file));
CSF_FUNC(int, __fastcall, LengthPrefixedBuffer_TerminateAtLength, 0x694C20, (void* self, void*));

// Opens the file at a1 for writing, writes the length-prefixed buffer described by a2,
// closes the file, and reports whether the number of bytes written equals a2->field_000c.
static int __stdcall SaveGame_WriteBufferToFileAndVerify(int a1, S_00694cd0* a2)
{
    void* file = OpenSceneFile(reinterpret_cast<const char*>(a1), CSF_STR(0x840044, "w"), 0);
    if (file == nullptr)
        return 0;

    uint32_t expectedByteCount = a2->field_000c;
    int payloadLength = LengthPrefixedBuffer_TerminateAtLength(a2, nullptr);

    // Virtual slot 3 (offset 0x0C) of the file object: __thiscall(this, int length, uint32 count).
    int bytesWritten = ((int(__fastcall*)(void*, void*, int, uint32_t))CSF_AT(void**, file, 0)[3])(
        file, nullptr, payloadLength, expectedByteCount);

    CloseSceneFile(file);
    return bytesWritten == static_cast<int>(a2->field_000c) ? 1 : 0;
}

HOOK(0x515890, SaveGame_WriteBufferToFileAndVerify, "auto");
