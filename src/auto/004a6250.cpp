#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Cmd_MapGroupA, 0x004A5F90, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Cmd_MapGroupB, 0x004A5FD0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Cmd_MapGroupC, 0x004A6020, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Cmd_MapCase8, 0x004A6040, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Cmd_MapCase12, 0x004A6080, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Cmd_MapCase13, 0x004A60A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Cmd_MapCase15, 0x004A60C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Cmd_MapCase16, 0x004A60E0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Cmd_MapCase43, 0x004A6100, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Cmd_MapCase44, 0x004A6120, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Cmd_MapCase46, 0x004A6140, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Cmd_MapCase10, 0x004A6170, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Cmd_MapCase48, 0x004A6190, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Cmd_MapCase49, 0x004A61B0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Cmd_MapCase50, 0x004A61D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Cmd_MapCase53, 0x004A61F0, (void* self, void* edx));

static int __fastcall C_00834640_MapActionToIndex(C_00834640* self, void* /*edx*/, int action)
{
    int result = -1;
    switch (action) {
    case 0:
    case 17:
    case 18:
    case 19:
        result = Cmd_MapGroupA(self, nullptr);
        break;
    case 1:
    case 2:
    case 5:
    case 6:
        result = Cmd_MapGroupB(self, nullptr);
        break;
    case 3:
    case 4:
        result = Cmd_MapGroupC(self, nullptr);
        break;
    case 7:
        result = 6;
        break;
    case 8:
        result = Cmd_MapCase8(self, nullptr);
        break;
    case 9:
        result = 5;
        break;
    case 10:
        result = Cmd_MapCase10(self, nullptr);
        break;
    case 12:
        result = Cmd_MapCase12(self, nullptr);
        break;
    case 13:
        result = Cmd_MapCase13(self, nullptr);
        break;
    case 14:
        result = 11;
        break;
    case 15:
        result = Cmd_MapCase15(self, nullptr);
        break;
    case 16:
        result = Cmd_MapCase16(self, nullptr);
        break;
    case 20:
        result = 18;
        break;
    case 21:
        result = 19;
        break;
    case 22:
        result = 20;
        break;
    case 28:
        result = 22;
        break;
    case 29:
        result = 23;
        break;
    case 30:
        result = 24;
        break;
    case 35:
        result = 25;
        break;
    case 40:
        result = 26;
        break;
    case 43:
        result = Cmd_MapCase43(self, nullptr);
        break;
    case 44:
        result = Cmd_MapCase44(self, nullptr);
        break;
    case 45:
        result = 4;
        break;
    case 46:
        result = Cmd_MapCase46(self, nullptr);
        break;
    case 48:
        result = Cmd_MapCase48(self, nullptr);
        break;
    case 49:
        result = Cmd_MapCase49(self, nullptr);
        break;
    case 50:
        result = Cmd_MapCase50(self, nullptr);
        break;
    case 51:
        result = 34;
        break;
    case 53:
        result = Cmd_MapCase53(self, nullptr);
        break;
    case 54:
        result = 3;
        break;
    case 55:
        result = 21;
        break;
    default:
        break;
    }
    return result;
}

HOOK(0x004A6250, C_00834640_MapActionToIndex, "auto");
