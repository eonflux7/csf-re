#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, RpAtomicRegisterPlugin, 0x0058F210, (int a1, int a2, int a3, int a4, int a5));
CSF_FUNC(int, __cdecl, RpAtomicRegisterPluginStream, 0x0058F270, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RpGeometryRegisterPlugin, 0x00590400, (int a1, int a2, int a3, int a4, int a5));
CSF_FUNC(int, __cdecl, RpGeometryRegisterPluginStream, 0x00590430, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RpMaterialRegisterPlugin, 0x00591590, (int a1, int a2, int a3, int a4, int a5));
CSF_FUNC(int, __cdecl, RpClumpRegisterPluginStream, 0x005915C0, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RpLightRegisterPlugin, 0x0059CEC0, (int a1, int a2, int a3, int a4, int a5));
CSF_FUNC(int, __cdecl, RpLightRegisterPluginStream, 0x0059CEF0, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwFrameRegisterPlugin, 0x00623950, (int a1, int a2, int a3, int a4, int a5));
CSF_FUNC(int, __cdecl, RwEngineRegisterPlugin, 0x00626AE0, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwFrameRegisterPluginStream, 0x006329D0, (int a1, int a2, int a3, int a4));

CSF_GLOBAL(int, g_EnginePropertyPluginOffset, 0x008BB474);
CSF_GLOBAL(int, g_AtomicPropertyPluginOffset, 0x008BB478);
CSF_GLOBAL(void*, g_EntryTable, 0x008BB47C);
CSF_GLOBAL(void*, g_EntryTable_8BB480, 0x008BB480);
CSF_GLOBAL(int, g_LightPropertyPluginOffset, 0x008BB484);
CSF_GLOBAL(int, g_GeometryPropertyPluginOffset, 0x008BB488);

static char __cdecl GfxObjectProps_RegisterPlugins()
{
    g_EnginePropertyPluginOffset = RwEngineRegisterPlugin(
        12, -256,
        (int)CSF_ADDR_FN(0x006BAF80),
        (int)CSF_ADDR_FN(0x006BA4B0));

    const int atomicOffset = RpAtomicRegisterPlugin(
        8, -256,
        (int)CSF_ADDR_FN(0x006B83A0),
        (int)CSF_ADDR_FN(0x006B9FD0),
        (int)CSF_ADDR_FN(0x006BB030));
    if (!atomicOffset)
        return 0;

    const int atomicStreamOffset = RpAtomicRegisterPluginStream(
        -256,
        (int)CSF_ADDR_FN(0x006BB220),
        (int)CSF_ADDR_FN(0x006B9190),
        (int)CSF_ADDR_FN(0x006B8C70));
    if (!atomicStreamOffset || atomicOffset != atomicStreamOffset)
        return 0;

    g_AtomicPropertyPluginOffset = atomicOffset;

    const int geometryOffset = RpGeometryRegisterPlugin(
        8, -256,
        (int)CSF_ADDR_FN(0x006B8450),
        (int)CSF_ADDR_FN(0x006B84A0),
        (int)CSF_ADDR_FN(0x006B8500));
    if (!geometryOffset)
        return 0;

    const int geometryStreamOffset = RpGeometryRegisterPluginStream(
        -256,
        (int)CSF_ADDR_FN(0x006B8D50),
        (int)CSF_ADDR_FN(0x006B8510),
        (int)CSF_ADDR_FN(0x006B8D20));
    if (!geometryStreamOffset || geometryOffset != geometryStreamOffset)
        return 0;

    g_GeometryPropertyPluginOffset = geometryOffset;

    const int materialOffset = RpMaterialRegisterPlugin(
        8, -256,
        (int)CSF_ADDR_FN(0x006B8580),
        (int)CSF_ADDR_FN(0x006BA050),
        (int)CSF_ADDR_FN(0x006BA0C0));
    if (!materialOffset)
        return 0;

    const int materialStreamOffset = RpClumpRegisterPluginStream(
        -256,
        (int)CSF_ADDR_FN(0x006BB640),
        (int)CSF_ADDR_FN(0x006B9400),
        (int)CSF_ADDR_FN(0x006B8DE0));
    if (!materialStreamOffset || materialOffset != materialStreamOffset)
        return 0;

    g_EntryTable = reinterpret_cast<void*>(materialOffset);

    const int frameOffset = RwFrameRegisterPlugin(
        8, -256,
        (int)CSF_ADDR_FN(0x006B85A0),
        (int)CSF_ADDR_FN(0x006BA170),
        (int)CSF_ADDR_FN(0x006BA1B0));
    if (!frameOffset)
        return 0;

    const int frameStreamOffset = RwFrameRegisterPluginStream(
        -256,
        (int)CSF_ADDR_FN(0x006BB7A0),
        (int)CSF_ADDR_FN(0x006B94A0),
        (int)CSF_ADDR_FN(0x006B8E20));
    if (!frameStreamOffset || frameOffset != frameStreamOffset)
        return 0;

    g_EntryTable_8BB480 = reinterpret_cast<void*>(frameOffset);

    const int lightOffset = RpLightRegisterPlugin(
        8, -256,
        (int)CSF_ADDR_FN(0x006B85C0),
        (int)CSF_ADDR_FN(0x006B9060),
        (int)CSF_ADDR_FN(0x006B8600));
    if (!lightOffset)
        return 0;

    const int lightStreamOffset = RpLightRegisterPluginStream(
        -256,
        (int)CSF_ADDR_FN(0x006B8E60),
        (int)CSF_ADDR_FN(0x006B8630),
        (int)CSF_ADDR_FN(0x006B8E50));
    if (!lightStreamOffset || lightOffset != lightStreamOffset)
        return 0;

    g_LightPropertyPluginOffset = lightOffset;
    return 1;
}

HOOK(0x006BBA20, GfxObjectProps_RegisterPlugins, "auto");
