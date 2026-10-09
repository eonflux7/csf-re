#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobal8afc7c, 0x0043E010, ());
CSF_FUNC(bool, __fastcall, sub_43F6E0, 0x0043F6E0, (void* self, void* edx, int* a2));

// Looks up a value through the global at 0x8afc7c, runs the test at 0x43F6E0 on a
// pointer derived from the record chain in a3, and stores the 8-bit result at
// (object a1 + 4)->+1.
static int __stdcall StoreChainLookupResultByte(int a1, int a2, int a3)
{
    // v3 = a1->field_0004 (pointer to the byte target)
    uint32_t v3 = CSF_AT(uint32_t, reinterpret_cast<void*>(a1), 0x4);

    // v5 = (*(a3)->field_0004) + 4 : dereference a3 -> record P, then P+4 -> Q, then Q+4
    uint32_t recordP = CSF_AT(uint32_t, reinterpret_cast<void*>(a3), 0x0);
    uint32_t recordQ = CSF_AT(uint32_t, reinterpret_cast<void*>(recordP), 0x4);
    int* v5 = reinterpret_cast<int*>(recordQ + 4);

    int globalValue = GetGlobal8afc7c();
    int result = sub_43F6E0(reinterpret_cast<void*>(globalValue), nullptr, v5);

    CSF_AT(uint8_t, reinterpret_cast<void*>(v3), 0x1) = static_cast<uint8_t>(result);
    return result;
}

HOOK(0x45B970, StoreChainLookupResultByte, "auto");
