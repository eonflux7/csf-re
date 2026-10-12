#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"

// Virtual slot 0x1C / 4 = 7: the setter of the sink object, called thiscall as (key, value).
static void __fastcall DynamicObj_CallSetter(void* self, void* /*edx*/, int key, int value)
{
    ((void(__fastcall*)(void*, void*, int, int))CSF_AT(void**, self, 0)[7])(self, nullptr, key, value);
}

// Pushes a record's fields into the sink object. mode 0 = full record (3 values),
// mode 4 = single value, mode 5 = three values with the byte field at +12.
static void __cdecl DynamicObj_SetFieldsFromRecord(void* sink, int mode, uint32_t* record)
{
    if (record == nullptr)
        return;

    if (mode == 0)
    {
        DynamicObj_CallSetter(sink, nullptr, 0, (int)record[3]);
        DynamicObj_CallSetter(sink, nullptr, 1, (int)record[4]);
        DynamicObj_CallSetter(sink, nullptr, 2, (int)record[5]);
    }
    else if (mode == 4)
    {
        DynamicObj_CallSetter(sink, nullptr, 0, (int)record[3]);
    }
    else if (mode == 5)
    {
        DynamicObj_CallSetter(sink, nullptr, 0, (int)record[4]);
        DynamicObj_CallSetter(sink, nullptr, 1, (int)reinterpret_cast<uint8_t*>(record)[12]);
        DynamicObj_CallSetter(sink, nullptr, 3, (int)record[5]);
    }
}

HOOK(0x442AD0, DynamicObj_SetFieldsFromRecord, "auto");
