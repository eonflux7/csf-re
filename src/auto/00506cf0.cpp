#include <cstdint>
#include "globals.h"
#include "hook.h"

// No pseudocode export: call order and error strings taken from the original bytes.
CSF_FUNC(uint8_t, __cdecl, GlobalFlag_IsSet, 0x007A7B50, ());
CSF_FUNC(int, __cdecl, AppMain_ReportInitializationError, 0x0069CDF0, (const char* lpCaption, char* Format, ...));
CSF_FUNC(int, __cdecl, RpWorldPluginAttach, 0x0058D0B0, ());
CSF_FUNC(int, __cdecl, RtAnimInitialize, 0x0061ECD0, ());
CSF_FUNC(int, __cdecl, RpHAnimPluginAttach, 0x0056DAE0, ());
CSF_FUNC(int, __cdecl, RpSkinPluginAttach, 0x00587450, ());
CSF_FUNC(int, __cdecl, RpMatFXPluginAttach, 0x00571FF0, ());
CSF_FUNC(int, __cdecl, RpLtMapPluginAttach, 0x0056F3C0, ());
CSF_FUNC(int, __cdecl, RpMipmapKLPluginAttach, 0x00577060, ());
CSF_FUNC(int, __cdecl, RpADCPluginAttach, 0x005663C0, ());
CSF_FUNC(int, __cdecl, RpCollisionPluginAttach, 0x0056C1B0, ());
CSF_FUNC(int, __cdecl, RpPTankPluginAttach, 0x005816E0, ());
CSF_FUNC(int, __cdecl, RpPrtStdPluginAttach, 0x0057C4A0, ());
CSF_FUNC(int, __cdecl, Rt2dPluginAttach, 0x005784A0, ());
CSF_FUNC(int, __cdecl, RtRandomPluginAttach, 0x005869E0, ());
CSF_FUNC(int, __cdecl, RpPatchPluginAttach, 0x005865E0, ());
CSF_FUNC(int, __cdecl, RpUserDataPluginAttach, 0x0058C040, ());
CSF_FUNC(int, __cdecl, RpAnisotPluginAttach, 0x005665A0, ());
CSF_FUNC(int, __cdecl, RpMorphPluginAttach, 0x005776E0, ());
CSF_FUNC(char, __cdecl, GfxObjectProps_RegisterPlugins, 0x006BBA20, ());
CSF_FUNC(int, __cdecl, RtCompressedKeyFrameRegister, 0x006205C0, ());
CSF_FUNC(int, __cdecl, AnimRegisterInterpolationSchemeTable, 0x006C34F0, ());
static int __cdecl AttachEnginePlugins() {
    if (!GlobalFlag_IsSet())
        AppMain_ReportInitializationError(CSF_STR(0x0083F688, "ERROR"), const_cast<char*>(CSF_STR(0x0083F690, "PipeUtils::RegisterPipelines() ha fallado")));
    RpWorldPluginAttach();
    RtAnimInitialize();
    RpHAnimPluginAttach();
    RpSkinPluginAttach();
    RpMatFXPluginAttach();
    RpLtMapPluginAttach();
    RpMipmapKLPluginAttach();
    RpADCPluginAttach();
    RpCollisionPluginAttach();
    RpPTankPluginAttach();
    RpPrtStdPluginAttach();
    Rt2dPluginAttach();
    RtRandomPluginAttach();
    RpPatchPluginAttach();
    RpUserDataPluginAttach();
    RpAnisotPluginAttach();
    RpMorphPluginAttach();
    GfxObjectProps_RegisterPlugins();
    RtCompressedKeyFrameRegister();
    return AnimRegisterInterpolationSchemeTable();
}
HOOK(0x00506CF0, AttachEnginePlugins, "auto");
