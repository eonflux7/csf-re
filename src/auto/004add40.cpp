#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, AudioSoundManager_ProcessArchiveContext, 0x00405450, (void* context));

// Record `a1` is accessed through raw offsets: +0 flag byte, +4 sub-object pointer, +0x1C saved field.
// The sub-object's vtable slot 32 (+0x80) is a boolean query taking (0, record) as stack args;
// slot 31 (+0x7C) takes only the record.
static int __stdcall Audio_UpdateAndReportDone(void* a1)
{
    // Saved before the context call; restored after it.
    uint32_t savedField1C = CSF_AT(uint32_t, a1, 0x1C);

    // Flag byte at +0: when clear, ask the sub-object (slot 32) whether to continue.
    bool v2 = false;
    if (CSF_AT(uint8_t, a1, 0) == 0) {
        void* subObject = CSF_AT(void*, a1, 4);
        bool queryResult = ((bool(__fastcall*)(void*, void*, int, void*))
            CSF_AT(void**, subObject, 0)[0x80 / 4])(subObject, nullptr, 0, a1);
        v2 = !queryResult;
    }
    CSF_AT(uint8_t, a1, 0) = v2 ? 1 : 0;

    AudioSoundManager_ProcessArchiveContext(a1);

    CSF_AT(uint32_t, a1, 0x1C) = savedField1C;

    if (CSF_AT(uint8_t, a1, 0) == 0) {
        void* subObject = CSF_AT(void*, a1, 4);
        bool finished = ((bool(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, subObject, 0)[0x7C / 4])(subObject, nullptr, a1);
        if (finished) {
            CSF_AT(uint8_t, a1, 0) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, a1, 0) = 1;
    return 1;
}

HOOK(0x4ADD40, Audio_UpdateAndReportDone, "auto");
