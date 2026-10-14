// Standalone-only ABI facade over the unmodified MIT librw checkout.
#ifdef CSF_STANDALONE
#include "globals.h"
#ifdef CSF_RW_NATIVE_TEST
#define HOOK(addr, fn, module) template <> void* const ::csf::fn_slot<(addr)>::p = (void*)&fn
#else
#include "hook.h"
#endif
#include "image.h"
#include <algorithm>
#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>
#include <limits>

namespace {
using I = csf_rw::Image;
using R = csf_rw::Raster;
using T = csf_rw::Texture;
using D = csf_rw::TexDictionary;
using K = csf_rw::Registry;
std::unordered_map<I*, rw::Image*> images;
struct RasterRecord { rw::Raster* native; int level = -1; };
std::unordered_map<R*, RasterRecord> rasters;
R* contexts[32]{};
unsigned contextCount;
std::unordered_map<T*, rw::Texture*> textures;
std::unordered_map<D*, rw::TexDictionary*> dictionaries;
csf_rw::Link dictionaryList;
D* initialDictionary;
std::string imagePath;
using ReadImage = I* (__cdecl *)(const char*);
using WriteImage = I* (__cdecl *)(I*, const char*);
struct Format { std::string ext; ReadImage read; WriteImage write; };
std::vector<Format> formats;
using FindTexture = T* (__cdecl *)(const char*);
using ReadTexture = T* (__cdecl *)(const char*, const char*);
FindTexture findCallback;
ReadTexture readCallback;
void initLink(csf_rw::Link& l) { l.next = l.prev = &l; }
void unlink(csf_rw::Link& l) { l.prev->next = l.next; l.next->prev = l.prev; initLink(l); }
void append(csf_rw::Link& head, csf_rw::Link& l) {
    l.prev = head.prev; l.next = &head; head.prev->next = &l; head.prev = &l;
}
template<class A> A* allocate(K k) {
    void* p = rw::Engine::memfuncs.rwmalloc(csf_rw::plugins(k).size, rw::MEMDUR_EVENT);
    if (p) std::memset(p, 0, csf_rw::plugins(k).size);
    return static_cast<A*>(p);
}
void freeFacade(void* p) { rw::Engine::memfuncs.rwfree(p); }
void exportImage(I* a, rw::Image* n) {
    a->flags=n->flags; a->width=n->width; a->height=n->height; a->depth=n->depth;
    a->stride=n->stride; a->pixels=n->pixels; a->palette=n->palette;
}
I* adoptImage(rw::Image* n) {
    if (!n) return nullptr;
    for (auto& p: images) if (p.second == n) { exportImage(p.first,n); return p.first; }
    auto* a = static_cast<I*>(rw::Engine::memfuncs.rwmalloc(sizeof(I),rw::MEMDUR_EVENT));
    if (!a) { n->destroy(); return nullptr; }
    exportImage(a,n);
    try { images.emplace(a,n); } catch (...) { freeFacade(a); n->destroy(); return nullptr; }
    return a;
}
// 006dba70/006dc2c0 create 32-bit RGBA images and dereference +10/+14.
// Keep the old prefix; librw's inserted bpp stays private. Bound dimensions
// before upstream's signed stride*height allocation arithmetic.
I* __cdecl imageCreate(int w, int h, int d) {
    if (rw::Engine::state != rw::Engine::Started || w <= 0 || h <= 0 ||
        (d!=4 && d!=8 && d!=16 && d!=24 && d!=32) ||
        int64_t(w)*h*(d<8?1:d/8) > INT32_MAX) return nullptr;
    return adoptImage(rw::Image::create(w,h,d));
}
// 006dba70 writes through pixels immediately; return facade/null, not void.
I* __cdecl imageAllocate(I* a) {
    auto* n = csf_rw::imageNative(a); if (!n) return nullptr;
    if (n->width<=0 || n->height<=0 || int64_t(n->width)*n->height*n->bpp>INT32_MAX) return nullptr;
    n->allocate(); exportImage(a,n);
    return n->pixels && (n->depth>8 || n->palette) ? a : nullptr;
}
// 006db8b0 destroys captured images; direct borrowed pixel pointers must not
// acquire ownership merely because librw's setPixels sets the allocation flag.
int __cdecl imageDestroy(I* a) {
    auto* n=csf_rw::imageNative(a); if (!n) return 0;
    images.erase(a); n->destroy(); freeFacade(a); return 1;
}
// 006c1620 saves/restores the complete semicolon-separated path string.
// librw splits its private copy into NUL-separated components; keep an ABI copy.
const char* __cdecl imageSetPath(const char* path) {
    if (!rw::engine) return nullptr;
    try { imagePath=path?path:""; } catch (...) { return nullptr; }
    rw::Image::setSearchPath(imagePath.c_str()); return imagePath.c_str();
}
// 00705dd0 expects a stable string to concatenate, never the split native list.
const char* __stdcall imageGetPath() { return imagePath.c_str(); }
template<int N> rw::Image* formatRead(const char* name) {
    if (N >= static_cast<int>(formats.size()) || !formats[N].read) return nullptr;
    return csf_rw::imageNative(formats[N].read(name));
}
template<int N> void formatWrite(rw::Image* n, const char* name) {
    if (N >= static_cast<int>(formats.size()) || !formats[N].write) return;
    I* a=adoptImage(n); if (a) formats[N].write(a,name);
}
// 006e2e60 passes BMP/PNG callback addresses. Callbacks always receive ABI
// images; adapt the native Image*/void callback types, do not cast them.
int __cdecl imageRegister(const char* ext, ReadImage read, WriteImage write) {
    if (!rw::engine || !ext || !*ext || formats.size()>=10) return 0;
    static rw::Image::fileRead readers[]={formatRead<0>,formatRead<1>,formatRead<2>,formatRead<3>,formatRead<4>,formatRead<5>,formatRead<6>,formatRead<7>,formatRead<8>,formatRead<9>};
    static rw::Image::fileWrite writers[]={formatWrite<0>,formatWrite<1>,formatWrite<2>,formatWrite<3>,formatWrite<4>,formatWrite<5>,formatWrite<6>,formatWrite<7>,formatWrite<8>,formatWrite<9>};
    size_t i=formats.size();
    try { formats.push_back({ext,read,write}); } catch (...) { return 0; }
    if (!rw::Image::registerFileFormat(ext,read?readers[i]:nullptr,write?writers[i]:nullptr)) {
        formats.pop_back(); return 0;
    }
    return 1;
}
bool sameExtension(const char* a, const char* b) { return rw::strncmp_ci(a,b,std::max(std::strlen(a),std::strlen(b))+1)==0; }
// 00705b40 passes an explicit .bmp extension; native read always APPENDS an
// extension. Resolve explicit names with librw's filesystem/search-path helper.
I* __cdecl imageRead(const char* name) {
    if (!rw::engine || !name) return nullptr;
    const char* ext=std::strrchr(name,'.');
    if (ext) for (auto& f: formats) if (f.read && sameExtension(ext+1,f.ext.c_str())) {
        char* full=rw::Image::getFilename(name); if (!full) return nullptr;
        I* a=f.read(full); freeFacade(full); return a;
    }
    return adoptImage(rw::Image::read(name));
}
// 006db8b0 writes an image to the selected extension and expects image/null.
// librw has no public generic writer: registered game callbacks retain that
// result contract. Native codecs themselves return void (see wrappers below).
I* __cdecl imageWrite(I* a, const char* name) {
    if (!csf_rw::imageNative(a) || !name) return nullptr;
    const char* ext=std::strrchr(name,'.'); if (!ext) return nullptr;
    for (auto& f: formats) if (f.write && sameExtension(ext+1,f.ext.c_str())) return f.write(a,name);
    return nullptr;
}
// 006e2e60 registers these four addresses as ABI image callbacks, not codecs
// taking native headers. Actual parsing/encoding belongs entirely to librw.
I* __cdecl readBMP(const char* name);
I* __cdecl readPNG(const char* name) {
    if (!name || !rw::engine) return nullptr;
    // getFileContents asserts on an absent file in this checkout.
    void* f=rw::engine->filefuncs.rwfopen(name,"rb"); if (!f) return nullptr;
    rw::engine->filefuncs.rwfclose(f); return adoptImage(rw::readPNG(name));
}
// Codecs return void and ignore short writes. Capture their encoded bytes via
// librw's documented FileFunctions boundary, then check real write AND close.
// This is a scoped, single-threaded adapter, not an upstream source patch.
struct Capture {
    std::vector<unsigned char> bytes;
    size_t position=0;
    bool failed=false;
    static void* open(const char*,const char*) { return active; }
    static int close(void*) { return 0; }
    static int seek(void*,long off,int whence) {
        int64_t base=whence==0?0:whence==1?int64_t(active->position):whence==2?int64_t(active->bytes.size()):-1;
        int64_t pos=base+off;
        if (base<0 || pos<0 || uint64_t(pos)>active->bytes.size()) return -1;
        active->position=size_t(pos); return 0;
    }
    static long tell(void*) { return static_cast<long>(active->position); }
    static int eof(void*) { return active->position==active->bytes.size(); }
    static size_t read(void* p,size_t size,size_t count,void*) {
        if (!size) return 0;
        count=std::min(count,(active->bytes.size()-active->position)/size);
        if (count) std::memcpy(p,active->bytes.data()+active->position,size*count);
        active->position+=size*count; return count;
    }
    static size_t write(const void* p,size_t size,size_t count,void*) {
        if (size && count>SIZE_MAX/size) { active->failed=true; return 0; }
        size_t n=size*count;
        try {
            auto* b=static_cast<const unsigned char*>(p);
            active->bytes.insert(active->bytes.end(),b,b+n);
        } catch (...) { active->failed=true; return 0; }
        return count;
    }
    static Capture* active;
};
Capture* Capture::active;
I* __cdecl readBMP(const char* name) {
    if (!name || !rw::engine || Capture::active) return nullptr;
    uint32_t length=0; auto* bytes=rw::getFileContents(name,&length);
    if (!bytes) return nullptr;
    // Native reader repeats the writer's modulo-padding error. Normalize only
    // byte-stream row padding, then let librw do all header/pixel decoding.
    Capture capture;
    auto word=[&](unsigned off) { uint32_t value; std::memcpy(&value,bytes+off,4); return value; };
    uint16_t depth=0;
    if (length>=54) std::memcpy(&depth,bytes+28,2);
    int w=length>=54?static_cast<int>(word(18)):0;
    int h=length>=54?static_cast<int>(word(22)):0;
    unsigned offset=length>=54?word(10):0;
    if (length<54 || bytes[0]!='B' || bytes[1]!='M' || word(14)<40 || word(30)!=0 ||
        w<=0 || h<=0 || (depth!=4 && depth!=8 && depth!=16 && depth!=24 && depth!=32) ||
        (depth==4 && (w&1)) || int64_t(w)*h*4>INT32_MAX || offset>length) {
        freeFacade(bytes); return nullptr;
    }
    size_t rowBytes=(size_t(w)*depth+7)/8, stride=(rowBytes+3)&~size_t(3);
    size_t nativeStride=rowBytes+(size_t(w)*(depth<8?1:depth/8))%4;
    if (size_t(h)*stride>length-offset) { freeFacade(bytes); return nullptr; }
    try {
        capture.bytes.resize(offset+nativeStride*h,0);
        std::memcpy(capture.bytes.data(),bytes,offset);
        for (int y=0;y<h;++y) std::memcpy(capture.bytes.data()+offset+y*nativeStride,bytes+offset+y*stride,rowBytes);
    } catch (...) { freeFacade(bytes); return nullptr; }
    freeFacade(bytes);
    auto saved=rw::engine->filefuncs;
    Capture::active=&capture;
    rw::engine->filefuncs.rwfopen=Capture::open; rw::engine->filefuncs.rwfclose=Capture::close;
    rw::engine->filefuncs.rwfseek=Capture::seek; rw::engine->filefuncs.rwftell=Capture::tell;
    rw::engine->filefuncs.rwfread=Capture::read; rw::engine->filefuncs.rwfeof=Capture::eof;
    auto* native=rw::readBMP(name);
    rw::engine->filefuncs=saved; Capture::active=nullptr; return adoptImage(native);
}
I* encodeImage(I* a,const char* name,bool png) {
    auto* n=csf_rw::imageNative(a);
    if (!n || !name || !n->pixels || n->width<=0 || n->height<=0 ||
        (n->depth!=4 && n->depth!=8 && n->depth!=16 && n->depth!=24 && n->depth!=32) ||
        (n->depth==4 && (n->width&1)) || (n->depth<=8 && !n->palette) ||
        int64_t(n->width)*n->height*4>INT32_MAX || n->stride<int64_t(n->width)*n->bpp || Capture::active) return nullptr;
    // PNG assumes tightly packed rows and mutates 16-bit images. BMP's 32-bit
    // header stride uses four bytes but writes three. Encode a private tight
    // clone (24-bit for BMP RGBA), keeping the caller's header/pixels intact.
    int depth=!png && n->depth==32?24:n->depth;
    auto* clone=rw::Image::create(n->width,n->height,depth); if (!clone) return nullptr;
    clone->allocate();
    if (!clone->pixels || (depth<=8 && !clone->palette)) { clone->destroy(); return nullptr; }
    for (int y=0;y<n->height;++y) {
        auto* src=n->pixels+y*n->stride; auto* dst=clone->pixels+y*clone->stride;
        if (depth!=n->depth) for (int x=0;x<n->width;++x) rw::conv_RGB888_from_RGB888(dst+x*3,src+x*4);
        else std::memcpy(dst,src,clone->stride);
    }
    if (depth<=8) std::memcpy(clone->palette,n->palette,(1<<depth)*4);
    Capture capture;
    auto saved=rw::engine->filefuncs;
    Capture::active=&capture;
    rw::engine->filefuncs.rwfopen=Capture::open;
    rw::engine->filefuncs.rwfclose=Capture::close;
    rw::engine->filefuncs.rwfwrite=Capture::write;
    if (png) rw::writePNG(clone,name); else rw::writeBMP(clone,name);
    rw::engine->filefuncs=saved; Capture::active=nullptr;
    clone->destroy();
    if (capture.failed || capture.bytes.empty()) return nullptr;
    if (!png) {
        // Native BMP adds rowBytes%4 padding instead of (-rowBytes)%4. Repair
        // the encoded row boundaries; the palette/header are native codec work.
        size_t rowBytes=(size_t(n->width)*depth+7)/8;
        size_t header=54+(depth<=8?(size_t(1)<<depth)*4:0);
        size_t nativeStride=rowBytes+rowBytes%4, stride=(rowBytes+3)&~size_t(3);
        if (capture.bytes.size()!=header+nativeStride*n->height) return nullptr;
        if (nativeStride!=stride) {
            try {
                std::vector<unsigned char> corrected(header+stride*n->height,0);
                std::memcpy(corrected.data(),capture.bytes.data(),header);
                for (int y=0;y<n->height;++y)
                    std::memcpy(corrected.data()+header+y*stride,capture.bytes.data()+header+y*nativeStride,rowBytes);
                capture.bytes.swap(corrected);
            } catch (...) { return nullptr; }
        }
    }
    void* f=saved.rwfopen(name,"wb"); if (!f) return nullptr;
    bool complete=saved.rwfwrite(capture.bytes.data(),1,capture.bytes.size(),f)==capture.bytes.size();
    int closed=saved.rwfclose(f); return complete && closed==0?a:nullptr;
}
I* __cdecl writeBMP(I* a,const char* name) { return encodeImage(a,name,false); }
I* __cdecl writePNG(I* a,const char* name) { return encodeImage(a,name,true); }
void exportRaster(R* a,rw::Raster* n) {
    a->pixels=n->pixels; a->palette=n->palette;
    a->width=n->width; a->height=n->height; a->depth=n->depth; a->stride=n->stride;
    a->offsetX=static_cast<short>(n->offsetX); a->offsetY=static_cast<short>(n->offsetY);
    a->type=static_cast<unsigned char>(n->type); a->flags=static_cast<unsigned char>(n->flags);
    a->privateFlags=static_cast<unsigned char>(n->privateFlags); a->format=static_cast<unsigned char>(n->format>>8);
    a->originalPixels=n->originalPixels; a->originalWidth=n->originalWidth;
    a->originalHeight=n->originalHeight; a->originalStride=n->originalStride;
}
// 00705b40 needs mipmapped textures, 007091d0 zero-sized DONTALLOCATE headers.
// NULL's own raster operations assert: use librw's OFFLINE D3D9 raster codec
// on the NULL engine, not a fake rendering device. Camera/Z resources require
// slice 7 and fail explicitly here. No upstream source or dispatch mutation.
R* __cdecl rasterCreate(int w,int h,int depth,int format) {
    if (rw::Engine::state!=rw::Engine::Started || w<0 || h<0 || w>32767 || h>32767 ||
        int64_t(w)*h*8>INT32_MAX || ((format>>8)&15)>10 ||
        ((format&0x6000)==0x6000) ||
        ((format&7)!=rw::Raster::NORMAL && (format&7)!=rw::Raster::TEXTURE)) return nullptr;
    R* a=allocate<R>(K::Raster); if (!a) return nullptr;
    auto* n=rw::Raster::create(w,h,depth,format,rw::PLATFORM_D3D9);
    if (!n) { freeFacade(a); return nullptr; }
    a->parent=a; exportRaster(a,n);
    try { rasters.emplace(a,RasterRecord{n}); } catch (...) { n->destroy(); freeFacade(a); return nullptr; }
    if (!csf_rw::constructPlugins(K::Raster,a)) { rasters.erase(a); n->destroy(); freeFacade(a); return nullptr; }
    return a;
}
// 00705a10 locks individual mip levels and reads temporary dimensions/stride.
unsigned char* __cdecl rasterLock(R* a,int level,int mode) {
    auto it=rasters.find(a); if (it==rasters.end() || it->second.level>=0 || !mode || !(mode&3) || (mode&~15)) return nullptr;
    auto* n=it->second.native;
    if (n->parent!=n || (n->flags&rw::Raster::DONTALLOCATE) || level<0 || level>=n->getNumLevels()) return nullptr;
    auto* p=n->lock(level,mode); if (p) it->second.level=level;
    exportRaster(a,n); return p;
}
// 00705a10 expects raster identity, restoring original dimensions on unlock.
R* __cdecl rasterUnlock(R* a) {
    auto it=rasters.find(a); if (it==rasters.end() || it->second.level<0) return nullptr;
    it->second.native->unlock(it->second.level); it->second.level=-1;
    exportRaster(a,it->second.native); return a;
}
// 006db160/00727bd0 free owned rasters; reject destruction while locked, in
// context, referenced by textures, or parenting another facade (avoid UAF).
int __cdecl rasterDestroy(R* a) {
    auto it=rasters.find(a); if (it==rasters.end() || it->second.level>=0) return 0;
    for (auto& t:textures) if (t.first->raster==a) return 0;
    for (auto& r:rasters) if (r.first!=a && r.first->parent==a) return 0;
    for (unsigned i=0;i<contextCount;++i) if (contexts[i]==a) return 0;
    csf_rw::destructPlugins(K::Raster,a); it->second.native->destroy(); rasters.erase(it); freeFacade(a); return 1;
}
// 006a0000's width thunk relies on +0c, not native Raster's width offset.
int __cdecl rasterWidth(R* a) { return a ? a->width:0; }
// 00705a10 uses level count to cap loading; DONTALLOCATE has no level storage.
int __cdecl rasterLevels(R* a) {
    auto* n=csf_rw::rasterNative(a); return n && !(n->flags&rw::Raster::DONTALLOCATE) ? n->getNumLevels():0;
}
// 00709930 stores plugin offset and adds it to a GAME raster pointer.
int __cdecl rasterPlugin(uint32_t id) { return csf_rw::plugins(K::Raster).getPluginOffset(id); }
// 00529d30/006dbbf0 bracket offscreen drawing with a bounded context stack.
R* __cdecl rasterPush(R* a) {
    auto* n=csf_rw::rasterNative(a);
    if (!n || contextCount==32 || !rw::Raster::pushContext(n)) return nullptr;
    contexts[contextCount++]=a; return a;
}
// 00726a70 pops and expects the popped facade, not native identity.
R* __stdcall rasterPop() {
    auto* a=csf_rw::rasterFacade(rw::Raster::popContext());
    if (a && contextCount) contexts[--contextCount]=nullptr;
    return a;
}
// 007091d0 makes DONTALLOCATE subviews; librw doesn't inherit format/depth.
// Supply those fields and keep the game parent chain rather than native root.
R* __cdecl rasterSub(R* a,R* parent,rw::Rect* rect) {
    auto* n=csf_rw::rasterNative(a); auto* p=csf_rw::rasterNative(parent);
    if (!n || !p || a==parent || !rect || !(n->flags&rw::Raster::DONTALLOCATE) ||
        rect->x<0 || rect->y<0 || rect->w<0 || rect->h<0 ||
        int64_t(rect->x)+rect->w>p->width || int64_t(rect->y)+rect->h>p->height) return nullptr;
    for (R* r=parent; r && r->parent!=r; r=r->parent) if (r->parent==a) return nullptr;
    n->subRaster(p,rect); n->depth=p->depth; n->format=p->format; n->type=p->type;
    a->parent=parent; exportRaster(a,n); return a;
}
// 00705b40 consumes image identity and outputs dimensions/depth/full flags.
I* __cdecl imageRasterFormat(I* a,int type,int* w,int* h,int* d,int* f) {
    auto* n=csf_rw::imageNative(a); if (!n || !w || !h || !d || !f || (type&15)!=rw::Raster::TEXTURE) return nullptr;
    return rw::Raster::imageFindRasterFormat(n,type,w,h,d,f,rw::PLATFORM_D3D9) ? a:nullptr;
}
// 00705d10 uploads RGBA pixels; 00705a10 temporarily adjusts headers for mips.
R* __cdecl rasterFromImage(R* a,I* image) {
    auto* n=csf_rw::rasterNative(a); auto* i=csf_rw::imageNative(image);
    if (!n || !i || !i->pixels || n->parent!=n || (n->flags&rw::Raster::DONTALLOCATE)) return nullptr;
    auto* ret=n->setFromImage(i,rw::PLATFORM_D3D9); exportRaster(a,n); return ret?a:nullptr;
}
// 006dc2c0 passes an already allocated capture image; copy/convert pixels into
// that image, not a replacement handle. Only matching native depth is bridged.
I* __cdecl imageFromRaster(I* a,R* raster) {
    auto* dst=csf_rw::imageNative(a); auto* r=csf_rw::rasterNative(raster);
    if (!dst || !r || r->parent!=r || (r->flags&rw::Raster::DONTALLOCATE)) return nullptr;
    rw::Image* src=nullptr;
    if (r->format & (rw::Raster::PAL4|rw::Raster::PAL8)) {
        // Upstream rasterToImage incorrectly calls a four-byte colour converter
        // on one-byte palette indices (ASan fixture catches the overrun). Keep
        // allocation/locking in librw and copy the verified POD index payload.
        bool locked=r->pixels==nullptr;
        if (locked && !r->lock(0,rw::Raster::LOCKREAD)) return nullptr;
        int depth=r->format&rw::Raster::PAL4?4:8;
        src=rw::Image::create(r->width,r->height,depth);
        if (src) {
            src->allocate();
            auto* palette=static_cast<unsigned char*>(GETD3DRASTEREXT(r)->palette);
            if (!src->pixels || !src->palette || !palette) { src->destroy(); src=nullptr; }
            else {
                std::memcpy(src->palette,palette,(1<<depth)*4);
                for (int y=0;y<r->height;++y) for (int x=0;x<r->width;++x)
                    rw::conv_8_from_8(src->pixels+y*src->stride+x,r->pixels+y*r->stride+x);
            }
        }
        if (locked) r->unlock(0);
    } else {
        int format=r->format&0xf00;
        if (format!=rw::Raster::C1555 && format!=rw::Raster::C555 &&
            format!=rw::Raster::C888 && format!=rw::Raster::C8888) return nullptr;
        src=r->toImage();
    }
    if (!src) return nullptr;
    if (dst->depth==32 && src->depth!=32) src->convertTo32();
    if (src->depth!=dst->depth || src->width!=dst->width || src->height!=dst->height || !dst->pixels || dst->stride<src->width*src->bpp) { src->destroy(); return nullptr; }
    for (int y=0;y<src->height;++y) std::memcpy(dst->pixels+y*dst->stride,src->pixels+y*src->stride,src->width*src->bpp);
    if (src->palette && dst->palette) std::memcpy(dst->palette,src->palette,(1<<src->depth)*4);
    src->destroy(); exportImage(a,dst); return a;
}
void exportTexture(T* a,rw::Texture* n) {
    std::memcpy(a->name,n->name,32); std::memcpy(a->mask,n->mask,32);
    a->filterAddressing=n->filterAddressing; a->refCount=n->refCount;
}
D* adoptDictionary(rw::TexDictionary* n) {
    if (!n) return nullptr;
    for (auto& d:dictionaries) if (d.second==n) return d.first;
    auto* a=allocate<D>(K::Dictionary); if (!a) return nullptr;
    a->object.type=rw::TexDictionary::ID;
    initLink(a->textures); initLink(a->inGlobalList);
    try { dictionaries.emplace(a,n); } catch (...) { freeFacade(a); return nullptr; }
    if (!csf_rw::constructPlugins(K::Dictionary,a)) { dictionaries.erase(a); freeFacade(a); return nullptr; }
    append(dictionaryList,a->inGlobalList);
    return a;
}
T* adoptTexture(rw::Texture* n) {
    if (!n) return nullptr;
    for (auto& t:textures) if (t.second==n) { exportTexture(t.first,n); return t.first; }
    auto* a=allocate<T>(K::Texture); if (!a) return nullptr;
    a->raster=csf_rw::rasterFacade(n->raster); initLink(a->inDict); exportTexture(a,n);
    try { textures.emplace(a,n); } catch (...) { freeFacade(a); return nullptr; }
    if (!csf_rw::constructPlugins(K::Texture,a)) { textures.erase(a); freeFacade(a); return nullptr; }
    return a;
}
// 006d0ba0/00705b40 transfer raster ownership to a texture; expose refCount=1
// at +54 and filter/address packed word at +50. Null raster is a valid header.
T* __cdecl textureCreate(R* a) {
    if (rw::Engine::state!=rw::Engine::Started) return nullptr;
    auto* r=csf_rw::rasterNative(a); if (a && !r) return nullptr;
    for (auto& t:textures) if (a && t.first->raster==a) return nullptr;
    auto* n=rw::Texture::create(r); if (!n) return nullptr;
    auto* t=adoptTexture(n); if (!t) { n->raster=nullptr; n->destroy(); } return t;
}
// 006c1120/006c1c90 remove a texture without changing its reference count.
T* __cdecl dictionaryRemove(T* a) {
    auto* n=csf_rw::textureNative(a); if (!n) return nullptr;
    if (n->dict) n->dict->remove(n);
    if (a->dict) { unlink(a->inDict); a->dict=nullptr; } return a;
}
// 006c1120 and 00726990 manipulate +54 directly. Import it before native
// release. On final release tear down facade/plugin/raster identity BEFORE
// librw destroys its owning raster; never leave dangling lookup entries.
int __cdecl textureDestroy(T* a) {
    auto* n=csf_rw::textureNative(a); if (!n || a->refCount<=0) return 0;
    if (n->refCount>1) { n->destroy(); a->refCount=n->refCount; return 1; }
    R* raster=a->raster;
    if (raster) {
        auto it=rasters.find(raster); if (it==rasters.end() || it->second.level>=0) return 0;
        for (auto& r:rasters) if (r.first!=raster && r.first->parent==raster) return 0;
        for (unsigned i=0;i<contextCount;++i) if (contexts[i]==raster) return 0;
    }
    dictionaryRemove(a); csf_rw::destructPlugins(K::Texture,a);
    if (raster) { csf_rw::destructPlugins(K::Raster,raster); rasters.erase(raster); freeFacade(raster); }
    textures.erase(a); n->destroy(); freeFacade(a); return 1;
}
// 006e3020 sets the dictionary lookup name; zero-terminate the 32-byte prefix.
T* __cdecl textureName(T* a,const char* name) {
    auto* n=csf_rw::textureNative(a); if (!n || !name) return nullptr;
    std::memset(n->name,0,32); std::strncpy(n->name,name,31); exportTexture(a,n); return a;
}
// 006c1260 creates an empty dictionary; callbacks and lists see facades only.
D* __stdcall dictionaryCreate() {
    if (rw::Engine::state!=rw::Engine::Started) return nullptr;
    auto* n=rw::TexDictionary::create(); auto* a=adoptDictionary(n); if (!a && n) n->destroy(); return a;
}
// 006c11c0 moves textures between dictionaries without adding a reference.
D* __cdecl dictionaryAdd(D* d,T* t) {
    auto it=dictionaries.find(d); auto* n=csf_rw::textureNative(t);
    if (it==dictionaries.end() || !n) return nullptr;
    dictionaryRemove(t); it->second->add(n); t->dict=d; append(d->textures,t->inDict); return d;
}
// 00726b00 finds names case-insensitively; find itself does NOT add a reference.
T* __cdecl dictionaryFind(D* d,const char* name) {
    auto it=dictionaries.find(d); if (it==dictionaries.end() || !name) return nullptr;
    for (auto& t:textures) if (t.first->dict==d) csf_rw::textureNative(t.first);
    auto* n=it->second->find(name); return csf_rw::textureFacade(n);
}
// 006c1700 saves/restores current dict; native pointer stays private.
D* __cdecl dictionaryCurrent(D* d) {
    auto it=dictionaries.find(d); if (d && it==dictionaries.end()) return nullptr;
    rw::TexDictionary::setCurrent(d?it->second:nullptr); return d;
}
// 006c1060 consumes the identity of the engine's initial/current dictionary.
D* __stdcall dictionaryGetCurrent() {
    return rw::Engine::state==rw::Engine::Started?adoptDictionary(rw::TexDictionary::getCurrent()):nullptr;
}
// 006c17b0's callbacks read names/refcounts and may remove/destroy the current
// texture. Snapshot identities and test membership before each call; stop on
// null. Do not call native list visitors with a game callback pointer.
D* __cdecl dictionaryTextures(D* d,T* (__cdecl *cb)(T*,void*),void* data) {
    if (!cb || !dictionaries.count(d)) return nullptr;
    std::vector<T*> list;
    for (auto* l=d->textures.next;l!=&d->textures;l=l->next)
        list.push_back(reinterpret_cast<T*>(reinterpret_cast<char*>(l)-offsetof(T,inDict)));
    for (auto* t:list) if (textures.count(t) && t->dict==d && !cb(t,data)) break;
    return d;
}
// 006c1770 visits dictionaries with 006c10b0, finding a named texture through
// the supplied search record. Callbacks must receive game dictionaries.
// Native's private global list has no public visitor API.
int __cdecl allDictionaries(D* (__cdecl *cb)(D*,void*),void* data) {
    if (!cb) return 0; std::vector<D*> list;
    for (auto* l=dictionaryList.next;l!=&dictionaryList;l=l->next)
        list.push_back(reinterpret_cast<D*>(reinterpret_cast<char*>(l)-offsetof(D,inGlobalList)));
    for (auto* d:list) if (dictionaries.count(d) && !cb(d,data)) break;
    return 1;
}
// 006c1700 destroys the dictionary and releases each texture once; referenced
// textures survive detached. Extra export args are caller stack leftovers.
int __cdecl dictionaryDestroy(D* d) {
    auto it=dictionaries.find(d); if (it==dictionaries.end() || d==initialDictionary) return 0;
    while (d->textures.next!=&d->textures) {
        auto* t=reinterpret_cast<T*>(reinterpret_cast<char*>(d->textures.next)-offsetof(T,inDict));
        dictionaryRemove(t); if (!textureDestroy(t)) return 0;
    }
    csf_rw::destructPlugins(K::Dictionary,d); unlink(d->inGlobalList);
    it->second->destroy(); dictionaries.erase(it); freeFacade(d); return 1;
}
rw::Texture* nativeFind(const char* name) {
    T* t=nullptr;
    if (findCallback) t=findCallback(name);
    else if (auto* current=dictionaryGetCurrent()) t=dictionaryFind(current,name);
    else for (auto& d:dictionaries) if ((t=dictionaryFind(d.first,name))) break;
    return csf_rw::textureNative(t);
}
rw::Texture* nativeRead(const char* name,const char* mask) {
    if (readCallback) return csf_rw::textureNative(readCallback(name,mask));
    auto* i=rw::Image::readMasked(name,mask); if (!i) return nullptr;
    I* a=adoptImage(i); if (!a) return nullptr;
    int w,h,depth,format; T* t=nullptr;
    if (imageRasterFormat(a,rw::Raster::TEXTURE,&w,&h,&depth,&format)) {
        auto* r=rasterCreate(w,h,depth,format);
        if (r) {
            if (rasterFromImage(r,a)) t=textureCreate(r);
            if (!t) rasterDestroy(r);
        }
    }
    imageDestroy(a);
    if (t) { textureName(t,name); if (mask) std::strncpy(t->mask,mask,31); }
    return csf_rw::textureNative(t);
}
// 006eea50's apparent second find argument is the previous call's leftover
// stack value (confirmed by packet). Original is cdecl with ONE argument.
int __cdecl setFind(FindTexture cb) { findCallback=cb; rw::Texture::findCB=nativeFind; return 1; }
// 006eea50 installs a read(name,mask) callback returning a facade.
int __cdecl setRead(ReadTexture cb) { readCallback=cb; rw::Texture::readCB=nativeRead; return 1; }
// 006c1620 consumes found texture reference; native read adds ref on find and
// inserts newly read textures into current dictionary. Mirror both outcomes.
T* __cdecl textureRead(const char* name,const char* mask) {
    if (rw::Engine::state!=rw::Engine::Started || !name) return nullptr;
    auto* n=rw::Texture::read(name,mask); if (!n) return nullptr;
    auto* a=adoptTexture(n); if (!a) { n->destroy(); return nullptr; }
    if (n->dict) {
        auto* d=adoptDictionary(n->dict); if (!d) return nullptr;
        if (a->dict!=d) { if (a->dict) unlink(a->inDict); a->dict=d; append(d->textures,a->inDict); }
    }
    return a;
}
// 006c4eb0 reads the signed capability byte and clamps its requested value.
char __stdcall maxAnisotropy() { return static_cast<char>(std::min(127,rw::getMaxSupportedMaxAnisotropy())); }
// Same caller writes +50 filter byte BEFORE this call: import packed word.
T* __cdecl anisotropy(T* a,char level) {
    auto* n=csf_rw::textureNative(a); if (!n) return nullptr;
    n->setMaxAnisotropy(static_cast<unsigned char>(level)); return a;
}
}
namespace csf_rw {
rw::Image* imageNative(Image* a) {
    auto it=images.find(a); if (it==images.end()) return nullptr;
    auto* n=it->second; n->flags=a->flags; n->width=a->width; n->height=a->height;
    n->depth=a->depth; n->bpp=a->depth<8?1:a->depth/8; n->stride=a->stride;
    n->pixels=a->pixels; n->palette=a->palette; return n;
}
rw::Raster* rasterNative(Raster* a) { auto it=rasters.find(a); return it==rasters.end()?nullptr:it->second.native; }
Raster* rasterFacade(rw::Raster* n) { for (auto& p:rasters) if (p.second.native==n) return p.first; return nullptr; }
rw::Texture* textureNative(Texture* a) {
    auto it=textures.find(a); if (it==textures.end()) return nullptr;
    auto* n=it->second;
    auto* r=rasterNative(a->raster); if (a->raster && !r) return nullptr;
    n->raster=r;
    std::memcpy(n->name,a->name,32); std::memcpy(n->mask,a->mask,32);
    n->filterAddressing=a->filterAddressing; n->refCount=a->refCount; return n;
}
Texture* textureFacade(rw::Texture* n) { for (auto& p:textures) if (p.second==n) return p.first; return nullptr; }
void textureExport(Texture* a) { auto it=textures.find(a); if (it!=textures.end()) exportTexture(a,it->second); }
int textureRelease(Texture* a) { return textureDestroy(a); }
bool textureCanRelease(Texture* a,int references) {
    auto* n=textureNative(a); if(!n || a->refCount<=0) return false;
    if(n->refCount>references || !a->raster) return true;
    auto it=rasters.find(a->raster); if(it==rasters.end() || it->second.level>=0) return false;
    for(auto& r:rasters) if(r.first!=a->raster && r.first->parent==a->raster) return false;
    for(unsigned i=0;i<contextCount;++i) if(contexts[i]==a->raster) return false;
    return true;
}
bool imagesEmpty() {
    return !contextCount && images.empty() && rasters.empty() && textures.empty() &&
        (dictionaries.empty() || (dictionaries.size()==1 && dictionaries.count(initialDictionary)));
}
bool imageBoundaryStart() {
    initLink(dictionaryList);
    initialDictionary=adoptDictionary(rw::TexDictionary::getCurrent());
    findCallback=nullptr; readCallback=nullptr;
    rw::Texture::findCB=nativeFind; rw::Texture::readCB=nativeRead;
    return initialDictionary!=nullptr;
}
void imageBoundaryStop() {
    // Stop is guarded: only the engine-owned initial dictionary remains. Free
    // its game tail/map BEFORE native module destruction frees that dictionary.
    if (initialDictionary) {
        destructPlugins(K::Dictionary,initialDictionary);
        unlink(initialDictionary->inGlobalList);
        dictionaries.erase(initialDictionary); freeFacade(initialDictionary); initialDictionary=nullptr;
    }
    imagePath.clear(); formats.clear(); findCallback=nullptr; readCallback=nullptr;
}
bool textureStreamWrite(Texture* a,Stream* s) {
    auto* n=textureNative(a); auto* stream=streamNative(s);
    return n && stream && streamPrepareWrite(s,n->streamGetSize()+12) && n->streamWrite(stream);
}
Texture* textureStreamRead(Stream* s) {
    auto* stream=streamNative(s); if (!stream) return nullptr;
    auto* n=rw::Texture::streamRead(stream); return n?adoptTexture(n):nullptr;
}
}
HOOK(0x00566520,maxAnisotropy,"boundary-rw");
HOOK(0x00566530,anisotropy,"boundary-rw");
HOOK(0x005870D0,rasterWidth,"boundary-rw");
HOOK(0x00625060,imageCreate,"boundary-rw");
HOOK(0x006250C0,imageAllocate,"boundary-rw");
HOOK(0x00625270,imageSetPath,"boundary-rw");
HOOK(0x00625320,imageGetPath,"boundary-rw");
HOOK(0x006254C0,imageRead,"boundary-rw");
HOOK(0x00625680,imageRegister,"boundary-rw");
HOOK(0x006257E0,imageWrite,"boundary-rw");
HOOK(0x00625F10,imageDestroy,"boundary-rw");
HOOK(0x00627430,rasterPlugin,"boundary-rw");
HOOK(0x00627490,rasterUnlock,"boundary-rw");
HOOK(0x00627570,rasterDestroy,"boundary-rw");
HOOK(0x006275B0,rasterPush,"boundary-rw");
HOOK(0x00627650,rasterPop,"boundary-rw");
HOOK(0x006276A0,rasterLevels,"boundary-rw");
HOOK(0x00627710,rasterSub,"boundary-rw");
HOOK(0x00627770,rasterCreate,"boundary-rw");
HOOK(0x00627810,rasterLock,"boundary-rw");
HOOK(0x006315B0,setFind,"boundary-rw");
HOOK(0x006315D0,setRead,"boundary-rw");
HOOK(0x00631650,dictionaryCreate,"boundary-rw");
HOOK(0x006316E0,dictionaryTextures,"boundary-rw");
HOOK(0x00631730,allDictionaries,"boundary-rw");
HOOK(0x00631780,textureCreate,"boundary-rw");
HOOK(0x006317E0,textureDestroy,"boundary-rw");
HOOK(0x00631800,textureName,"boundary-rw");
HOOK(0x006318E0,dictionaryAdd,"boundary-rw");
HOOK(0x00631920,dictionaryRemove,"boundary-rw");
HOOK(0x00631950,dictionaryFind,"boundary-rw");
HOOK(0x00631990,dictionaryCurrent,"boundary-rw");
HOOK(0x006319B0,dictionaryGetCurrent,"boundary-rw");
HOOK(0x006319C0,textureRead,"boundary-rw");
HOOK(0x00632340,dictionaryDestroy,"boundary-rw");
HOOK(0x00633010,imageFromRaster,"boundary-rw");
HOOK(0x00633040,rasterFromImage,"boundary-rw");
HOOK(0x00633070,imageRasterFormat,"boundary-rw");
HOOK(0x007C5680,readBMP,"boundary-rw");
HOOK(0x007C5CA0,writeBMP,"boundary-rw");
HOOK(0x007C8D30,writePNG,"boundary-rw");
HOOK(0x007C9050,readPNG,"boundary-rw");
#endif
