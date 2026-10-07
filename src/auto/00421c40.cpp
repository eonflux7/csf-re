#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int* __fastcall NotifyLinkedObjectAndSetFlag(void* self, void* /*edx*/)
{
    void* linkedObject = CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x14);

    reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int, int)>(
        CSF_AT(void**, linkedObject, 0x00)[0x8C / 4])(
            linkedObject, nullptr, 11, 0, -1, 0);

    linkedObject = CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x14);
    int* flags = &CSF_AT(int, linkedObject, 0x1F0);
    *flags |= 0x10;
    return flags;
}

HOOK(0x00421C40, NotifyLinkedObjectAndSetFlag, "auto");
