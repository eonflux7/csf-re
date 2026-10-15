// Standalone boundary only; no SDK declarations and no native owning-object casts.
#ifdef CSF_STANDALONE
#include "globals.h"
#ifdef CSF_RW_NATIVE_TEST
#define HOOK(addr, fn, module) template <> void* const ::csf::fn_slot<(addr)>::p = (void*)&fn
#else
#include "hook.h"
#endif
#include "toolkit9.h"
#include <cstring>

using O = csf_rw::ToolkitObject;
using D = csf_rw::ToolkitDescriptor;
using H = csf_rw::ToolkitHandle;
CSF_GLOBAL(void*, engine, 0x008B68C0);
CSF_GLOBAL(D, visibilityDescriptor, 0x008851D8);
CSF_FUNC(rw::Matrix*, __cdecl, multiply, 0x00624630,
         (rw::Matrix*, const rw::Matrix*, const rw::Matrix*));

template<class T> static T load(const void* p, size_t offset) {
    T v; std::memcpy(&v, static_cast<const unsigned char*>(p)+offset, sizeof v); return v;
}
template<class T> static void store(void* p, size_t offset, T v) {
    std::memcpy(static_cast<unsigned char*>(p)+offset, &v, sizeof v);
}
static void release(void* p) {
    // Slice 2 publishes the original allocator at engine table index 67.
    auto freeFn = load<void (__cdecl *)(void*)>(engine, 67*sizeof(void*));
    freeFn(p);
}
static void alignedRelease(void* p) {
    auto bytes = static_cast<unsigned char*>(p);
    release(bytes - bytes[-1]); // original alignment allocation header, not _aligned_free
}
static int __cdecl releaseVisibilityProxy(O* o) { store(o->payload,0,static_cast<void*>(nullptr));return 1; }

// 006d6760 supplies a count. Exact RET-only slot: formal identity stays unknown.
static void __cdecl noOperation(uint32_t) {}

// 006d6a40/006d6760: owned 96-byte toolkit object, descriptor destructor then
// alignment-header release. Descriptor callbacks always receive the ABI object.
static int __cdecl destroyObject(O* o) {
    if(o->descriptor->methods[0]==reinterpret_cast<void*>(&releaseVisibilityProxy))
        reinterpret_cast<int (__cdecl *)(O*)>(o->descriptor->methods[0])(o);
    else reinterpret_cast<void (__cdecl *)(O*)>(o->descriptor->methods[0])(o);
    alignedRelease(o);
    return 1;
}

// 006ff080/00700250 use this to dispatch primitive bounds/output; NOT RpTeam.
static int __cdecl dispatchOutput(O* o, rw::Matrix* parent, int mode, void* output) {
    rw::Matrix combined;
    rw::Matrix* transform;
    if ((o->descriptor->flags & 2) || (o->matrix.flags & 0x20000)) transform = parent;
    else if (parent) transform = multiply(&combined, &o->matrix, parent);
    else transform = &o->matrix;
    return reinterpret_cast<int (__cdecl *)(O*, rw::Matrix*, int, void*)>(
        o->descriptor->methods[3])(o, transform, mode, output);
}

// 006b2000/006b2180/006b2240 pass matrices for box/sphere/capsule bounds.
static D* __cdecl setTransform(O* o, const rw::Matrix* m) {
    auto d = o->descriptor;
    if (!(d->flags & 2)) std::memcpy(&o->matrix, m, 64);
    return d; // original returns descriptor, NOT object
}
// 006d6ee0 saves/replaces the visibility proxy's callback at +48.
static uintptr_t __cdecl setCallback(O* o, uintptr_t cb) { store(o->payload,2*sizeof(void*),cb); return cb; }
static uintptr_t __cdecl getCallback(O* o) { return load<uintptr_t>(o->payload,2*sizeof(void*)); }

