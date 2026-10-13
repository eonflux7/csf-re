#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(uint8_t*, __cdecl, CoreTokenizer_ReplacePathCharacter, 0x00694BF0,
         (uint8_t* a1, char a2, char a3));
CSF_FUNC(bool, __cdecl, CoreFiles_RemoveBaseDirectoryPrefix, 0x00697470,
         (char* path));
CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41,
         (const char* left, const char* right));

static char __fastcall CoreTokenizer_ContainsFile(
    void* self, void* /*edx*/, const char* filename)
{
    alignas(8) uint8_t pathBytes[0x104];
    std::memset(pathBytes, 0, 0x100);
    char* path = reinterpret_cast<char*>(pathBytes);

    char* destination = path;
    const char* source = filename;
    do {
        *destination++ = *source;
    } while (*source++ != '\0');

    CoreFiles_RemoveBaseDirectoryPrefix(path);
    CoreTokenizer_ReplacePathCharacter(pathBytes, 92, 47);

    const uint32_t begin = CSF_AT(uint32_t, self, 0x10);
    uint32_t count = 0;
    if (begin != 0) {
        const uint32_t end = CSF_AT(uint32_t, self, 0x14);
        count = static_cast<uint32_t>(
            static_cast<int32_t>(end - begin) / 44);
    }

    uint32_t offset = 0;
    for (uint32_t index = 0; index < count; ++index, offset += 44) {
        uint8_t* entry = CSF_AT(uint8_t*, self, 0x10) + offset;
        const char* storedPath;
        if (CSF_AT(uint32_t, entry, 0x18) < 0x10)
            storedPath = reinterpret_cast<const char*>(entry + 4);
        else
            storedPath = CSF_AT(const char*, entry, 4);

        if (__stricmp(storedPath, path) == 0)
            return 1;
    }
    return 0;
}

HOOK(0x0069E830, CoreTokenizer_ContainsFile, "auto");
