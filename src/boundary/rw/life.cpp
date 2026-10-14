// Standalone ABI boundary over the unmodified MIT librw checkout.
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
#include "life.h"
#include "image.h"
#include "object.h"
#include "device.h"
#include "startup.h"
#include "plugins.h"
#include "particles.h"
#include "world.h"
#include "fs.h"
#ifdef RW_D3D9
#include "../../../work/tools/librw/src/d3d/rwd3dimpl.h"
#endif
#include <cstdlib>
#include <cstring>
#include <limits>
#include <new>
#include <unordered_set>

CSF_GLOBAL(void*, gameEngine, 0x008B68C0);

namespace {
csf_rw::MemoryFunctions gameMemory{};
void* windowHandle;
rw::DeviceSystem* startSystem;
#ifdef RW_D3D9
rw::DeviceSystem* openSystem;
int openDeviceSystem(rw::DeviceReq request,void* arg,int n) {
    return request==rw::DEVICEOPEN ? 1 : openSystem(request,arg,n);
}
#endif
bool finalized;
int startDeviceSystem(rw::DeviceReq request,void* arg,int n) {
    // Upstream Engine::start ignores DEVICEINIT/FINALIZE failures. Preflight
    // INIT once before plugin constructors can touch a failed/null device.
    if (request==rw::DEVICEINIT) return 1;
    int result=startSystem(request,arg,n);
    if (request==rw::DEVICEFINALIZE) finalized=result!=0;
    return result;
}
void* __cdecl defaultMalloc(uint32_t n, uint32_t h) { return rw::defaultMemfuncs.rwmalloc(n,h); }
void __cdecl defaultFree(void* p) { rw::defaultMemfuncs.rwfree(p); }
void* __cdecl defaultRealloc(void* p, uint32_t n, uint32_t h) { return rw::defaultMemfuncs.rwrealloc(p,n,h); }
void* __cdecl defaultCalloc(uint32_t count, uint32_t n, uint32_t h) {
    if (n && count > UINT32_MAX/n) return nullptr;
    void* p = defaultMalloc(count*n,h); if (p) std::memset(p,0,count*n); return p;
}
void* nativeMalloc(size_t n, rw::uint32 h) {
    return n <= UINT32_MAX ? gameMemory.malloc(static_cast<uint32_t>(n), h) : nullptr;
}
void* nativeRealloc(void* p, size_t n, rw::uint32 h) {
    return n <= UINT32_MAX ? gameMemory.realloc(p, static_cast<uint32_t>(n), h) : nullptr;
}
// 300-byte original engine prefix, from executable registry metadata; pointers
// widened only for native tests. Never publish rw::engine. Slots 8/9 are the
// direct render-state dispatch seen in 006f61b0; slots 66..69 are memory funcs.
struct EngineFacade { uintptr_t slots[75]; };
static_assert(sizeof(void*) != 4 || sizeof(EngineFacade) == 300);
static_assert(sizeof(void*) != 4 || sizeof(csf_rw::Memory) == 8);
static_assert(sizeof(void*) != 4 || sizeof(csf_rw::MemoryFunctions) == 16);
rw::PluginList registries[] = {
    rw::PluginList(sizeof(EngineFacade)), rw::PluginList(sizeof(csf_rw::Frame)),
    rw::PluginList(sizeof(csf_rw::Atomic)), rw::PluginList(sizeof(csf_rw::Geometry)), rw::PluginList(sizeof(csf_rw::Material)), rw::PluginList(sizeof(csf_rw::Light)),
    rw::PluginList(sizeof(csf_rw::Raster)), rw::PluginList(sizeof(csf_rw::Texture)),
    rw::PluginList(sizeof(csf_rw::TexDictionary)), rw::PluginList(sizeof(csf_rw::Clump)), rw::PluginList(sizeof(csf_rw::Camera)),
    rw::PluginList(sizeof(csf_rw::World)), rw::PluginList(sizeof(csf_rw::Sector))
};
int __cdecl setState(int state, void* value) {
    if (!rw::engine) return 0;
    if (state == rw::TEXTURERASTER) {
        auto* r = csf_rw::rasterNative(static_cast<csf_rw::Raster*>(value));
        if (value && !r) return 0;
        value = r;
    }
    rw::SetRenderStatePtr(state, value); return 1;
}
int __cdecl getState(int state, void* out) {
    if (!rw::engine || !out) return 0;
    void* value = rw::GetRenderStatePtr(state);
    if (state == rw::TEXTURERASTER)
        value = csf_rw::rasterFacade(static_cast<rw::Raster*>(value));
    *static_cast<void**>(out) = value; return 1;
}
// 00506f70 supplies the four-entry game table (006912a0 installs malloc,
// free, realloc, calloc), flags=0, arena bytes. librw's table instead orders
// malloc/realloc/free and adds must-allocate callbacks; adapt, do not cast.
int __cdecl engineInit(const csf_rw::MemoryFunctions* m, char flags, int arena) {
    csf_rw::startupTrace("0x00627360","EngineInit");
    if (rw::Engine::state != rw::Engine::Dead || flags != 0 || arena < 0) return 0;
    if (m) {
        if (!m->malloc || !m->free || !m->realloc || !m->calloc) return 0;
        gameMemory = *m;
        rw::MemoryFunctions native{nativeMalloc, nativeRealloc, m->free, nullptr, nullptr};
        return rw::Engine::init(&native);
    }
    gameMemory = {defaultMalloc, defaultFree, defaultRealloc, defaultCalloc};
    return rw::Engine::init();
}
// 006dc000 passes address of HWND. Copy it before its stack frame returns.
int __cdecl engineOpen(void* params) {
    csf_rw::startupTrace("0x00627290","EngineOpen");
    if (rw::Engine::state != rw::Engine::Initialized) return 0;
    void* window = params ? *static_cast<void**>(params) : nullptr;
#ifdef RW_D3D9
    if (!window) return 0;
    rw::EngineOpenParams nativeParams{static_cast<HWND>(window)};
    auto* nativeOpen = &nativeParams;
#else
    rw::EngineOpenParams* nativeOpen = nullptr;
#endif
    auto* a = std::calloc(1, registries[0].size);
    if (!a) return 0;
#ifdef RW_D3D9
    // Like Start, upstream Open discards the device-system result. Check its
    // real D3D9 Open first, then suppress only the repeated call in Engine.
    openSystem=rw::d3d::renderdevice.system;
    if (!openSystem(rw::DEVICEOPEN,nativeOpen,0)) { std::free(a); return 0; }
    rw::d3d::renderdevice.system=openDeviceSystem;
    bool opened=rw::Engine::open(nativeOpen)!=0;
    rw::d3d::renderdevice.system=openSystem;
    if (rw::engine) rw::engine->device.system=openSystem;
    if (!opened) { openSystem(rw::DEVICECLOSE,nullptr,0); std::free(a); return 0; }
#else
    if (!rw::Engine::open(nativeOpen)) { std::free(a); return 0; }
#endif
    windowHandle = window;
    if (csf_rw::startupTracing()) std::fprintf(stderr,"RW startup HWND=%p (copied)\n",windowHandle);
    auto* facade = static_cast<EngineFacade*>(a);
    facade->slots[8] = reinterpret_cast<uintptr_t>(&setState);
    facade->slots[9] = reinterpret_cast<uintptr_t>(&getState);
    if (gameMemory.malloc) {
        facade->slots[66] = reinterpret_cast<uintptr_t>(gameMemory.malloc);
        facade->slots[67] = reinterpret_cast<uintptr_t>(gameMemory.free);
        facade->slots[68] = reinterpret_cast<uintptr_t>(gameMemory.realloc);
        facade->slots[69] = reinterpret_cast<uintptr_t>(gameMemory.calloc);
    }
    gameEngine = a;
#ifdef RW_D3D9
    csf_rw::installImmediateDispatch(a);
#endif
    return 1;
}
void stopNative() {
    rw::Engine::stop();
    // NULL still constructs inactive PS2/Xbox/D3D/WDGL drivers. Upstream's
    // driverClose callbacks leave their default pipelines allocated; PS2 also
    // caches two global pipelines across engines. Release after module dtors,
    // before Close frees the driver headers. No librw source patch is needed.
    for (int platform : {rw::PLATFORM_PS2, rw::PLATFORM_XBOX,
                         rw::PLATFORM_D3D8, rw::PLATFORM_D3D9, rw::PLATFORM_WDGL}) {
        auto* driver = rw::engine->driver[platform];
        auto* pipe = driver->defaultPipeline;
        if (pipe && pipe != rw::engine->dummyDefaultPipeline) {
            pipe->destroy(); driver->defaultPipeline = rw::engine->dummyDefaultPipeline;
        }
    }
    rw::ps2::defaultObjPipe = nullptr;
    if (rw::ps2::defaultMatPipe) {
        rw::ps2::defaultMatPipe->destroy(); rw::ps2::defaultMatPipe = nullptr;
    }
}
// 006db9a0 checks Start success. Game engine ctor 006baf80 uses the global
// facade, so publish before invoking callbacks; native callbacks stay native.
int __stdcall engineStart() {
    csf_rw::startupTrace("0x00626D60","EngineStart");
    if (rw::Engine::state != rw::Engine::Opened || !gameEngine) return 0;
    startSystem=rw::engine->device.system;
    if (!startSystem(rw::DEVICEINIT,nullptr,0)) {
        csf_rw::startupTrace("0x00626D60","DEVICEINIT failed (engine remains Opened)");
        return 0;
    }
    finalized=false;
    rw::engine->device.system=startDeviceSystem;
    bool started=rw::Engine::start()!=0;
    rw::engine->device.system=startSystem;
    if (!started || !finalized) { if (started) stopNative(); return 0; }
#ifdef RW_D3D9
    // Start (DEVICEINIT) creates the device; only now may the game borrow it.
    if (!csf_rw::bindD3D9Device(rw::d3d::d3ddevice)) { stopNative(); return 0; }
    if (csf_rw::startupTracing()) {
        const auto& p=rw::d3d::d3d9Globals.present;
        std::fprintf(stderr,"RW startup D3D9 device=%p (librw-owned) backbuffer=%ux%u format=%u depthFormat=%u windowed=%d msaa=%u\n",
            static_cast<void*>(rw::d3d::d3ddevice),p.BackBufferWidth,p.BackBufferHeight,
            static_cast<unsigned>(p.BackBufferFormat),static_cast<unsigned>(p.AutoDepthStencilFormat),
            p.Windowed,static_cast<unsigned>(p.MultiSampleType));
    }
#endif
    if (!csf_rw::imageBoundaryStart()) { csf_rw::bindDeviceBackend(nullptr); stopNative(); return 0; }
    if (!csf_rw::constructPlugins(csf_rw::Registry::Engine, gameEngine)) {
        csf_rw::imageBoundaryStop(); csf_rw::bindDeviceBackend(nullptr); stopNative(); return 0;
    }
    return 1;
}
// 006db7c0/006dbd50 tear down explicitly, 0050d1a0 calls Term. librw returns void for these three
// transitions; preserve boolean game results and reject invalid ordering.
int __stdcall engineStop() {
    if (rw::Engine::state != rw::Engine::Started || !csf_rw::framesEmpty() || !csf_rw::imagesEmpty() || !csf_rw::objectsEmpty() || !csf_rw::pipelinesEmpty() || !csf_rw::worldsEmpty() || !csf_rw::particlesEmpty()) return 0;
    csf_rw::destructPlugins(csf_rw::Registry::Engine, gameEngine);
    csf_rw::imageBoundaryStop();
    csf_rw::bindDeviceBackend(nullptr);
    stopNative(); return 1;
}
int __stdcall engineClose();
int __stdcall engineTerm() {
    if (rw::Engine::state != rw::Engine::Initialized) return 0;
    rw::Engine::term(); csf_rw::pluginBoundaryTerm(); csf_rw::particleBoundaryTerm(); gameMemory = {}; return 1;
}
void* descriptor;
decltype(rw::Engine::memfuncs.rwmalloc) savedAllocator;
void* registrationMalloc(size_t n, rw::uint32 hint) {
    if (descriptor && n == sizeof(rw::Plugin)) { void* p = descriptor; descriptor = nullptr; return p; }
    return savedAllocator(n, hint);
}
// 006bba20 stores offsets, later callbacks add them to facade pointers.
// librw's object registries cannot be used: their prefixes and builtins differ.
// A separate librw PluginList supplies registration/storage, with ABI prefixes.
int registerPlugin(csf_rw::Registry kind, int size, uint32_t id,
                   rw::Constructor ctor, rw::Destructor dtor, rw::CopyConstructor copy) {
    if (rw::Engine::state != rw::Engine::Initialized || size < 0) return -1;
    auto& list = csf_rw::plugins(kind);
    if (size > INT32_MAX - list.size - 8 || list.getPluginOffset(id) >= 0) return -1;
    // Upstream registerPlugin dereferences an unchecked allocation. Supply its
    // descriptor via a bounded allocator adapter, avoiding a double-allocation
    // probe/race. Registration invokes no callbacks; boundary is single-threaded.
    descriptor = rw::Engine::memfuncs.rwmalloc(sizeof(rw::Plugin), rw::MEMDUR_GLOBAL);
    if (!descriptor) return -1;
    savedAllocator = rw::Engine::memfuncs.rwmalloc;
    rw::Engine::memfuncs.rwmalloc = registrationMalloc;
    int off = list.registerPlugin(size, id, ctor, dtor, copy);
    rw::Engine::memfuncs.rwmalloc = savedAllocator;
    // Original offsets are 4-byte rounded, not sizeof(native-test-pointer).
    list.size = (off + size + 3) & ~3;
    return off;
}
#define REG_FN(name, kind, ea) \
int __cdecl name(int n, uint32_t id, rw::Constructor c, rw::Destructor d, rw::CopyConstructor cp) { \
    csf_rw::startupTrace(ea,"RegisterPlugin"); \
    return registerPlugin(csf_rw::Registry::kind, n, id, c, d, cp); }
REG_FN(framePlugin, Frame,"0x00623950")
REG_FN(atomicPlugin, Atomic,"0x0058F210")
// Executable registry operands prove the exported names are swapped:
// 590400 -> 89A858 (136-byte world sector), 590BA0 -> 89A870 (96-byte geometry).
REG_FN(sectorPlugin, Sector,"0x00590400")
REG_FN(geometryPlugin, Geometry,"0x00590BA0")
REG_FN(materialPlugin, Material,"0x00591590")
REG_FN(lightPlugin, Light,"0x0059CEC0")
int __cdecl enginePlugin(int n, uint32_t id, rw::Constructor c, rw::Destructor d) {
    csf_rw::startupTrace("0x00626AE0","RegisterPlugin");
    return registerPlugin(csf_rw::Registry::Engine, n, id, c, d, nullptr);
}
// 006dc000 selects subsystem and mode before Start, consuming boolean results.
int __cdecl subSystem(int n) {
    csf_rw::startupTrace("0x00626B30","SetSubSystem");
    return rw::Engine::state == rw::Engine::Opened && rw::Engine::setSubSystem(n);
}
int __cdecl videoMode(int n) {
    csf_rw::startupTrace("0x00626C40","SetVideoMode");
    return rw::Engine::state == rw::Engine::Opened && rw::Engine::setVideoMode(n);
}
int __stdcall numModes() { csf_rw::startupTrace("0x00626B90","GetNumVideoModes"); return rw::engine ? rw::Engine::getNumVideoModes() : 0; }
// 00626AB0/00626AC0 are a plain get/set pair over one RW global (0x8B68C4, 0 at start; the binary has no other
// reference to it), not the current video mode as rwname guessed: 006F8360 saves it, sets 0 and restores it; skin
// entries of type 4 (006C7B20) set it; 22 game call sites read it as a flag (e.g. 0056FAD5: flag ? 5 : 4).
static int engineFlag8B68C4 = 0;
int __cdecl getEngineFlag() { return engineFlag8B68C4; }
void __cdecl setEngineFlag(int value) { engineFlag8B68C4 = value; }
}

