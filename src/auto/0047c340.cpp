#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, CharTaskBase_Init_4DB3C0, 0x004DB3C0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static char* __fastcall TFunctionRecargar_DestroyAndMaybeFree(TFunctionRecargar* self, void* /*edx*/, char freeFlag)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    CharTaskBase_Init_4DB3C0(base + 0x78, nullptr);
    uint8_t* sub28 = base + 0x28;
    CharTaskBase_Init_4DB3C0(sub28 + 0x3C, nullptr);
    CharTaskBase_Init_4DB3C0(sub28 + 0x28, nullptr);
    CharTaskBase_Init_4DB3C0(sub28 + 0x14, nullptr);
    CharTaskBase_Init_4DB3C0(sub28, nullptr);
    CharTaskBase_Init_4DB3C0(base + 0x14, nullptr);
    CharTaskBase_Init_4DB3C0(base, nullptr);

    if ((freeFlag & 1) != 0)
        Mem_Free(self);

    return reinterpret_cast<char*>(self);
}

HOOK(0x0047C340, TFunctionRecargar_DestroyAndMaybeFree, "auto");
