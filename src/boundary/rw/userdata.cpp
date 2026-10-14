// Borrow native UserData payloads; the facade extension is never their owner.
#ifdef CSF_STANDALONE
#include "globals.h"
#ifdef CSF_RW_NATIVE_TEST
#define HOOK(addr, fn, module) template <> void* const ::csf::fn_slot<(addr)>::p = (void*)&fn
#else
#include "hook.h"
#endif
#include "object.h"
#include <cstring>
namespace {
using namespace csf_rw;
using Array=rw::UserDataArray;
using Extension=rw::UserDataExtension;
// On x86 these native POD payload headers have the exact game layout: count,
// array pointer; each array has name, type, count, data. No owning RW header
// crosses the boundary. The native object destroys name/data/array allocations.
static_assert(sizeof(void*)!=4 || (sizeof(Array)==16 && sizeof(Extension)==8));
template<class A,class N> Extension* extension(A* a,N* n,Registry registry) {
    int offset=plugins(registry).getPluginOffset(rw::ID_USERDATA);
    if(!a || !n || offset<0) return nullptr;
    auto* e=Extension::get(n);
    std::memcpy(reinterpret_cast<char*>(a)+offset,e,sizeof(*e));
    return e;
}
int activeCount(Extension* e) {
    int count=0;if(e) for(int i=0;i<e->numArrays;++i) if(e->arrays[i].data) ++count;
    return count;
}
Array* arrayAt(Extension* e,int i) { return e && i>=0 && i<e->numArrays?e->get(i):nullptr; }
void removeArray(Extension* e,int i) {
    if(!arrayAt(e,i)) return;
    // Original removal compacts but leaves the allocated capacity unchanged.
    // librw frees the payload and leaves a hole; bridge that difference using
    // its remove primitive, then move the remaining POD ownership headers.
    e->remove(i);
    for(int j=i+1;j<e->numArrays;++j) e->arrays[j-1]=e->arrays[j];
    e->arrays[e->numArrays-1]={};
}
Extension* geometryData(Geometry* g) { return extension(g,geometryNative(g),Registry::Geometry); }
Extension* materialData(Material* m) { return extension(m,materialNative(m),Registry::Material); }
Extension* frameData(Frame* f) { return extension(f,frameNative(f),Registry::Frame); }
Geometry* __cdecl geometryRemove(Geometry* g,int i) { removeArray(geometryData(g),i);return g; }
int __cdecl geometryCount(Geometry* g) { return activeCount(geometryData(g)); }
Array* __cdecl geometryGet(Geometry* g,int i) { return arrayAt(geometryData(g),i); }
Frame* __cdecl frameRemove(Frame* f,int i) { removeArray(frameData(f),i);return f; }
int __cdecl frameCount(Frame* f) { return activeCount(frameData(f)); }
Array* __cdecl frameGet(Frame* f,int i) { return arrayAt(frameData(f),i); }
Material* __cdecl materialRemove(Material* m,int i) { removeArray(materialData(m),i);return m; }
int __cdecl materialCount(Material* m) { return activeCount(materialData(m)); }
Array* __cdecl materialGet(Material* m,int i) { return arrayAt(materialData(m),i); }
// 58BE40 is FLD DWORD then RET: the callers declare/promote the ST0 value as
// double. Not an integer register return and not a float-pointer getter.
double __cdecl getReal(Array* a,int i) { return a->getFloat(i); }
int __cdecl getInt(Array* a,int i) { return a->getInt(i); }
}
HOOK(0x0058BCC0,geometryRemove,"boundary-rw");
HOOK(0x0058BCE0,geometryCount,"boundary-rw");
HOOK(0x0058BD10,geometryGet,"boundary-rw");
HOOK(0x0058BD40,frameRemove,"boundary-rw");
// Names in the inventory are wrong: BD60/BD90 use 8B12A4 (Frame),
// BDC0/BDE0/BE10 use 8B12BC (Material). UserAttach proves the registries.
HOOK(0x0058BD60,frameCount,"boundary-rw");
HOOK(0x0058BD90,frameGet,"boundary-rw");
HOOK(0x0058BDC0,materialRemove,"boundary-rw");
HOOK(0x0058BDE0,materialCount,"boundary-rw");
HOOK(0x0058BE10,materialGet,"boundary-rw");
HOOK(0x0058BE40,getReal,"boundary-rw");
HOOK(0x0058BE50,getInt,"boundary-rw");
#endif
