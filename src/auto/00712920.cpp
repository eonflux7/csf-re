#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Audio_CreatePlaybackObject, 0x007DF2B0,
         (int* a1, char* a2, int a3,
          int (__cdecl *a4)(int, int, char*, int), int a5));
CSF_FUNC(int, __cdecl, Audio_ConfigurePlaybackObject, 0x007DF740,
         (int a1, int a2));
CSF_FUNC(uint32_t*, __cdecl, Audio_SetParameter, 0x007E1380,
         (uint32_t* a1, int a2, int a3, ...));

CSF_GLOBAL(float, g_AudioScalarFactor, 0x008A85DC);
CSF_GLOBAL(float, g_AudioScalarConversion, 0x008A85D8);

// The game's variadic interface consumes one raw float word, not a
// C++ variadic double.
static void Audio_SetFloatParameter(uint32_t* object, int parameter, float value)
{
    uint32_t word;
    std::memcpy(&word, &value, sizeof(word));
    Audio_SetParameter(object, parameter, 1, word);
}

static float* __fastcall C_00863dd0_Initialize(
    C_00863dd0* self, void* /*edx*/)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00863DD0);

    const int playbackObject =
        Audio_CreatePlaybackObject(nullptr, nullptr, 0, nullptr, 0);
    self->field_0004 = static_cast<uint32_t>(playbackObject);
    self->field_0008 = 1.0f;
    self->field_000c = 0;
    Audio_ConfigurePlaybackObject(playbackObject, 0);

    CSF_AT(float, reinterpret_cast<void*>(self->field_0004), 0x70) = 200.0f;
    uint32_t* parameterObject =
        CSF_AT(uint32_t*, reinterpret_cast<void*>(self->field_0004), 0x30);
    if (parameterObject)
        Audio_SetFloatParameter(parameterObject, 2, 200.0f);

    CSF_AT(float, reinterpret_cast<void*>(self->field_0004), 0x74) = 8000.0f;
    parameterObject =
        CSF_AT(uint32_t*, reinterpret_cast<void*>(self->field_0004), 0x30);
    if (parameterObject)
        Audio_SetFloatParameter(parameterObject, 3, 8000.0f);

    CSF_AT(float, reinterpret_cast<void*>(self->field_0004), 0x40) =
        g_AudioScalarFactor * self->field_0008 * g_AudioScalarConversion;
    parameterObject =
        CSF_AT(uint32_t*, reinterpret_cast<void*>(self->field_0004), 0x30);
    if (parameterObject)
        Audio_SetFloatParameter(
            parameterObject, 5,
            g_AudioScalarFactor * self->field_0008 * g_AudioScalarConversion);

    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 0x88) = 0;
    parameterObject =
        CSF_AT(uint32_t*, reinterpret_cast<void*>(self->field_0004), 0x30);
    if (parameterObject)
        Audio_SetParameter(parameterObject, 11, 1, 0);

    return reinterpret_cast<float*>(self);
}

HOOK(0x00712920, C_00863dd0_Initialize, "auto");
