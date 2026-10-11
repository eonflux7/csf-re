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

CSF_FUNC(uint32_t*, __cdecl, RwStreamWrite, 0x00629530, (uint32_t* a1, void* a2, unsigned int a3));
CSF_FUNC(uint32_t*, __cdecl, RwStreamWriteInt32, 0x00629C70, (uint32_t* a1, void* a2, unsigned int a3));

// Writes a length-prefixed (int32) string taken from a string object: inline buffer at +4
// when capacity (+0x18) < 0x10, otherwise heap pointer stored at +4.
static bool __cdecl GfxObjectProps_WriteLengthPrefixedString(uint32_t* stream, uint8_t* strObj,
                                                             int a3, int a4, int a5)
{
    (void)a3;
    (void)a4;
    (void)a5;

    const char* text = nullptr;
    int32_t length = 0;

    if (strObj != nullptr)
    {
        if (CSF_AT(uint32_t, strObj, 0x18) < 0x10u)
            text = reinterpret_cast<const char*>(strObj + 4);
        else
            text = reinterpret_cast<const char*>(CSF_AT(uint32_t, strObj, 4));

        const char* scan = text;
        while (*scan != '\0')
            ++scan;
        length = static_cast<int32_t>(scan - text);
    }

    bool result = RwStreamWriteInt32(stream, &length, 4) != nullptr;

    if (length > 0)
    {
        if (CSF_AT(uint32_t, strObj, 0x18) < 0x10u)
            return RwStreamWrite(stream, strObj + 4, static_cast<unsigned int>(length)) != nullptr;
        else
            return RwStreamWrite(stream, reinterpret_cast<void*>(CSF_AT(uint32_t, strObj, 4)),
                                 static_cast<unsigned int>(length)) != nullptr;
    }

    return result;
}

HOOK(0x6B90B0, GfxObjectProps_WriteLengthPrefixedString, "auto");