namespace csf_rw {
int registerABIPlugin(Registry k,int n,uint32_t id,rw::Constructor c,rw::Destructor d,rw::CopyConstructor cp) {
    return registerPlugin(k,n,id,c,d,cp);
}
void* engineWindow() { return windowHandle; }
rw::PluginList& plugins(Registry k) { return registries[static_cast<unsigned>(k)]; }
bool constructPlugins(Registry k, void* object) {
    auto& list = plugins(k);
    for (rw::LLLink* link = list.plugins.link.next; link != &list.plugins.link; link = link->next) {
        auto* p = reinterpret_cast<rw::Plugin*>(reinterpret_cast<char*>(link) - offsetof(rw::Plugin, inParentList));
        if (!p->constructor(object, p->offset, p->size)) {
            // Roll back completed constructors, excluding the failing one.
            for (rw::LLLink* prev = link->prev; prev != &list.plugins.link; prev = prev->prev) {
                auto* q = reinterpret_cast<rw::Plugin*>(reinterpret_cast<char*>(prev) - offsetof(rw::Plugin, inParentList));
                q->destructor(object, q->offset, q->size);
            }
            return false;
        }
    }
    return true;
}
void destructPlugins(Registry k, void* object) { plugins(k).destruct(object); }
struct Stream { int type, access; rw::Stream* sidecar; bool ownsMemory; };
rw::Stream* streamNative(Stream* s) { return s ? s->sidecar : nullptr; }
}
namespace {
using S = csf_rw::Stream;
std::unordered_set<S*> streams;
bool valid(S* s) { return streams.find(s) != streams.end(); }
// All compiled game Open callers use filename type=2, access=1/2. Memory=3
// additionally supports native fixtures and future serialization. File handles
// and custom callbacks are NOT filenames; unsupported types fail explicitly.
S* __cdecl streamOpen(int type, int access, void* data) {
    if (!rw::engine || (access != 1 && access != 2 && access != 3) || (type != 2 && type != 3)) return nullptr;
    auto* s = new(std::nothrow) S{type, access, nullptr, false};
    if (!s) return nullptr;
    if (type == 2) {
        auto* f = csf_rw::openFileStream(static_cast<char*>(data), access == 1 ? "rb" : access == 2 ? "wb" : "ab");
        if (!f) { delete s; return nullptr; }
        s->sidecar = f;
    } else {
        if (!data) { delete s; return nullptr; }
        auto* m = static_cast<csf_rw::Memory*>(data);
        if (access != 1) {
            // Write streams own a growable buffer and transfer it on Close.
            // Upstream StreamMemory never grows or frees its buffer.
            if (access == 3) { delete s; return nullptr; }
            auto* native = new(std::nothrow) rw::StreamMemory;
            if (!native) { delete s; return nullptr; }
            native->open(nullptr, 0, 0); s->sidecar = native; s->ownsMemory = true;
        } else {
            if (m->length && !m->start) { delete s; return nullptr; }
            auto* native = new(std::nothrow) rw::StreamMemory;
            if (!native) { delete s; return nullptr; }
            native->open(m->start, m->length); s->sidecar = native;
        }
    }
    try { streams.insert(s); }
    catch (...) { s->sidecar->close(); delete s->sidecar; delete s; return nullptr; }
    return s;
}
// 006b8d50 and 0070fc30 consume byte counts (not a handle). Preserve short
// reads. librw marks memory EOF with UINT32_MAX; no subsequent arithmetic on it.
uint32_t __cdecl streamRead(S* s, void* dst, uint32_t n) {
    if (!valid(s) || s->access != 1 || (n && !dst)) return 0;
    if (!n) return 0;
    if (s->type == 3) {
        auto* m = static_cast<rw::StreamMemory*>(s->sidecar);
        if (m->eof()) return 0;
        // Prevent upstream uint32 position+length overflow before its memcpy.
        if (n > m->length - m->position) {
            uint32_t remaining = m->length - m->position;
            uint32_t got = remaining ? m->read8(dst, remaining) : 0;
            m->position = rw::StreamMemory::S_EOF; return got;
        }
    }
    return s->sidecar->read8(dst, n);
}
bool reserve(S* s, uint32_t n) {
    if (s->type != 3) return true;
    auto* m = static_cast<rw::StreamMemory*>(s->sidecar);
    if (m->eof() || n > UINT32_MAX - m->position) return false;
    uint32_t end = m->position + n;
    if (end <= m->capacity) return true;
    void* p = rw::Engine::memfuncs.rwrealloc(m->data, end, rw::MEMDUR_EVENT);
    if (!p) return false;
    m->data = static_cast<rw::uint8*>(p); m->capacity = end; return true;
}
// 006bddc0/006b8510 expect handle/null, not librw's byte count.
S* __cdecl streamWrite(S* s, const void* src, uint32_t n) {
    if (!valid(s) || s->access == 1 || (n && !src) || !reserve(s, n)) return nullptr;
    return !n || s->sidecar->write8(src, n) == n ? s : nullptr;
}
// 006bec60 skips unknown payloads by byte count. Upstream seek is void and
// memory seek can extend the length: reject past-end on read before calling it.
S* __cdecl streamSkip(S* s, uint32_t n) {
    if (!valid(s) || n > INT32_MAX || s->sidecar->eof()) return nullptr;
    if (s->type == 3) {
        auto* m = static_cast<rw::StreamMemory*>(s->sidecar);
        if (s->access == 1 && n > m->length - m->position) return nullptr;
        if (s->access != 1 && !reserve(s, n)) return nullptr;
    }
    uint32_t pos = s->sidecar->tell();
    if (n > UINT32_MAX - pos) return nullptr;
    s->sidecar->seek(static_cast<int32_t>(n));
    return s->sidecar->tell() == pos + n ? s : nullptr;
}
// 006bddc0/006b23e0 close filename streams with null output. Memory writes
// transfer ownership only when an output descriptor is supplied, otherwise free.
int __cdecl streamClose(S* s, csf_rw::Memory* out) {
    if (!valid(s)) return 0;
    if (s->ownsMemory) {
        auto* m = static_cast<rw::StreamMemory*>(s->sidecar);
        if (out) { out->start = m->data; out->length = m->length; }
        else rw::Engine::memfuncs.rwfree(m->data);
    }
    s->sidecar->close(); delete s->sidecar; streams.erase(s); delete s; return 1;
}
int __stdcall engineClose() {
    if (rw::Engine::state != rw::Engine::Opened || !streams.empty()) return 0;
    rw::Engine::close(); std::free(gameEngine); gameEngine = nullptr; windowHandle = nullptr; return 1;
}
// 006b8510/006b9400 pass BYTE counts of aligned int32 arrays. librw performs
// endian conversion but returns bytes; preserve original handle/null contract.
S* __cdecl write32(S* s, const void* src, uint32_t n) {
    if (!valid(s) || s->access == 1 || n % 4 || (n && !src) || !reserve(s, n)) return nullptr;
    return !n || s->sidecar->write32(src, n) == n ? s : nullptr;
}
// 006b90b0 writes float words; no numeric conversion is expected by callers.
S* __cdecl writeReal(S* s, const void* src, uint32_t n) { return write32(s, src, n); }
// 006b8d50/006bb640 expect the original stream identity on complete int32 read.
S* __cdecl read32(S* s, void* dst, uint32_t n) {
    if (!valid(s) || s->access != 1 || n % 4 || (n && !dst)) return nullptr;
    if (!n) return s;
    uint32_t got = streamRead(s, dst, n);
    // x86 game words are little-endian; librw supplies the POD endian helper.
    rw::memNative32(dst, got & ~3u);
    return got == n ? s : nullptr;
}
// 006bddc0 explicitly passes version=225282, build=55. Upstream's helper uses
// mutable process globals and always returns true, even on a short write.
S* __cdecl chunkWrite(S* s, uint32_t type, uint32_t n, uint32_t version, uint16_t build) {
    uint32_t header[3] = {type, n, rw::libraryIDPack(version, build)};
    return write32(s, header, sizeof(header));
}
// 006b23e0/006bec60 allocate 20 bytes for the old header, not librw's 16.
// Upstream accepts truncated headers on some streams; check all 12 bytes first.
int __cdecl chunkRead(S* s, csf_rw::ChunkHeader* h) {
    uint32_t words[3];
    if (!h || !read32(s, words, sizeof(words))) return 0;
    *h = {words[0], words[1], static_cast<uint32_t>(rw::libraryIDUnpackVersion(words[2])),
          static_cast<uint32_t>(rw::libraryIDUnpackBuild(words[2])), 0};
    return 1;
}
// 006b7190/006be9c0 allow null size/version outputs; leave position at payload.
int __cdecl chunkFind(S* s, uint32_t type, uint32_t* n, uint32_t* version) {
    csf_rw::ChunkHeader h;
    while (chunkRead(s, &h)) {
        if (!h.type) return 0;
        if (h.type == type) { if (n) *n = h.length; if (version) *version = h.version; return 1; }
        if (!streamSkip(s, h.length)) return 0;
    }
    return 0;
}
}

