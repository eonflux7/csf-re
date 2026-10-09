#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Resolves the handle stored in the record reached through a4[0], calls the
// control's vtable slot 42 with a 4-dword request block, then copies two
// fields from the record into self and clears the flag at self+0x82.
static int __fastcall ApplyHandleRecordToGuiControl(void* self, void* /*edx*/, int a2, int a3, uint32_t* a4)
{
    (void)a2;
    (void)a3;

    void* ownerPtr = reinterpret_cast<void*>(static_cast<uintptr_t>(a4[0]));
    uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, ownerPtr, 4)));
    int result = CSF_AT(int, entry, 12);

    if (result >= 0)
    {
        int resolvedControl = CHandleTable_Resolve(g_pCurrentScene, nullptr,
            static_cast<int>(reinterpret_cast<uintptr_t>(entry + 4)));
        result = resolvedControl;

        if (result)
        {
            int requestBlock[4] = {0, 0, 0, 0};
            void* sourceOwnerPtr = reinterpret_cast<void*>(static_cast<uintptr_t>(a4[1]));
            uint8_t* sourceRecord = reinterpret_cast<uint8_t*>(
                static_cast<uintptr_t>(CSF_AT(uint32_t, sourceOwnerPtr, 4))) + 4;

            requestBlock[0] = 35;
            requestBlock[1] = CSF_AT(int, sourceRecord, 0);
            requestBlock[2] = CSF_AT(int, sourceRecord, 4);
            requestBlock[3] = CSF_AT(int, sourceRecord, 8);

            void* control = reinterpret_cast<void*>(static_cast<uintptr_t>(result));
            void* controlPart = CSF_AT(void*, control, 0x34);
            void** partVtbl = CSF_AT(void**, controlPart, 0);
            result = ((int(__fastcall*)(void*, void*, int*))partVtbl[42])(controlPart, nullptr, &requestBlock[0]);

            CSF_AT(uint32_t, self, 0x60) = CSF_AT(uint32_t, entry + 4, 4);
            CSF_AT(uint32_t, self, 0x64) = CSF_AT(uint32_t, entry + 4, 8);
            CSF_AT(uint8_t, self, 0x82) = 0;
        }
    }

    return result;
}

HOOK(0x00460D30, ApplyHandleRecordToGuiControl, "auto");
