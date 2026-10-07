#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

using LookupNameTable = const char*[];
CSF_GLOBAL(LookupNameTable, g_LookupNames, 0x008A8BD0);

static int __stdcall GetLookupValueByNameIndex(int nameIndex)
{
    void* lookup = CSF_AT(
        void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FCC);

    const char* name = g_LookupNames[nameIndex];
    int entryIndex =
        reinterpret_cast<int(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, lookup, 0)[3])(lookup, nullptr, name);

    const std::int32_t* value =
        reinterpret_cast<const std::int32_t*(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, lookup, 0)[2])(lookup, nullptr, entryIndex);

    return *value;
}

HOOK(0x0051F770, GetLookupValueByNameIndex, "auto");
