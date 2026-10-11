#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, sub_655DF0, 0x00655DF0, (void* self, void* edx, bool* a2));
CSF_FUNC(void*, __fastcall, sub_655E20, 0x00655E20, (void* self, void* edx, char* a2));
CSF_FUNC(uint32_t*, __fastcall, sub_655F00, 0x00655F00, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, sub_655FC0, 0x00655FC0, (void* self, void* edx, char* a2));
CSF_FUNC(bool, __fastcall, sub_740190, 0x00740190, (void* self, void* edx, char a2, int arg4, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15, int a16, int a17, char a18, int a19, int a3));

static bool __fastcall NetCogibleDDL_ReadMessageAndDispatch(C_00858180* self, void* /*edx*/, C_008622e8* stream)
{
    alignas(8) uint8_t tailBytes[8];
    char byteVal[4];
    int v[15];
    bool flag;

    sub_655E20(stream, nullptr, byteVal);
    for (int i = 0; i < 15; ++i)
        sub_655F00(stream, nullptr, reinterpret_cast<int>(&v[i]));
    sub_655DF0(stream, nullptr, &flag);
    sub_655FC0(stream, nullptr, reinterpret_cast<char*>(tailBytes));

    return sub_740190(self, nullptr, byteVal[0],
                      v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7],
                      v[8], v[9], v[10], v[11], v[12], v[13], v[14],
                      (char)flag,
                      CSF_AT(int, tailBytes, 0),
                      CSF_AT(int, tailBytes, 4));
}

HOOK(0x00742540, NetCogibleDDL_ReadMessageAndDispatch, "auto");
