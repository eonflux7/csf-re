#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(char, __stdcall, NetPlayerDDL_ProcessEmbeddedField, 0x00753410, (int a1));
CSF_FUNC(char, __fastcall, NetPlayerDDL_ProcessBaseField, 0x007A31A0,
         (char* self, void* edx, char* field, int operation));

static char __fastcall NetPlayerDDL_DispatchFieldOperation(
    char* self, void* /*edx*/, char* field, int operation)
{
    static constexpr int fieldOffsets[] = {
        1088, 1091, 1094, 1100, 1103, 1106, 1112,
        1640, 1652, 1664, 1670, 1676, 1682, 1688,
        1694, 1700, 1704
    };

    for (int offset : fieldOffsets)
    {
        if (!field)
            NetPlayerDDL_ProcessEmbeddedField(operation);

        if (field == self + offset)
            return NetPlayerDDL_ProcessEmbeddedField(operation);
    }

    return NetPlayerDDL_ProcessBaseField(self, nullptr, field, operation);
}

HOOK(0x00744650, NetPlayerDDL_DispatchFieldOperation, "auto");
