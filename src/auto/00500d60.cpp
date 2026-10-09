#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Walks records of 0x58 bytes in [firstRecord, endRecord): releases the
// ref-counted helper object at +0x48, re-points three vtable slots at
// 0x82D410, frees the buffer at +4 when its capacity (+0x18) is >= 16, and
// resets the record's size/capacity/first-byte state.
static int __cdecl SceneRecordRange_Reset(int firstRecord, int endRecord, int /*unused1*/, int /*unused2*/)
{
    int result = 0;
    void* const vtbl = CSF_VTBL(0x0082D410);

    for (int record = firstRecord; record != endRecord; record += 0x58)
    {
        uint8_t* rec = reinterpret_cast<uint8_t*>(record);

        void* helper = CSF_AT(void*, rec, 0x48);
        if (helper)
        {
            uint8_t* helperHeader = reinterpret_cast<uint8_t*>(helper) - 4;
            if (CSF_AT(int, helperHeader, 0) != 0)
            {
                // Ref-counted object: call its deleting destructor (slot 0, flag 3).
                ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, helper, 0)[0])(helper, nullptr, 3);
            }
            else
            {
                Mem_Free(helperHeader);
            }
            CSF_AT(void*, rec, 0x48) = nullptr;
        }

        CSF_AT(void*, rec, 0x3C) = vtbl;
        CSF_AT(void*, rec, 0x30) = vtbl;
        CSF_AT(void*, rec, 0x24) = vtbl;
        result = static_cast<int>(reinterpret_cast<intptr_t>(vtbl));

        if (CSF_AT(uint32_t, rec, 0x18) >= 0x10u)
        {
            void* buffer = CSF_AT(void*, rec, 4);
            result = static_cast<int>(reinterpret_cast<intptr_t>(buffer));
            if (result)
                result = Mem_FreeDebug(buffer);
        }

        CSF_AT(int, rec, 0x18) = 15;
        CSF_AT(int, rec, 0x14) = 0;
        CSF_AT(uint8_t, rec, 4) = 0;
    }
    return result;
}

HOOK(0x00500D60, SceneRecordRange_Reset, "auto");
