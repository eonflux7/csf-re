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

CSF_FUNC(int, __fastcall, TypeRegistry_AppendElement8, 0x0054BE50, (void* self, void* , uint32_t* source));

CSF_GLOBAL(uint32_t, g_IndentWord, 0x00844294);
CSF_GLOBAL(uint8_t, g_IndentTerminator, 0x00844298);

static char __fastcall C_008442a0_PushTypeEntry(C_008442a0* self, void* /*edx*/, int a2, int a3)
{
    void* sourceObj = reinterpret_cast<void*>(self->field_00a4);

    uint32_t entryElement[2];
    entryElement[0] = ((uint32_t(__fastcall*)(void*, void*))CSF_AT(void**, sourceObj, 0)[36])(sourceObj, nullptr);
    entryElement[1] = static_cast<uint32_t>(a2);
    TypeRegistry_AppendElement8(reinterpret_cast<uint8_t*>(self) + 0x88, nullptr, entryElement);

    self->field_0004 = self->field_0004 + 1;

    uint8_t* cursor = reinterpret_cast<uint8_t*>(self) + 7;
    do {
        ++cursor;
    } while (*cursor != 0);

    CSF_AT(uint32_t, cursor, 0) = g_IndentWord;
    CSF_AT(uint8_t, cursor, 4) = g_IndentTerminator;

    uint32_t childAddr = self->field_00a4;
    if (childAddr != 0) {
        void* child = reinterpret_cast<void*>(childAddr);
        auto childFn = reinterpret_cast<char(__fastcall*)(void*, void*, int, int)>(CSF_AT(void**, child, 0)[32]);
        return childFn(child, nullptr, a2, a3);
    }
    return 1;
}

HOOK(0x0054BFB0, C_008442a0_PushTypeEntry, "auto");
