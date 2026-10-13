#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, NetPlayerDDL_ResolveObject, 0x00655DF0, (void* self, void* edx, bool* a2));
CSF_FUNC(void*, __fastcall, ReadSerializedByte, 0x00655E20, (void* self, void* edx, char* a2));
CSF_FUNC(uint32_t*, __fastcall, NetCogibleDdl_ReadIntoSlot, 0x00655F00, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, NetStream_ReadIntoSlot, 0x00655FC0, (void* self, void* edx, char* a2));
CSF_FUNC(uint8_t, __fastcall, C_00858180_SetSettings, 0x0073F870, (C_00858180* self, void* , char a2, int arg4, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15, int a16, int a17, char a18, int a19, int a3));

static int __fastcall C_00858180_ReadSettingsFromStream(C_00858180* self, void* /*edx*/, C_008622e8* stream)
{
    alignas(8) uint8_t tailBytes[8];
    char byteVal[4];
    int v[15];
    bool flag;

    ReadSerializedByte(stream, nullptr, byteVal);
    for (int i = 0; i < 15; ++i)
        NetCogibleDdl_ReadIntoSlot(stream, nullptr, reinterpret_cast<int>(&v[i]));
    NetPlayerDDL_ResolveObject(stream, nullptr, &flag);
    NetStream_ReadIntoSlot(stream, nullptr, reinterpret_cast<char*>(tailBytes));

    return C_00858180_SetSettings(self, nullptr, byteVal[0],
                                  v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7],
                                  v[8], v[9], v[10], v[11], v[12], v[13], v[14],
                                  (char)flag,
                                  CSF_AT(int, tailBytes, 0),
                                  CSF_AT(int, tailBytes, 4));
}

HOOK(0x00747700, C_00858180_ReadSettingsFromStream, "auto");
