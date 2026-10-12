#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char, __stdcall, NetPlayerDDL_ProcessEmbeddedField, 0x00753410, (int a1));
CSF_FUNC(char, __stdcall, NetGameDDL_ProcessBaseField, 0x0065E740, (int a1, int a2));

static char __fastcall NetGameDDL_ProcessField(
    char* self, void* /*edx*/, char* field, int operation)
{
    static constexpr int embeddedFieldOffsets[] = {
        68, 80, 92, 95, 100, 696, 708,
        714, 720, 726, 732, 744, 768
    };

    for (int offset : embeddedFieldOffsets)
    {
        if (!field)
            NetPlayerDDL_ProcessEmbeddedField(operation);

        if (field == self + offset)
            return NetPlayerDDL_ProcessEmbeddedField(operation);
    }

    return NetGameDDL_ProcessBaseField(
        reinterpret_cast<int>(field), operation);
}

HOOK(0x0074C560, NetGameDDL_ProcessField, "auto");
