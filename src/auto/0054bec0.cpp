#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, TypeRegistry_AppendElement8, 0x0054BE50, (void* self, void* , uint32_t* source));

CSF_GLOBAL(uint32_t, g_IndentWord, 0x00844294);
CSF_GLOBAL(uint8_t, g_IndentTerminator, 0x00844298);

// Enters a nested serialization scope: registers a type entry in the registry at +0x88,
// bumps the nesting counter, appends the indent text to the running name buffer, and
// forwards the call to the child object at +0xA4 (vtable slot 30).
static bool __fastcall C_008442a0_EnterNestedScope(C_008442a0* self, void* /*edx*/, int a2, int a3)
{
    void* child = reinterpret_cast<void*>(self->field_00a4);
    uint32_t entry[2];
    entry[0] = ((uint32_t(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[36])(child, nullptr);
    entry[1] = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x0084429C));
    TypeRegistry_AppendElement8(reinterpret_cast<uint8_t*>(self) + 0x88, nullptr, entry);

    ++self->field_0004;

    char* cursor = reinterpret_cast<char*>(&self->field_0004) + 3;
    while (*++cursor) {
    }
    CSF_AT(uint32_t, cursor, 0) = g_IndentWord;
    CSF_AT(uint8_t, cursor, 4) = g_IndentTerminator;

    uint32_t childObj = self->field_00a4;
    if (childObj) {
        void* childPtr = reinterpret_cast<void*>(childObj);
        ((void(__fastcall*)(void*, void*, int, int))CSF_AT(void**, childPtr, 0)[30])(childPtr, nullptr, a2, a3);
    }
    return true;
}

HOOK(0x0054BEC0, C_008442a0_EnterNestedScope, "auto");
