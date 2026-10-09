#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, FindActiveEntryIndexByKey, 0x004DC5B0, (void* self, void* , void* keyObject));
CSF_FUNC(int, __fastcall, ResolveEntryStateRecord, 0x004DC6E0, (void* self, void* edx, int a2));

static char __stdcall UpdateActiveStateFlag(void* a1, void* /*a2*/, void* a3)
{
    // *(*(a1 + 4) + 1) = 0
    uint8_t* flagOwner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, a1, 4));
    flagOwner[1] = 0;

    // v3 = *(*(a3) + 4); activeEntryIndex = v3->field_0C; keyObject = v3 + 4
    uint32_t entryRef = CSF_AT(uint32_t, a3, 0);
    uint8_t* v3 = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, entryRef, 4));
    int activeEntryIndexByKey = CSF_AT(int, v3, 12);
    void* keyObject = v3 + 4;

    if (activeEntryIndexByKey >= 0)
    {
        void* root = reinterpret_cast<void*>(GetValue8B0554());
        uint8_t* searchSelf = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, root, 0x1FD8)) + 0x34;
        activeEntryIndexByKey = FindActiveEntryIndexByKey(searchSelf, nullptr, keyObject);

        if (activeEntryIndexByKey != -1)
        {
            void* rootAgain = reinterpret_cast<void*>(GetValue8B0554());
            uint8_t* stateSelf = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, rootAgain, 0x1FD8)) + 0x34;
            int* stateRecord = reinterpret_cast<int*>(ResolveEntryStateRecord(stateSelf, nullptr, activeEntryIndexByKey));
            uint8_t isStateTwo = (*stateRecord == 2) ? 1 : 0;

            uint8_t* ownerAfterCall = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, a1, 4));
            ownerAfterCall[1] = isStateTwo;
            activeEntryIndexByKey = isStateTwo;
        }
    }

    return static_cast<char>(activeEntryIndexByKey);
}

HOOK(0x0045D060, UpdateActiveStateFlag, "auto");
