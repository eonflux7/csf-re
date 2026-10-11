#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// DestroyPointerStorage: releases the pointer storage embedded at the given address.
CSF_FUNC(int, __fastcall, DestroyPointerStorage, 0x00558790, (int* self, void* edx));

static int __fastcall C_00832f18_ReleasePointerStorages(C_00832f18* self, void* /*edx*/)
{
    DestroyPointerStorage(reinterpret_cast<int*>(reinterpret_cast<uint8_t*>(self) + 0x08), nullptr);
    DestroyPointerStorage(reinterpret_cast<int*>(reinterpret_cast<uint8_t*>(self) + 0x18), nullptr);
    int result = DestroyPointerStorage(reinterpret_cast<int*>(reinterpret_cast<uint8_t*>(self) + 0x30), nullptr);
    self->field_002c = 0;
    return result;
}

HOOK(0x0047E660, C_00832f18_ReleasePointerStorages, "auto");
