#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"

CSF_FUNC(void*, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(void*, __fastcall, GetGlobalSubObject_4FC0A0, 0x004FC0A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ProcessSubObjectRecord, 0x00443000, (void* self, void* edx, int a2));

// Reads a value from the chain *a3 -> +4 -> +4, builds a 3-slot record {5, value, 0}
// and hands it to the global sub-object's handler.
static int __stdcall DispatchGlobalSubObjectValue(int /*a1*/, int /*a2*/, int* a3) {
    int referencedObject = *a3;
    int innerObject = CSF_AT(int, referencedObject, 0x4);
    int recordValue = CSF_AT(int, innerObject, 0x4);

    int record[3];
    record[0] = 5;
    record[1] = recordValue;
    record[2] = 0;

    void* globalValue = GetGlobalValue_4FBDE0();
    void* globalSubObject = GetGlobalSubObject_4FC0A0(globalValue, nullptr);
    return ProcessSubObjectRecord(globalSubObject, nullptr, reinterpret_cast<int>(&record[0]));
}

HOOK(0x45B760, DispatchGlobalSubObjectValue, "auto");
