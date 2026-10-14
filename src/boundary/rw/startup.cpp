// Startup-only ABI bridges, over the unmodified MIT librw backend.
#ifdef CSF_STANDALONE
#if defined(_WIN32) && defined(RW_D3D9)
#include <windows.h>
#include <d3d9.h>
#endif
#include "globals.h"
#ifdef CSF_RW_NATIVE_TEST
#define HOOK(addr, fn, module) template <> void* const ::csf::fn_slot<(addr)>::p = (void*)&fn
#else
#include "hook.h"
#endif
#include "device.h"
#include "startup.h"
#ifdef RW_D3D9
#include "../../../work/tools/librw/src/d3d/rwd3dimpl.h"
#endif
#include <cstring>
#include <cmath>

namespace {
// Original device request 6 writes SIX words. +10 is refresh, +14 RW format;
// 006dc1f0 consumes +10. Native VideoMode contains only the first four.
using csf_rw::VideoMode;
VideoMode* __cdecl modeInfo(VideoMode* out, int index) {
    csf_rw::startupTrace("0x00626BE0","GetVideoModeInfo");
    if (!out || !rw::engine || index < 0 || index >= rw::Engine::getNumVideoModes()) return nullptr;
    rw::VideoMode n{};
    if (!rw::Engine::getVideoModeInfo(&n,index)) return nullptr;
    VideoMode result{n.width,n.height,n.depth,n.flags,0,0};
#ifdef RW_D3D9
    auto& mode = rw::d3d::d3d9Globals.modes[index].mode;
    result.refresh = mode.RefreshRate;
    switch (mode.Format) {
    case D3DFMT_A8R8G8B8: result.format = rw::Raster::C8888; break;
    case D3DFMT_X8R8G8B8: result.format = rw::Raster::C888; break;
    case D3DFMT_R5G6B5: result.format = rw::Raster::C565; break;
    case D3DFMT_X1R5G5B5: result.format = rw::Raster::C555; break;
    case D3DFMT_A1R5G5B5: result.format = rw::Raster::C1555; break;
    default: break;
    }
#endif
    *out = result; return out;
}
// 006dc250 requests zero (default refresh). Upstream Start always chooses
// D3DPRESENT_RATE_DEFAULT: nonzero requests are explicitly unsupported.
int __cdecl refreshRate(int rate) {
    csf_rw::startupTrace("0x0062B200","SetRefreshRate (only default supported)");
    if (rw::Engine::state != rw::Engine::Opened || rate != 0) return 0;
    return 0; // original zero-input result is zero, caller ignores it
}
// 006dc250 selects pre-start MSAA. Clamp against the SELECTED mode, not
// librw's uninitialized startMode (its max-level query uses that too early).
unsigned __cdecl multiSampling(unsigned requested) {
    csf_rw::startupTrace("0x0062B170","SetMultiSamplingLevels");
    if (rw::Engine::state != rw::Engine::Opened) return 0;
    unsigned level = requested < 2 ? 1 : requested > 16 ? 16 : requested;
#ifdef RW_D3D9
    auto& g = rw::d3d::d3d9Globals;
    auto& mode = g.modes[g.currentMode];
    BOOL windowed=!(mode.flags & rw::VIDEOMODEEXCLUSIVE);
    D3DFORMAT format=mode.mode.Format;
    // Match unmodified native Start's alpha upgrade and depth selection.
    if (format==D3DFMT_X8R8G8B8 && SUCCEEDED(g.d3d9->CheckDeviceType(g.adapter,
        D3DDEVTYPE_HAL,format,D3DFMT_A8R8G8B8,windowed))) format=D3DFMT_A8R8G8B8;
    rw::VideoMode info{};rw::Engine::getVideoModeInfo(&info,g.currentMode);
    D3DFORMAT depth=info.depth==32 ? D3DFMT_D24S8 : D3DFMT_D16;
    while (level > 1 && (FAILED(g.d3d9->CheckDeviceMultiSampleType(g.adapter,
        D3DDEVTYPE_HAL,format,windowed,static_cast<D3DMULTISAMPLE_TYPE>(level),nullptr)) ||
        FAILED(g.d3d9->CheckDeviceMultiSampleType(g.adapter,D3DDEVTYPE_HAL,depth,
        windowed,static_cast<D3DMULTISAMPLE_TYPE>(level),nullptr)))) --level;
#else
    level = 1; // no device, no MSAA support
#endif
    return rw::Engine::setMultiSamplingLevels(level) ? level : 0;
}
// 006dc250 uses pre-start TRUE to disable MSAA for dialog compatibility.
// Native librw has no fullscreen dialog compositor policy: only windowed
// startup is supported here. Fullscreen TRUE fails rather than claiming it.
int __cdecl dialogMode(int enabled) {
    csf_rw::startupTrace("0x0062B2A0","SetFullScreenDialogBoxMode (windowed only)");
    if (rw::Engine::state != rw::Engine::Opened || (enabled != 0 && enabled != 1)) return 0;
    rw::VideoMode mode{};
    if (!rw::Engine::getVideoModeInfo(&mode,rw::Engine::getCurrentVideoMode())) return 0;
    if (enabled && (mode.flags & rw::VIDEOMODEEXCLUSIVE)) return 0;
    return !enabled || rw::Engine::setMultiSamplingLevels(1);
}
// 006dc750 consumes byte budget. Real device's approximate available texture
// memory is the policy, not a fabricated size for the NULL platform.
int __stdcall textureMemory() {
    return csf_rw::deviceReady() ? static_cast<int>(csf_rw::deviceTextureMemory()) : -1;
}
// 006dbdb0 clears the current CAMERA context, before any Camera exists.
int __cdecl rasterClear(uint32_t pixel) { csf_rw::startupTrace("0x006274B0","RasterClear"); return csf_rw::clearRasterContext(pixel); }
// Native renderFast only copies CAMERA -> CAMERATEXTURE. Clock startup instead
// composites NORMAL -> CAMERA; the bridge draws a librw Im2D textured quad.
csf_rw::Raster* __cdecl rasterFast(csf_rw::Raster* src,int x,int y) {
    csf_rw::startupTrace("0x00627510","RasterRenderFast");
    if (!src) return nullptr;
    rw::Rect rect{x,y,src->width,src->height};
    return csf_rw::drawRasterContext(src,rect) ? src : nullptr;
}
csf_rw::Raster* __cdecl rasterScaled(csf_rw::Raster* src,const rw::Rect* rect) {
    csf_rw::startupTrace("0x00627470","RasterRenderScaled");
    return rect && csf_rw::drawRasterContext(src,*rect) ? src : nullptr;
}
// First frame supplies window=0, flags=0; use the HWND copied at EngineOpen.
csf_rw::Raster* __cdecl rasterShow(csf_rw::Raster* raster,void* window,unsigned flags) {
    csf_rw::startupTrace("0x006276E0","ShowRaster");
    return csf_rw::showRaster(raster,window,flags) ? raster : nullptr;
}
// 00704c30 bounds query: inclusive native BBox/V3d POD coordinates.
int __cdecl bboxContains(const rw::BBox* b,const rw::V3d* p) {
    return b && p && p->x >= b->inf.x && p->y >= b->inf.y && p->z >= b->inf.z &&
        p->x <= b->sup.x && p->y <= b->sup.y && p->z <= b->sup.z;
}
}
HOOK(0x00626BE0,modeInfo,"boundary-rw");
HOOK(0x0062B200,refreshRate,"boundary-rw");
HOOK(0x0062B170,multiSampling,"boundary-rw");
HOOK(0x0062B2A0,dialogMode,"boundary-rw");
HOOK(0x00626C90,textureMemory,"boundary-rw");
HOOK(0x006274B0,rasterClear,"boundary-rw");
HOOK(0x00627510,rasterFast,"boundary-rw");
HOOK(0x00627470,rasterScaled,"boundary-rw");
HOOK(0x006276E0,rasterShow,"boundary-rw");
HOOK(0x0063CA30,bboxContains,"boundary-rw");
#endif
