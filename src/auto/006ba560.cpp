#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);
CSF_GLOBAL(uint32_t, g_RwEngineInstanceOffset, 0x008BB474);

CSF_FUNC(uint32_t*, __fastcall, GfxObjectProps_ResetRecordSlot, 0x006B8020, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxObjectProps_ReleaseRecord, 0x006B96C0, (void* self, void* edx, int* a2));
CSF_FUNC(void*, __fastcall, GfxObjectProps_LoadRecord, 0x006BA370, (void* self, void* edx, void* a2));

// Resolves the rw engine subobject, releases any previously stored property record,
// and, when a new source record with a nonzero payload is given, reloads the record
// and stores the new handle into the slot pointed to by this->field_0010.
static C_0082d410* __fastcall GfxObjectProps_AssignPropertyRecord(S_006b8020* self, void* /*edx*/, C_0082d410* sourceRecord)
{
    S_006b9130* engineSubobject = *reinterpret_cast<S_006b9130**>(
        reinterpret_cast<char*>(g_pRwEngineInstance) + g_RwEngineInstanceOffset);

    C_0082d410* result = reinterpret_cast<C_0082d410*>(static_cast<uintptr_t>(self->field_0010));
    if (result)
    {
        result = reinterpret_cast<C_0082d410*>(static_cast<uintptr_t>(
            GfxObjectProps_ReleaseRecord(engineSubobject, nullptr, reinterpret_cast<int*>(self->field_0010))));
    }

    if (sourceRecord)
    {
        // a2[1].field_0008 as laid out in the original: dword at +0x14.
        uint32_t sourcePayload = CSF_AT(uint32_t, sourceRecord, 0x14);
        result = reinterpret_cast<C_0082d410*>(static_cast<uintptr_t>(sourcePayload));
        if (result)
        {
            GfxObjectProps_ResetRecordSlot(self, nullptr);
            void* loadedRecord = GfxObjectProps_LoadRecord(engineSubobject, nullptr, sourceRecord);
            result = reinterpret_cast<C_0082d410*>(loadedRecord);
            *reinterpret_cast<uint32_t*>(self->field_0010) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(loadedRecord));
        }
    }
    return result;
}

HOOK(0x006BA560, GfxObjectProps_AssignPropertyRecord, "auto");