static O* __cdecl createVisibilityProxy(void* object,uint32_t kind) {
    // Compatible with alignedRelease (one-byte distance to engine allocation).
    auto alloc=load<void* (__cdecl *)(uint32_t,uint32_t)>(engine,66*sizeof(void*));
    auto* base=static_cast<unsigned char*>(alloc(sizeof(O)+16,198912));
    if(!base) return nullptr;
    auto at=(reinterpret_cast<uintptr_t>(base)+16)&~uintptr_t(15);
    auto* o=reinterpret_cast<O*>(at); reinterpret_cast<unsigned char*>(o)[-1]=uint8_t(at-reinterpret_cast<uintptr_t>(base));
    std::memset(o,0,sizeof(*o));o->matrix.setIdentity();o->descriptor=&visibilityDescriptor;
    store(o->payload,0,object);store(o->payload,sizeof(void*),kind);
    store(o->payload,2*sizeof(void*),reinterpret_cast<uintptr_t>(CSF_ADDR_FN(0x007FF600)));
    float value=.2f;std::memcpy(&o->multiplier,&value,4);std::memcpy(&o->offset,&value,4);
    return o;
}
static H* __cdecl dictAdd(H* h,void* entry,uintptr_t value) {
    auto** entries=static_cast<void**>(h->entries);
    uint32_t i=0;while(i<h->cursor && entries[i]) ++i;
    if(i==h->count) return nullptr;
    if(i==h->cursor) ++h->cursor;
    entries[i]=entry;static_cast<uintptr_t*>(h->scratch)[i]=value;return h;
}
static int __cdecl dictRemove(H* h,void* entry) {
    auto** entries=static_cast<void**>(h->entries);
    for(uint32_t i=0;i<h->cursor;++i) if(entries[i]==entry) {
        entries[i]=nullptr;if(i+1==h->cursor) --h->cursor;return 1;
    }
    return 0;
}

// 006e3d70/006e41d0/006e3f60/006d6ee0 pass scalar bit patterns, not RGBA.
// Type 8 is an inline array of 96-byte children, not an Rt2d scene hierarchy.
static void* propagate(O* o, uint32_t bits, bool offset) {
    auto d = o->descriptor;
    if (static_cast<uint16_t>(d->type) != 8) {
        (offset ? o->offset : o->multiplier) = bits;
        return d;
    }
    auto children = load<csf_rw::ToolkitChildren*>(o->payload,0);
    uint32_t i = 0;
    while (i < children->count) {
        propagate(&children->objects[i], bits, offset);
        children = load<csf_rw::ToolkitChildren*>(o->payload,0);
        ++i;
    }
    return children;
}
static void* __cdecl setOffset(O* o, uint32_t bits) { return propagate(o,bits,true); }
static void* __cdecl setMultiplier(O* o, uint32_t bits) { return propagate(o,bits,false); }

static void resetPrimitive(O* o, D* d) {
    store<uint32_t>(o->payload,12,0);
    o->control0 = o->control1 = 0;
    o->matrix.right = {1,0,0}; o->matrix.up = {0,1,0};
    o->matrix.at = {0,0,1}; o->matrix.pos = {0,0,0};
    o->matrix.flags |= 0x20003;
    o->multiplier = o->offset = 0x3e4ccccd; // 0.2f, scalar defaults
    o->descriptor = d;
    // Matrix padding and other payload words deliberately remain untouched.
}
// 006b2000 computes half-extents, not a position. Reinitializes box descriptor.
static O* __cdecl setBox(O* o, const void* v) {
    resetPrimitive(o,reinterpret_cast<D*>(CSF_ADDR_DATA(0x00884E38)));
    for (size_t i=0;i<12;i+=4) store(o->payload,i,load<uint32_t>(v,i));
    return o;
}
// 00700250 consumes three primitive dimensions. Does NOT read matrix position.
static void* __cdecl getDimensions(O* o, void* v) {
    for (size_t i=0;i<12;i+=4) store(v,i,load<uint32_t>(o->payload,i));
    return v;
}
// 006b2180 passes bounding sphere radius's float representation in a dword.
static O* __cdecl setSphere(O* o, uint32_t radius) {
    resetPrimitive(o,reinterpret_cast<D*>(CSF_ADDR_DATA(0x00884EF8)));
    store(o->payload,12,radius); return o;
}
// 00700630/00700250: primitive radius, x87 float load returned as double.
static double __cdecl getRadius(O* o) { return load<float>(o->payload,12); }
// 006b2240 passes radius then axial half-length: capsule, not 2D size.
static O* __cdecl setCapsule(O* o, uint32_t radius, uint32_t halfLength) {
    resetPrimitive(o,reinterpret_cast<D*>(CSF_ADDR_DATA(0x00884F88)));
    store(o->payload,0,halfLength); store<uint32_t>(o->payload,4,0);
    store<uint32_t>(o->payload,8,0); store(o->payload,12,radius); return o;
}

