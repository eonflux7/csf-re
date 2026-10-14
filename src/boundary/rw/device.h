#pragma once
#include "image.h"
#include <cstdint>

namespace csf_rw {
// Private device seam. bindD3D9Device takes a BORROWED librw-owned COM device;
// call it after native Start/reset and unbind before native Stop. No COM facade
// may replace it: the game calls the returned device's vtable directly.
struct DeviceBackend {
    virtual ~DeviceBackend() = default;
    virtual void* raw() = 0;
    virtual bool caps(void* out) = 0; // D3DCAPS9, 304 bytes
    virtual int declaration(void*) = 0;
    virtual int vertexShader(void*) = 0;
    virtual int pixelShader(void*) = 0;
    virtual int indices(void*) = 0;
    virtual int stream(unsigned, void*, unsigned, unsigned) = 0;
    virtual int draw(unsigned, int, unsigned, unsigned, unsigned, unsigned) = 0;
    virtual int transform(unsigned, const float*) = 0;
    virtual int createShader(bool pixel, const uint32_t*, void**) = 0;
    virtual int constants(bool pixel, unsigned, const float*, unsigned) = 0;
    virtual int renderState(unsigned, unsigned) = 0;
    // Startup raster operations have no usable native renderFast equivalent.
    // Defaults fail: a NULL/recording fixture must explicitly opt in.
    virtual bool clearRaster(rw::Raster*, uint32_t) { return false; }
    virtual bool beginRasterDraw(rw::Raster*, rw::Raster*, const rw::Rect&) { return false; }
    virtual bool endRasterDraw() { return false; }
    virtual bool presentRaster(rw::Raster*, void*, uint32_t) { return false; }
    virtual uint32_t textureMemory() { return 0; }
};
void bindDeviceBackend(DeviceBackend*); // borrowed; native recording fixture
bool bindD3D9Device(void*);
bool deviceReady();
bool clearRasterContext(uint32_t);
bool drawRasterContext(Raster*, const rw::Rect&);
bool showRaster(Raster*, void*, uint32_t);
uint32_t deviceTextureMemory();
void invalidateDeviceBoundary(); // reset/rebind clears snapshots and Im3D batch
void resetImmediateBoundary();

// Engine-global dispatch entries are NOT fn_slot references. Lifecycle must
// explicitly install these thunks after Open; this helper never replaces engine
// ownership or patches upstream. The slice-2 NULL lifecycle is unchanged.
bool installImmediateDispatch(void* engineFacade);
int setGameRenderState(int, void*);
int getGameRenderState(int, void*);

// Slice 4 must supply original-layout resource-entry data, not a native
// InstanceDataHeader. Callback receives (resource entry, atomic, type, flags).
// No Rx packet, graph executor, or node plugin callbacks are emulated here.
using PipelineCallback = void (__cdecl *)(void*, void*, uint8_t, uint32_t);
struct Pipeline;
bool dispatchPipeline(Pipeline*, void* resourceEntry, void* atomic,
                      uint8_t type, uint32_t flags);
bool pipelinesEmpty();
}
