#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char*, __fastcall, NetCogibleDdl_ProcessSource, 0x00749AA0, (void* self, void* edx, char* Source));
CSF_FUNC(char, __fastcall, SetFlag20C, 0x00751810, (void* self, void*));

static int __fastcall NetCogibleDdl_ProcessSourceAndSetFlag(void* self, void* /*edx*/, char* Source)
{
    void* subobject = reinterpret_cast<uint8_t*>(self) + 0x458;
    NetCogibleDdl_ProcessSource(subobject, nullptr, Source);
    return SetFlag20C(subobject, nullptr);
}

HOOK(0x00740F10, NetCogibleDdl_ProcessSourceAndSetFlag, "auto");
