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

// Original: sprintf(buffer, "%s\r\n", name); RwStreamWrite(stream, buffer, strlen(buffer)); return obj;

CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));
CSF_FUNC(uint32_t*, __cdecl, RwStreamWrite, 0x00629530, (uint32_t* a1, void* a2, unsigned int a3));

static int __cdecl TexDict_WriteNameLineToStream(int a1, int a2)
{
    char lineBuffer[512];

    const char* objectName = reinterpret_cast<const char*>(static_cast<uintptr_t>(a1 + 0x10));
    _sprintf(lineBuffer, CSF_STR(0x85E6E8, "%s\r\n"), objectName);
    RwStreamWrite(reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(a2)),
                  lineBuffer,
                  static_cast<unsigned int>(std::strlen(lineBuffer)));
    return a1;
}

HOOK(0x6C1160, TexDict_WriteNameLineToStream, "auto");
