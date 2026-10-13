#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, RpTeam_FlushPendingSceneEntry, 0x006FF910, (void* self, void*));

static char __fastcall C_00861120_FlushPendingEntriesAndSetFlag(C_00861120* self, void* /*edx*/, char flag)
{
    if (self->field_00da == 0)
    {
        self->field_00da = static_cast<uint8_t>(flag);
        return flag;
    }

    uint32_t count = self->field_005c;
    if (count == 0)
    {
        self->field_00da = static_cast<uint8_t>(flag);
        return static_cast<char>(count);
    }

    uint32_t index = 0;
    do
    {
        uint32_t entry = reinterpret_cast<uint32_t*>(self->field_0054)[index];
        RpTeam_FlushPendingSceneEntry(reinterpret_cast<void*>(entry), nullptr);
        count = self->field_005c;
        ++index;
    } while (index < count);

    self->field_00da = static_cast<uint8_t>(flag);
    return static_cast<char>(count);
}

HOOK(0x006D6700, C_00861120_FlushPendingEntriesAndSetFlag, "auto");
