#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

static int __fastcall CMenuSoundOptions_ApplyAcousticSelection(
    CMenuSoundOptions* self, void* /*edx*/)
{
    std::uint32_t objectAddress = self->field_002c;
    if (objectAddress == 0)
        return 0;

    objectAddress = self->field_0020;
    if (objectAddress == 0)
        return 0;

    void* selector = reinterpret_cast<void*>(objectAddress);
    const int selection = ((int(__fastcall*)(void*, void*))
        CSF_AT(void**, selector, 0)[0x2AC / 4])(selector, nullptr);

    if (static_cast<std::uint32_t>(selection) > 3)
        return selection;

    void* settings = reinterpret_cast<void*>(self->field_002c);
    return ((int(__fastcall*)(void*, void*, int))
        CSF_AT(void**, settings, 0)[0x2C / 4])(
            settings, nullptr, selection + 1);
}

HOOK(0x0053CB10, CMenuSoundOptions_ApplyAcousticSelection, "auto");
