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

CSF_FUNC(int, __fastcall, CoreZlibLog_BeginRecord, 0x006951B0, (void* self, void* edx, char a2, char a3, int a4));
CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_EndGroup_6951D0, 0x006951D0, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_WriteInteger, 0x006952F0, (void* self, void* edx, int a2, char* a3));
CSF_FUNC(int, __fastcall, CoreZlibLog_BeginGroup, 0x00695470, (void* self, void* edx, char* a2, int a3, char a4));

CSF_GLOBAL(const char*, g_pszKey_VERSION, 0x00899A9C);
CSF_GLOBAL(const char*, g_pszKey_OPCIONES, 0x00899AA0);

// Writes the options record: version key, then the "OPCIONES" group containing
// each non-null sub-option object's serializer (vtable slot 3), then closes the group.
static int __fastcall C_0083fdf4_SaveOptions(C_0083fdf4* self, void* /*edx*/, S_00694cd0* a2)
{
    CoreZlibLog_BeginRecord(a2, nullptr, 0, 0, 0);
    CoreZlibLog_WriteInteger(a2, nullptr, 8, const_cast<char*>(g_pszKey_VERSION));
    CoreZlibLog_BeginGroup(a2, nullptr, const_cast<char*>(g_pszKey_OPCIONES), 0, 0);

    uint32_t optionA = self->field_0004;
    if (optionA)
        ((void (__fastcall*)(void*, void*, S_00694cd0*))CSF_AT(void**, reinterpret_cast<void*>(optionA), 0)[3])(
            reinterpret_cast<void*>(optionA), nullptr, a2);

    uint32_t optionB = self->field_0008;
    if (optionB)
        ((void (__fastcall*)(void*, void*, S_00694cd0*))CSF_AT(void**, reinterpret_cast<void*>(optionB), 0)[3])(
            reinterpret_cast<void*>(optionB), nullptr, a2);

    uint32_t optionC = self->field_000c;
    if (optionC)
        ((void (__fastcall*)(void*, void*, S_00694cd0*))CSF_AT(void**, reinterpret_cast<void*>(optionC), 0)[3])(
            reinterpret_cast<void*>(optionC), nullptr, a2);

    uint32_t optionD = self->field_0010;
    if (optionD)
        ((void (__fastcall*)(void*, void*, S_00694cd0*))CSF_AT(void**, reinterpret_cast<void*>(optionD), 0)[3])(
            reinterpret_cast<void*>(optionD), nullptr, a2);

    CoreZlibLog_EndGroup_6951D0(a2, nullptr);
    return static_cast<int>(CoreZlibLog_EndGroup_6951D0(a2, nullptr));
}

HOOK(0x0050BD10, C_0083fdf4_SaveOptions, "auto");
