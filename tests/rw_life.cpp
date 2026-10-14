#include "globals.h"
#include "boundary/rw/life.h"
#include "boundary/rw/world.h"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <unordered_map>
using S = csf_rw::Stream;
using F = csf_rw::Frame;
using Mem = csf_rw::Memory;
using Header = csf_rw::ChunkHeader;
using C = rw::Constructor;
using D = rw::Destructor;
using CP = rw::CopyConstructor;
CSF_GLOBAL(void*, facade, 0x008B68C0);
CSF_FUNC(int, __cdecl, init, 0x00627360, (const csf_rw::MemoryFunctions*, char, int));
CSF_FUNC(int, __cdecl, open, 0x00627290, (void*));
CSF_FUNC(int, __stdcall, start, 0x00626D60, ());
CSF_FUNC(int, __stdcall, stop, 0x00626CE0, ());
CSF_FUNC(int, __stdcall, close, 0x00626E30, ());
CSF_FUNC(int, __stdcall, term, 0x00626EC0, ());
CSF_FUNC(int, __cdecl, engineFlag, 0x00626AB0, ());
CSF_FUNC(void, __cdecl, setEngineFlag, 0x00626AC0, (int));
CSF_FUNC(int, __stdcall, modes, 0x00626B90, ());
CSF_FUNC(int, __cdecl, subsystem, 0x00626B30, (int));
CSF_FUNC(int, __cdecl, mode, 0x00626C40, (int));
CSF_FUNC(int, __cdecl, regEngine, 0x00626AE0, (int, uint32_t, C, D));
CSF_FUNC(int, __cdecl, regFrame, 0x00623950, (int, uint32_t, C, D, CP));
CSF_FUNC(int, __cdecl, regAtomic, 0x0058F210, (int, uint32_t, C, D, CP));
CSF_FUNC(int, __cdecl, regSector, 0x00590400, (int, uint32_t, C, D, CP));
CSF_FUNC(int, __cdecl, regGeometry, 0x00590BA0, (int, uint32_t, C, D, CP));
CSF_FUNC(int, __cdecl, regMaterial, 0x00591590, (int, uint32_t, C, D, CP));
CSF_FUNC(int, __cdecl, regLight, 0x0059CEC0, (int, uint32_t, C, D, CP));
CSF_FUNC(F*, __stdcall, frameCreate, 0x006235D0, ());
CSF_FUNC(int, __cdecl, frameDestroy, 0x00623B10, (F*));
CSF_FUNC(csf_rw::ObjectWithFrame*, __cdecl, setFrame, 0x0063CE30, (csf_rw::ObjectWithFrame*, F*));
CSF_FUNC(rw::Quat*, __cdecl, fromMatrix, 0x007D9D40, (rw::Quat*, const rw::Matrix*));
CSF_FUNC(rw::Quat*, __cdecl, rotateQuat, 0x007D9E30, (rw::Quat*, const rw::V3d*, float, rw::CombineOp));
CSF_FUNC(S*, __cdecl, streamOpen, 0x006298E0, (int, int, void*));
CSF_FUNC(int, __cdecl, streamClose, 0x00629810, (S*, Mem*));
CSF_FUNC(uint32_t, __cdecl, read, 0x006293E0, (S*, void*, uint32_t));
CSF_FUNC(S*, __cdecl, write, 0x00629530, (S*, const void*, uint32_t));
CSF_FUNC(S*, __cdecl, skip, 0x00629700, (S*, uint32_t));
CSF_FUNC(S*, __cdecl, writeReal, 0x00629BE0, (S*, const void*, uint32_t));
CSF_FUNC(S*, __cdecl, writeInt, 0x00629C70, (S*, const void*, uint32_t));
CSF_FUNC(S*, __cdecl, readInt, 0x00629C90, (S*, void*, uint32_t));
CSF_FUNC(S*, __cdecl, writeHeader, 0x00629A80, (S*, uint32_t, uint32_t, uint32_t, uint16_t));
CSF_FUNC(int, __cdecl, readHeader, 0x00629CE0, (S*, Header*));
CSF_FUNC(int, __cdecl, find, 0x00629AE0, (S*, uint32_t, uint32_t*, uint32_t*));
static int engineCtors, engineDtors, frameCtors, frameDtors, engineOffset, frameOffset;
static bool failCtor;
static bool failEngineCtor;
static void* __cdecl extraEngineCtor(void* p, int, int) { return failEngineCtor ? nullptr : p; }
static void* __cdecl engineCtor(void* p, int off, int n) {
    assert(p == facade && off == engineOffset && n == 4); ++engineCtors;
    *reinterpret_cast<uint32_t*>(static_cast<char*>(p)+off) = 0x12345678;
    return p;
}
static void* __cdecl engineDtor(void* p, int off, int n) {
    assert(p == facade && off == engineOffset && n == 4); ++engineDtors; return p;
}
static void* __cdecl frameCtor(void* p, int off, int n) {
    assert(off == frameOffset && n == 8 && static_cast<F*>(p)->object.type == 0);
    if (failCtor) return nullptr;
    ++frameCtors; std::memset(static_cast<char*>(p)+off, 0x5a, n); return p;
}
static void* __cdecl frameDtor(void* p, int off, int n) {
    assert(off == frameOffset && n == 8); ++frameDtors; return p;
}
static void* __cdecl failAlloc(size_t, rw::uint32) { return nullptr; }
static void* __cdecl failRealloc(void*, size_t, rw::uint32) { return nullptr; }
static int customAllocs, customFrees, customReallocs;
static int customLive;
static std::unordered_map<void*,uint32_t> liveSizes;
static void* __cdecl customMalloc(uint32_t n, uint32_t) {
    ++customAllocs; void* p=std::malloc(n); if(p) { ++customLive; liveSizes[p]=n; } return p;
}
static void __cdecl customFree(void* p) { if (p) { ++customFrees; --customLive; liveSizes.erase(p); } std::free(p); }
static void* __cdecl customRealloc(void* p, uint32_t n, uint32_t) {
    ++customReallocs; bool fresh=!p; uintptr_t old=reinterpret_cast<uintptr_t>(p);
    void* q=std::realloc(p,n); if(q) { if(fresh) ++customLive; liveSizes.erase(reinterpret_cast<void*>(old)); liveSizes[q]=n; } return q;
}
static void* __cdecl customCalloc(uint32_t n, uint32_t size, uint32_t) {
    ++customAllocs; void* p=std::calloc(n,size); if(p) { ++customLive; liveSizes[p]=n*size; } return p;
}
static void payload(S* s) {
    uint32_t word = 0x11223344; float real = 1.25f;
    assert(writeHeader(s, 99, 3, 0x37002, 55) == s);
    assert(write(s, "xyz", 3) == s);
    assert(writeHeader(s, 100, 8, 0x37002, 55) == s);
    assert(writeInt(s, &word, 4) == s); assert(writeReal(s, &real, 4) == s);
    assert(!readInt(s, &word, 4));
    assert(!writeInt(s, &word, 3));
}
static void check(S* s) {
    uint32_t n=0, v=0, word=0, bits=0; Header h{};
    assert(readHeader(s, &h));
    assert(h.type == 99 && h.length == 3 && h.version == 0x37002 && h.build == 55);
    char text[4]{}; assert(read(s, text, 3) == 3 && !std::strcmp(text,"xyz"));
    assert(find(s, 100, &n, &v) && n == 8 && v == 0x37002);
    assert(readInt(s, &word, 4) == s && word == 0x11223344);
    assert(readInt(s, &bits, 4) == s); float real; std::memcpy(&real, &bits, 4); assert(real == 1.25f);
    h.type = 999; assert(!readHeader(s, &h) && h.type == 999);
    assert(!write(s, text, 3));
}
int main() {
    assert(!start() && !stop() && !close() && !term());
    assert(!frameCreate()); // no lazy Init/Open/Start
    assert(regFrame(8, 42, frameCtor, frameDtor, nullptr) == -1);
    assert(!init(nullptr, 1, 0)); assert(init(nullptr, 0, 1024)); assert(!init(nullptr,0,0));
    engineOffset = regEngine(4, 42, engineCtor, engineDtor); assert(engineOffset >= 300);
    frameOffset = regFrame(8, 42, frameCtor, frameDtor, nullptr); assert(frameOffset == sizeof(F));
    // Native fixtures widen pointers; the actual x86 prefixes remain asserted
    // as 116/96/28/64 in abi.h. Tail storage must not overlap a widened header.
    assert(regAtomic(4,42,nullptr,nullptr,nullptr)==sizeof(csf_rw::Atomic));
    assert(regGeometry(4,42,nullptr,nullptr,nullptr)==sizeof(csf_rw::Geometry));
    assert(regSector(4,42,nullptr,nullptr,nullptr)==sizeof(csf_rw::Sector));
    assert(regMaterial(4,42,nullptr,nullptr,nullptr)==sizeof(csf_rw::Material));
    assert(regLight(4,42,nullptr,nullptr,nullptr)==sizeof(csf_rw::Light));
    assert(regFrame(8,42,nullptr,nullptr,nullptr)==-1);
    auto old = rw::Engine::memfuncs.rwmalloc;
    rw::Engine::memfuncs.rwmalloc = failAlloc;
    assert(regLight(4,43,nullptr,nullptr,nullptr)==-1);
    rw::Engine::memfuncs.rwmalloc = old;
    int windowToken;
    void* window=&windowToken;
    assert(!start()); assert(open(&window)); assert(!open(nullptr));
    window=nullptr; // caller's EngineOpenParams storage can disappear/change
    assert(csf_rw::engineWindow()==&windowToken);
    assert(facade && facade != rw::engine);
    assert(regFrame(8,43,nullptr,nullptr,nullptr)==-1);
    assert(modes() == rw::Engine::getNumVideoModes());
    assert(engineFlag() == 0); setEngineFlag(1); assert(engineFlag() == 1); setEngineFlag(0);  // plain global 0x8B68C4
    assert(subsystem(0) == rw::Engine::setSubSystem(0));
    assert(mode(0) == rw::Engine::setVideoMode(0));
    assert(!term()); assert(start()); assert(engineCtors == 1); assert(!start());
    F* f = frameCreate(); assert(f && frameCtors == 1); assert(!stop());
    csf_rw::ObjectWithFrame obj{};
    assert(setFrame(&obj,f)==&obj && obj.object.parent==f && f->objects.next==&obj.inFrame);
    assert(setFrame(&obj,nullptr)==&obj && !obj.object.parent && f->objects.next==&f->objects);
    rw::Matrix matrix; matrix.setIdentity(); rw::Quat q{};
    assert(fromMatrix(&q,&matrix)==&q && std::fabs(q.w-1.0f)<0.0001f);
    rw::V3d axis=rw::makeV3d(0,0,1);
    assert(rotateQuat(&q,&axis,180,rw::COMBINEREPLACE)==&q);
    assert(std::fabs(q.z-1.0f)<0.0001f && std::fabs(q.w)<0.0001f);
    assert(*reinterpret_cast<unsigned char*>(reinterpret_cast<char*>(f)+frameOffset) == 0x5a);
    frameDestroy(f); assert(frameDtors == 1);
    failCtor = true; assert(!frameCreate()); failCtor = false;
    Mem mem{}; S* s = streamOpen(3, 2, &mem); assert(s);
    assert(!streamOpen(4,1,&mem)); assert(!streamOpen(2,1,const_cast<char*>("/not/a/real/file")));
    auto oldRealloc = rw::Engine::memfuncs.rwrealloc;
    rw::Engine::memfuncs.rwrealloc = failRealloc; assert(!write(s,"x",1));
    rw::Engine::memfuncs.rwrealloc = oldRealloc;
    payload(s); assert(streamClose(s,&mem)); assert(mem.length == 35);
    s = streamOpen(3,1,&mem); assert(s); check(s); assert(streamClose(s,nullptr));
    s = streamOpen(3,1,&mem); assert(s); assert(find(s,100,nullptr,nullptr)); assert(streamClose(s,nullptr));
    s = streamOpen(3,1,&mem); assert(s);
    unsigned char all[64]; assert(read(s,all,UINT32_MAX)==mem.length);
    assert(read(s,all,1)==0); assert(streamClose(s,nullptr));
    s = streamOpen(3,1,&mem); assert(s); assert(skip(s,12)==s); assert(!skip(s,UINT32_MAX));
    assert(!skip(s,mem.length)); assert(streamClose(s,nullptr));
    for (uint32_t i=0;i<12;++i) {
        Mem truncated{mem.start,i}; s=streamOpen(3,1,&truncated); assert(s);
        Header h{777}; assert(!readHeader(s,&h) && h.type==777); assert(streamClose(s,nullptr));
    }
    rw::Engine::memfuncs.rwfree(mem.start);
    // Unique filename supplied by tmpnam, then only librw StreamFile performs IO.
    char path[L_tmpnam]; assert(std::tmpnam(path));
    s=streamOpen(2,2,path); assert(s); payload(s); assert(streamClose(s,nullptr));
    s=streamOpen(2,1,path); assert(s); check(s);
    assert(stop()); assert(!close()); // active stream owns engine file interface
    assert(streamClose(s,nullptr)); assert(close()); assert(!facade); assert(term());
    assert(engineDtors==1); assert(std::remove(path)==0);
    // Reinitialization resets ABI registries and native plugin descriptors.
    assert(init(nullptr,0,0)); assert(regMaterial(4,77,nullptr,nullptr,nullptr)==sizeof(csf_rw::Material));
    assert(open(nullptr)); assert(start()); assert(stop()); assert(close()); assert(term());
    csf_rw::MemoryFunctions custom{customMalloc,customFree,customRealloc,customCalloc};
    assert(init(&custom,0,0)); assert(open(nullptr)); assert(start());
    auto* dispatch=static_cast<uintptr_t*>(facade);
    assert(dispatch[66]==reinterpret_cast<uintptr_t>(customMalloc));
    assert(dispatch[67]==reinterpret_cast<uintptr_t>(customFree));
    assert(dispatch[68]==reinterpret_cast<uintptr_t>(customRealloc));
    assert(dispatch[69]==reinterpret_cast<uintptr_t>(customCalloc));
    void* zeros=reinterpret_cast<decltype(custom.calloc)>(dispatch[69])(2,8,0);
    assert(zeros); for(int i=0;i<16;++i) assert(static_cast<unsigned char*>(zeros)[i]==0);
    reinterpret_cast<decltype(custom.free)>(dispatch[67])(zeros);
    mem={}; s=streamOpen(3,2,&mem); assert(s); payload(s); assert(streamClose(s,nullptr));
    assert(stop()); assert(close()); assert(term());
    assert(customAllocs>0 && customFrees>0 && customReallocs>0 && customLive==0 && liveSizes.empty());
    assert(init(nullptr,0,0));
    engineOffset=regEngine(4,42,engineCtor,engineDtor);
    assert(regEngine(4,43,extraEngineCtor,nullptr)>engineOffset);
    assert(open(nullptr)); failEngineCtor=true;
    int dtorsBefore=engineDtors;
    assert(!start() && rw::Engine::state==rw::Engine::Opened && engineDtors==dtorsBefore+1);
    failEngineCtor=false; assert(start()); assert(stop()); assert(close()); assert(term());
    std::puts("rw lifecycle: 30 slots linked; lifecycle/registries/memory+file streams/chunks/POD helpers passed");
}
