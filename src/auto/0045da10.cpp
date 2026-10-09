#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, DefaultDestination_Get, 0x004FE4A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_44BB80, 0x0044BB80, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_44BBA0, 0x0044BBA0, (void* self, void* edx, int a2));

// Walks the entries of the default destination list. For the first entry whose
// key (+0x38) equals the integer part of the float keyed by a3, it stores
// (entry->+0x3c == 0) into the byte at *(a1+4)+1 and stops.
static char __stdcall ScanEntriesSetFlagByKey(int a1, int /*a2*/, int a3)
{
    void* globalValue = reinterpret_cast<void*>(GetGlobalValue_8B0568());
    void* destination = DefaultDestination_Get(globalValue, nullptr);

    uint8_t* keyHolder = *reinterpret_cast<uint8_t**>(a3);
    uint8_t* keySource = *reinterpret_cast<uint8_t**>(keyHolder + 4);
    float keyFloat = CSF_AT(float, keySource, 4);
    int key = static_cast<int>(keyFloat);

    char result = 0;
    unsigned int index = 0;
    for (;;)
    {
        unsigned int entryCount = static_cast<unsigned int>(sub_44BB80(destination, nullptr));
        if (index >= entryCount)
        {
            result = static_cast<char>(entryCount);
            break;
        }

        uint8_t* entry = reinterpret_cast<uint8_t*>(sub_44BBA0(destination, nullptr, static_cast<int>(index)));
        bool isMatch = CSF_AT(int, entry, 0x38) == key;
        if (isMatch)
        {
            char flag = (CSF_AT(int, entry, 0x3C) == 0) ? 1 : 0;
            uint8_t* target = CSF_AT(uint8_t*, reinterpret_cast<uint8_t*>(a1), 4);
            CSF_AT(uint8_t, target, 1) = static_cast<uint8_t>(flag);
            result = flag;
            break;
        }
        ++index;
    }
    return result;
}

HOOK(0x0045DA10, ScanEntriesSetFlagByKey, "auto");
