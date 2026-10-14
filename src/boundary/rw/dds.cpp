// DDS container reader. librw has no DDS parser; its unmodified D3D9 raster
// allocator/locks and offline D3D texture storage own all native pixels.
#ifdef CSF_STANDALONE
#include "globals.h"
#ifdef CSF_RW_NATIVE_TEST
#define HOOK(addr, fn, module) template <> void* const ::csf::fn_slot<(addr)>::p = (void*)&fn
#else
#include "hook.h"
#endif
#include "image.h"
#include "startup.h"
#include <algorithm>
#include <cstring>
#include <vector>
#include <cstdio>

CSF_FUNC(csf_rw::Raster*, __cdecl, createDDSRaster, 0x00627770, (int,int,int,int));
CSF_FUNC(int, __cdecl, destroyDDSRaster, 0x00627570, (csf_rw::Raster*));
CSF_FUNC(uint32_t, __cdecl, readDDSBytes, 0x006293E0, (csf_rw::Stream*,void*,uint32_t));

namespace {
bool read(csf_rw::Stream* s,void* p,uint32_t n) { return readDDSBytes(s,p,n)==n; }
uint32_t word(const unsigned char* p,int bytes) {
    uint32_t v=0; for(int i=0;i<bytes;++i) v|=uint32_t(p[i])<<(i*8); return v;
}
bool maskValid(uint32_t mask,int bits) {
    if(bits<32 && (mask>>bits)) return false;
    if(!mask) return true;
    while(!(mask&1)) mask>>=1;
    return (mask&(mask+1))==0;
}
uint8_t component(uint32_t value,uint32_t mask,uint8_t fallback) {
    if(!mask) return fallback;
    while(!(mask&1)) { mask>>=1; value>>=1; }
    return uint8_t((uint64_t(value&mask)*255+mask/2)/mask);
}
bool dxtAlpha(const std::vector<unsigned char>& pixels,uint32_t w,uint32_t h) {
    for(uint32_t by=0;by<(h+3)/4;++by) for(uint32_t bx=0;bx<(w+3)/4;++bx) {
        auto* b=pixels.data()+(by*((w+3)/4)+bx)*8;
        if(word(b,2)>word(b+2,2)) continue;
        auto indices=word(b+4,4);
        for(uint32_t y=0;y<4 && by*4+y<h;++y) for(uint32_t x=0;x<4 && bx*4+x<w;++x)
            if(((indices>>((y*4+x)*2))&3)==3) return true;
    }
    return false;
}
csf_rw::Raster* __cdecl load(csf_rw::Stream* s) {
    if(rw::Engine::state!=rw::Engine::Started) return nullptr;
    uint32_t magic=0,hdr[31]{};
    if(!read(s,&magic,4) || magic!=0x20534444 || !read(s,hdr,sizeof(hdr))) return nullptr;
    // DDS_HEADER (public file format): flags +04, H/W +08/+0c, mip count
    // +18, pixel format +48, caps2 +6c. Matches 0063a4d0's 124-byte read.
    const uint32_t flags=hdr[1],h=hdr[2],w=hdr[3],pf=hdr[19],fourcc=hdr[20],bits=hdr[21];
    const uint32_t rmask=hdr[22],gmask=hdr[23],bmask=hdr[24],amask=hdr[25];
    if(hdr[0]!=124 || hdr[18]!=32 || (flags&0x1007)!=0x1007 || !w || !h || w>32767 || h>32767 ||
       uint64_t(w)*h*4>INT32_MAX || (hdr[27]&0xFE00) || (hdr[27]&0x200000) || hdr[5]>1) return nullptr;
    uint32_t levels=(flags&0x20000)?hdr[6]:1;
    uint32_t maxLevels=uint32_t(rw::Raster::calculateNumLevels(int(w),int(h)));
    if(!levels || levels>maxLevels) return nullptr;
    int dxt=0;
    if(pf&4) {
        if(fourcc>=0x31545844 && fourcc<=0x35545844 && (fourcc&0x00FFFFFF)==0x00545844) dxt=int((fourcc>>24)-'0');
        if(!dxt) return nullptr; // DX10/float/vendor formats not supported.
    }
    bool palette=!dxt && pf==0x20; // original also accepts zero-mask P8.
    if(!dxt && bits==8 && !(rmask|gmask|bmask|amask) && !(pf&2)) palette=true;
    bool luminance=(pf&0x20000)!=0;
    bool alphaOnly=(pf&2)!=0;
    if(!dxt) {
        if(bits!=8 && bits!=16 && bits!=24 && bits!=32) return nullptr;
        if(palette && bits!=8) return nullptr;
        if(!palette && !luminance && !alphaOnly && !(pf&0x40)) return nullptr;
        if(!maskValid(rmask,int(bits)) || !maskValid(gmask,int(bits)) || !maskValid(bmask,int(bits)) || !maskValid(amask,int(bits)) ||
           (rmask&gmask) || (rmask&bmask) || (rmask&amask) || (gmask&bmask) || (gmask&amask) || (bmask&amask)) return nullptr;
    }
    uint32_t paletteWords[256]{};
    if(palette && !read(s,paletteWords,sizeof(paletteWords))) return nullptr;
    std::vector<std::vector<unsigned char>> data;
    std::vector<uint32_t> pitches;
    uint64_t total=0;
    try {
        for(uint32_t i=0,iw=w,ih=h;i<levels;++i,iw=std::max(1u,iw/2),ih=std::max(1u,ih/2)) {
            uint64_t pitch=dxt?uint64_t((iw+3)/4)*(dxt==1?8:16):uint64_t(iw)*(bits/8);
            uint32_t rows=dxt?(ih+3)/4:ih;
            if(!dxt && i==0 && (flags&8)) { if(hdr[4]<pitch) return nullptr; pitch=hdr[4]; }
            uint64_t n=pitch*rows; total+=n;
            if(n>INT32_MAX || total>INT32_MAX) return nullptr;
            data.emplace_back(size_t(n)); pitches.push_back(uint32_t(pitch));
            if(!read(s,data.back().data(),uint32_t(n))) return nullptr;
        }
    } catch(...) { return nullptr; }
    bool alpha=dxt?dxt!=1 || dxtAlpha(data[0],w,h):amask!=0 || alphaOnly || palette;
    int format=dxt?(dxt==1?(alpha?rw::Raster::C1555:rw::Raster::C565):rw::Raster::C4444):(alpha?rw::Raster::C8888:rw::Raster::C888);
    if(flags&0x20000) format|=rw::Raster::MIPMAP;
    auto* facade=createDDSRaster(int(w),int(h),dxt?16:32,format|rw::Raster::TEXTURE|rw::Raster::DONTALLOCATE);
    if(!facade) return nullptr;
    auto* raster=csf_rw::rasterNative(facade);
    auto* ext=GETD3DRASTEREXT(raster);
    if(dxt) rw::d3d::allocateDXT(raster,dxt,int(levels),alpha);
    else {
        // Public D3DFMT_A8R8G8B8 / X8R8G8B8. Preserve the opaque/alpha
        // format class: 00705830 reads that class from the ABI raster.
        ext->format=alpha?21:22; ext->bpp=4; ext->hasAlpha=alpha;
        ext->texture=rw::d3d::createTexture(int(w),int(h),int(levels),0,ext->format);
        raster->flags&=~rw::Raster::DONTALLOCATE;
    }
    if(!ext->texture) { destroyDDSRaster(facade); return nullptr; }
    for(uint32_t i=0,iw=w,ih=h;i<levels;++i,iw=std::max(1u,iw/2),ih=std::max(1u,ih/2)) {
        auto* dst=raster->lock(int(i),rw::Raster::LOCKWRITE|rw::Raster::LOCKNOFETCH);
        if(!dst) { destroyDDSRaster(facade); return nullptr; }
        if(dxt) {
            uint32_t rowBytes=((iw+3)/4)*(dxt==1?8:16),rows=(ih+3)/4;
            // Offline librw's compressed stride is width*bpp, not block pitch;
            // allocation is contiguous. Real D3D9 LockRect supplies row pitch.
#ifdef RW_D3D9
            uint32_t dstPitch=uint32_t(raster->stride);
#else
            uint32_t dstPitch=rowBytes;
#endif
            if(dstPitch<rowBytes) { raster->unlock(int(i)); destroyDDSRaster(facade); return nullptr; }
            for(uint32_t y=0;y<rows;++y) std::memcpy(dst+size_t(y)*dstPitch,data[i].data()+size_t(y)*pitches[i],rowBytes);
        } else {
            for(uint32_t y=0;y<ih;++y) for(uint32_t x=0;x<iw;++x) {
                auto* src=data[i].data()+size_t(y)*pitches[i]+x*(bits/8);
                auto* p=dst+size_t(y)*raster->stride+x*4;
                uint32_t v=word(src,int(bits/8));
                if(palette) { auto c=paletteWords[v]; p[0]=uint8_t(c>>16); p[1]=uint8_t(c>>8); p[2]=uint8_t(c); p[3]=uint8_t(c>>24); }
                else if(alphaOnly) { p[0]=p[1]=p[2]=255; p[3]=component(v,amask?amask:(bits==32?UINT32_MAX:((1u<<bits)-1)),255); }
                else if(luminance) { p[0]=p[1]=p[2]=component(v,rmask,0); p[3]=component(v,amask,255); }
                else { p[0]=component(v,bmask,0); p[1]=component(v,gmask,0); p[2]=component(v,rmask,0); p[3]=component(v,amask,255); }
            }
        }
        raster->unlock(int(i));
    }
    csf_rw::rasterExport(facade);
    if(csf_rw::startupTracing()) std::fprintf(stderr,"RW DDS %ux%u levels=%u DXT=%d alpha=%d -> %p\n",w,h,levels,dxt,alpha,static_cast<void*>(facade));
    return facade;
}
}
HOOK(0x0063A4D0,load,"boundary-rw");
#endif
