#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"
#include "globals.h"

// Copies three toggle flags (invert axis, subtitles, voice output) from the
// options source object stored at +0x24 into the local cache bytes at
// +0x14, +0x15 and +0x16 via its virtual slots 6, 8 and 10.
static void __fastcall CMenuGameOptions_SyncOptionFlagsFromSource(CMenuGameOptions* self, void* /*edx*/)
{
    if (!self->field_0024)
        return;

    void* source = reinterpret_cast<void*>(self->field_0024);
    uint8_t invertAxis = ((uint8_t(__fastcall*)(void*, void*))CSF_AT(void**, source, 0)[6])(source, nullptr);

    source = reinterpret_cast<void*>(self->field_0024);
    self->field_0014 = invertAxis;
    uint8_t subtitles = ((uint8_t(__fastcall*)(void*, void*))CSF_AT(void**, source, 0)[8])(source, nullptr);

    source = reinterpret_cast<void*>(self->field_0024);
    self->field_0015 = subtitles;
    uint8_t voiceOutput = ((uint8_t(__fastcall*)(void*, void*))CSF_AT(void**, source, 0)[10])(source, nullptr);

    self->field_0016 = voiceOutput;
}

HOOK(0x53B510, CMenuGameOptions_SyncOptionFlagsFromSource, "auto");
