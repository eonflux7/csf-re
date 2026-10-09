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

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

// Walks the expression table held at this[1] (an array of node pointers terminated by a
// node whose +4 field is -1). Returns the first node whose +12 name matches String1
// case-insensitively. When String1 is null or nothing matches, returns the first array entry.
static int __fastcall MissionScript_FindExpressionDefinition(uint32_t* self, void* /*edx*/, char* String1)
{
    uint32_t* firstNode = reinterpret_cast<uint32_t*>(reinterpret_cast<uint32_t*>(self[1])[0]);
    if (firstNode[1] == 0xFFFFFFFFu)
        return static_cast<int>(reinterpret_cast<uint32_t*>(self[1])[0]);

    int index = 0;
    for (;;)
    {
        uint32_t* node = reinterpret_cast<uint32_t*>(reinterpret_cast<uint32_t*>(self[1])[index]);
        if (String1)
        {
            const char* nodeName = reinterpret_cast<const char*>(node[3]);
            if (nodeName && __stricmp(String1, nodeName) == 0)
                break;
        }

        uint32_t* nextNode = reinterpret_cast<uint32_t*>(reinterpret_cast<uint32_t*>(self[1])[index + 1]);
        index++;
        if (nextNode[1] == 0xFFFFFFFFu)
            return static_cast<int>(reinterpret_cast<uint32_t*>(self[1])[0]);
    }

    return static_cast<int>(reinterpret_cast<uint32_t*>(self[1])[index]);
}

HOOK(0x007692F0, MissionScript_FindExpressionDefinition, "auto");
