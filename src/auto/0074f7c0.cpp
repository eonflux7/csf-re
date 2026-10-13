#include "globals.h"
#include "hook.h"

CSF_FUNC(char, __stdcall, NetGameDDL_ProcessBaseField, 0x0065E740, (int a1, int a2));
CSF_FUNC(char, __stdcall, NetPlayerDDL_ProcessEmbeddedField, 0x00753410, (int a1));

static int __fastcall NetPostmanDDL_ProcessField(
    char* self, void* /*edx*/, char* field, int operation)
{
    static constexpr int embeddedOffsets[] = {
        68, 80, 92, 108, 120, 136, 160
    };

    for (int offset : embeddedOffsets)
    {
        if (!field)
            NetPlayerDDL_ProcessEmbeddedField(operation);

        if (field == self + offset)
            return NetPlayerDDL_ProcessEmbeddedField(operation);
    }

    return NetGameDDL_ProcessBaseField(
        reinterpret_cast<int>(field), operation);
}

HOOK(0x0074F7C0, NetPostmanDDL_ProcessField, "auto");
