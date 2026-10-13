// DataSetPerfCounters.cpp: builds a "name,value" string for a perf counter data set.
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

// Original callees, bound by address.
CSF_FUNC(void*, __cdecl, AllocateGatheringStorage, 0x00647160, (std::uint32_t size, const char* sourceFile, int sourceLine));
CSF_FUNC(uint64_t*, __cdecl, Perf_GetStringRefFields, 0x006474D0, (uint64_t* a1));
CSF_FUNC(uint32_t*, __fastcall, TempString_Init, 0x0064A690, (void* self, void* edx));
CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0, (void* self, void* edx));
CSF_FUNC(char*, __cdecl, FormatValueString, 0x0064A8F0, (void** a1, char* Format, ...));
CSF_FUNC(void*, __fastcall, Perf_CreatePairEntry, 0x00688540, (void* self, void* edx, char* a2, int arg4, int a3));

static void __fastcall S_0078cea0_FormatNameValuePair(S_0078cea0* self, void* /*edx*/, C_008310b8** outPtr, const char* suffix)
{
    // Temporary string object lives on this frame (sub esp, 0xC).
    // Its first dword is the string buffer pointer (the "vtable" slot in Hex-Rays naming).
    alignas(8) uint8_t v7Bytes[12];
    C_008310b8& v7 = *reinterpret_cast<C_008310b8*>(v7Bytes);

    CSF_AT(void*, &v7, 0) = nullptr;
    TempString_Init(&v7, nullptr);
    FormatValueString(reinterpret_cast<void**>(&v7), (char*)CSF_STR(0x872CD4, "%s,%s"), self->field_0000, suffix);

    void* gatheringStorage = AllocateGatheringStorage(80, CSF_STR(0x872CDC, "DataSetPerfCounters.cpp"), 40);
    if (gatheringStorage) {
        char* v5 = CSF_AT(char*, &v7, 0);
        uint32_t* v6 = reinterpret_cast<uint32_t*>(Perf_GetStringRefFields(reinterpret_cast<uint64_t*>(&v7.field_0004)));
        *outPtr = reinterpret_cast<C_008310b8*>(Perf_CreatePairEntry(gatheringStorage, nullptr, v5, (int)v6[0], (int)v6[1]));
    } else {
        *outPtr = nullptr;
    }

    DestroyTemporaryString(&v7, nullptr);
}

HOOK(0x78CEA0, S_0078cea0_FormatNameValuePair, "auto");
