#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AppendThreeDwordRecord_75A3A0, 0x0075A3A0, (C_0085e7b8* self, void* , const uint32_t* record));

// Reads one state value: fetches the global state object, then queries it with `key`.
static int ReadGlobalStateValue(int key)
{
    void* stateObject = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalState_50D190()));
    return GetStateValue(stateObject, nullptr, key);
}

static int __fastcall AttackTasks_AppendSpreadRecords(void* /*self*/, void* /*edx*/, float* offset, float a3, C_0085e7b8* out)
{
    int spreadRange = static_cast<int>(a3);

    int result = ReadGlobalStateValue(4) + 2;
    if (result != 0)
    {
        int remaining = result;
        do
        {
            float xDelta = static_cast<float>(ReadGlobalStateValue(2 * spreadRange) - spreadRange);
            int zDelta = ReadGlobalStateValue(2 * spreadRange) - spreadRange;

            float record[3];
            record[0] = xDelta + offset[0];
            record[1] = offset[1];
            record[2] = static_cast<float>(zDelta) + offset[2];

            result = AppendThreeDwordRecord_75A3A0(out, nullptr, reinterpret_cast<const uint32_t*>(record));
            --remaining;
        } while (remaining != 0);
    }
    return result;
}

HOOK(0x0047BAD0, AttackTasks_AppendSpreadRecords, "auto");