namespace csf_rw {
bool streamPrepareWrite(Stream* s, uint32_t n) {
    return valid(s) && s->access != 1 && reserve(s,n);
}
}

HOOK(0x0058F210, atomicPlugin, "boundary-rw");
HOOK(0x00590400, sectorPlugin, "boundary-rw");
HOOK(0x00590BA0, geometryPlugin, "boundary-rw");
HOOK(0x00591590, materialPlugin, "boundary-rw");
HOOK(0x0059CEC0, lightPlugin, "boundary-rw");
HOOK(0x00623950, framePlugin, "boundary-rw");
HOOK(0x00626AE0, enginePlugin, "boundary-rw");
HOOK(0x00626AB0, getEngineFlag, "boundary-rw");
HOOK(0x00626AC0, setEngineFlag, "boundary-rw");
HOOK(0x00626B30, subSystem, "boundary-rw");
HOOK(0x00626B90, numModes, "boundary-rw");
HOOK(0x00626C40, videoMode, "boundary-rw");
HOOK(0x00626CE0, engineStop, "boundary-rw");
HOOK(0x00626D60, engineStart, "boundary-rw");
HOOK(0x00626E30, engineClose, "boundary-rw");
HOOK(0x00626EC0, engineTerm, "boundary-rw");
HOOK(0x00627290, engineOpen, "boundary-rw");
HOOK(0x00627360, engineInit, "boundary-rw");
HOOK(0x006293E0, streamRead, "boundary-rw");
HOOK(0x00629530, streamWrite, "boundary-rw");
HOOK(0x00629700, streamSkip, "boundary-rw");
HOOK(0x00629810, streamClose, "boundary-rw");
HOOK(0x006298E0, streamOpen, "boundary-rw");
HOOK(0x00629A80, chunkWrite, "boundary-rw");
HOOK(0x00629AE0, chunkFind, "boundary-rw");
HOOK(0x00629BE0, writeReal, "boundary-rw");
HOOK(0x00629C70, write32, "boundary-rw");
HOOK(0x00629C90, read32, "boundary-rw");
HOOK(0x00629CE0, chunkRead, "boundary-rw");
#endif
