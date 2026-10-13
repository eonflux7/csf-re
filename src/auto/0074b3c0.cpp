#include "globals.h"
#include "hook.h"

CSF_FUNC(char, __stdcall, NetGameDDL_ProcessBaseField, 0x0065E740, (int a1, int a2));
CSF_FUNC(char, __stdcall, NetPlayerDDL_ProcessEmbeddedField, 0x00753410, (int a1));

static int __fastcall NetGhostDDL_ProcessField(
    char* self, void* /*edx*/, char* field, int operation)
{
    if (!field)
        NetPlayerDDL_ProcessEmbeddedField(operation);
    if (field == self + 0x44)
        return NetPlayerDDL_ProcessEmbeddedField(operation);

    if (!field)
        NetPlayerDDL_ProcessEmbeddedField(operation);
    if (field == self + 0x47)
        return NetPlayerDDL_ProcessEmbeddedField(operation);

    if (!field)
        NetPlayerDDL_ProcessEmbeddedField(operation);
    if (field == self + 0x4A)
        return NetPlayerDDL_ProcessEmbeddedField(operation);

    if (!field)
        NetPlayerDDL_ProcessEmbeddedField(operation);
    if (field == self + 0x50)
        return NetPlayerDDL_ProcessEmbeddedField(operation);

    if (!field)
        NetPlayerDDL_ProcessEmbeddedField(operation);
    if (field == self + 0x5C)
        return NetPlayerDDL_ProcessEmbeddedField(operation);

    if (!field)
        NetPlayerDDL_ProcessEmbeddedField(operation);
    if (field == self + 0x68)
        return NetPlayerDDL_ProcessEmbeddedField(operation);

    return NetGameDDL_ProcessBaseField(
        reinterpret_cast<int>(field), operation);
}

HOOK(0x0074B3C0, NetGhostDDL_ProcessField, "auto");
