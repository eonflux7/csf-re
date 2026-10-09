#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, SubmitRecordToGlobalSubObject, 0x00443000, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetGlobalSubObject_4FC0A0, 0x004FC0A0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());

static int ReadDwordAt(int address, int offset) {
    return *CSF_AT(int*, reinterpret_cast<void*>(address), offset);
}

static float ReadFloatAt(int address, int offset) {
    return *CSF_AT(float*, reinterpret_cast<void*>(address), offset);
}

static int __stdcall BuildAndSubmitVec3Record(int a1, int a2, int* a3) {
    (void)a1;
    (void)a2;

    // Record layout: { 3, component0, 0, component1, component2, component3 }
    int record[6];
    record[0] = 3;
    record[1] = 0;
    record[2] = 0;

    // a3[0] -> +4 -> +4 : integer component
    record[1] = ReadDwordAt(ReadDwordAt(a3[0], 4), 4);

    // a3[1] -> +4 -> +4 : float component, truncated to int
    record[3] = static_cast<int>(ReadFloatAt(ReadDwordAt(a3[1], 4), 4));

    // a3[2] -> +4 -> +4 : float component, truncated to int
    record[4] = static_cast<int>(ReadFloatAt(ReadDwordAt(a3[2], 4), 4));

    // a3[3] -> +4 -> +4 : float component, truncated to int
    record[5] = static_cast<int>(ReadFloatAt(ReadDwordAt(a3[3], 4), 4));

    void* globalValue = reinterpret_cast<void*>(GetGlobalValue_4FBDE0());
    void* subObject = GetGlobalSubObject_4FC0A0(globalValue, nullptr);
    return SubmitRecordToGlobalSubObject(subObject, nullptr, reinterpret_cast<int>(record));
}

HOOK(0x0045B6E0, BuildAndSubmitVec3Record, "auto");