// 006d6280/006d6fc0 pass configuration from renderHandle->configuration.
static void* __cdecl setVector(void* c, const void* v) {
    for (size_t i=0;i<12;i+=4) store(c,76+i,load<uint32_t>(v,i));
    return c;
}
// All six setters share the SAME visibility/workspace configuration, NOT
// an Image or ToonInk. 006d6cd0 sets integer update budgets 300/600, 200/400.
static void* __cdecl setBudget8(void* c, uint32_t v) { store(c,8,v); return c; }
static void* __cdecl setBudget4(void* c, uint32_t v) { store(c,4,v); return c; }
static void* __cdecl setConfig12(void* c, float v) { store(c,12,v); return c; }
static void* __cdecl setConfig20(void* c, float v) { store(c,20,v); return c; }
static void* __cdecl setConfig16(void* c, float v) { store(c,16,v); return c; }
static void* __cdecl setConfig24(void* c, float v) { store(c,24,v); return c; }

// 006d6760 releases the ancillary handle. Internal configuration has two
// separately aligned arenas; workspace is itself an aligned allocation.
static int __cdecl destroyHandle(H* h) {
    alignedRelease(load<void*>(h->configuration,1036));
    alignedRelease(load<void*>(h->configuration,1048));
    release(h->configuration);
    alignedRelease(h->workspace);
    release(h->entries); release(h->scratch);
    h->count = 0;
    release(h->nodes); release(h);
    return 1;
}
HOOK(0x007E4970,noOperation,"boundary-rw");
HOOK(0x007F7900,destroyObject,"boundary-rw");
HOOK(0x007F7BF0,dispatchOutput,"boundary-rw");
HOOK(0x007F8250,setTransform,"boundary-rw");
HOOK(0x007F8320,setCallback,"boundary-rw");
HOOK(0x007F8330,getCallback,"boundary-rw");
HOOK(0x007F8340,setOffset,"boundary-rw");
HOOK(0x007F8390,setMultiplier,"boundary-rw");
HOOK(0x007F83E0,setBox,"boundary-rw");
HOOK(0x007F8410,getDimensions,"boundary-rw");
HOOK(0x007F9860,setSphere,"boundary-rw");
HOOK(0x007F9880,getRadius,"boundary-rw");
HOOK(0x007F9B40,setCapsule,"boundary-rw");
HOOK(0x007FAFE0,setVector,"boundary-rw");
HOOK(0x007FB000,setBudget8,"boundary-rw");
HOOK(0x007FB010,setBudget4,"boundary-rw");
HOOK(0x007FB020,setConfig12,"boundary-rw");
HOOK(0x007FB030,setConfig20,"boundary-rw");
HOOK(0x007FB040,setConfig16,"boundary-rw");
HOOK(0x007FB050,setConfig24,"boundary-rw");
HOOK(0x007FD9C0,destroyHandle,"boundary-rw");
HOOK(0x007FF0E0,createVisibilityProxy,"boundary-rw");
HOOK(0x007FF120,releaseVisibilityProxy,"boundary-rw");
HOOK(0x007FDA30,dictAdd,"boundary-rw");
HOOK(0x007FDA80,dictRemove,"boundary-rw");
#endif
