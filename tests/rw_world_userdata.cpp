#include "globals.h"
#include "boundary/rw/object.h"
#include <cassert>
#include <cstring>
#include <cstdio>
using namespace csf_rw;
using A=rw::UserDataArray;
CSF_FUNC(int,__cdecl,init,0x00627360,(const MemoryFunctions*,char,int));
CSF_FUNC(int,__stdcall,attachUser,0x0058C040,());
CSF_FUNC(int,__cdecl,open,0x00627290,(void*));
CSF_FUNC(int,__stdcall,start,0x00626D60,());
CSF_FUNC(int,__stdcall,stop,0x00626CE0,());
CSF_FUNC(int,__stdcall,close,0x00626E30,());
CSF_FUNC(int,__stdcall,term,0x00626EC0,());
CSF_FUNC(Geometry*,__cdecl,removeG,0x0058BCC0,(Geometry*,int));
CSF_FUNC(int,__cdecl,countG,0x0058BCE0,(Geometry*));
CSF_FUNC(A*,__cdecl,getG,0x0058BD10,(Geometry*,int));
CSF_FUNC(Frame*,__cdecl,removeF,0x0058BD40,(Frame*,int));
CSF_FUNC(int,__cdecl,countF,0x0058BD60,(Frame*));
CSF_FUNC(A*,__cdecl,getF,0x0058BD90,(Frame*,int));
CSF_FUNC(Material*,__cdecl,removeM,0x0058BDC0,(Material*,int));
CSF_FUNC(int,__cdecl,countM,0x0058BDE0,(Material*));
CSF_FUNC(A*,__cdecl,getM,0x0058BE10,(Material*,int));
CSF_FUNC(double,__cdecl,realValue,0x0058BE40,(A*,int));
CSF_FUNC(int,__cdecl,intValue,0x0058BE50,(A*,int));
CSF_FUNC(const char*,__cdecl,arrayName,0x005870B0,(A*));
CSF_FUNC(Frame*,__stdcall,newFrame,0x006235D0,());
CSF_FUNC(int,__cdecl,deleteFrame,0x00623B10,(Frame*));
CSF_FUNC(Texture*,__cdecl,newTexture,0x00631780,(Raster*));
CSF_FUNC(int,__cdecl,deleteTexture,0x006317E0,(Texture*));
CSF_FUNC(int,__cdecl,deleteGeometry,0x00590FC0,(Geometry*));
CSF_FUNC(Clump*,__cdecl,cloneClump,0x0058FC60,(Clump*));
CSF_FUNC(int,__cdecl,deleteClump,0x0058F740,(Clump*));
template<class T> static void checkMirror(T* object,Registry r,rw::UserDataExtension* expected) {
    rw::UserDataExtension actual{};
    std::memcpy(&actual,reinterpret_cast<char*>(object)+plugins(r).getPluginOffset(rw::ID_USERDATA),sizeof(actual));
    assert(actual.numArrays==expected->numArrays && actual.arrays==expected->arrays);
}
static void seedEmpty(rw::UserDataExtension* e) {
    // Unmodified librw add() uses memcpy(dst,nullptr,0) on the first add.
    // Avoid that upstream nonnull-attribute UB in the synthetic fixture without
    // suppressing UBSan or changing upstream: give its zero-length source a
    // valid allocation. add() frees this allocation when it grows the array.
    assert(e->numArrays==0 && !e->arrays);
    e->arrays=static_cast<A*>(rw::Engine::memfuncs.rwmalloc(sizeof(A),0));assert(e->arrays);
}
int main() {
    assert(init(nullptr,0,0));assert(attachUser());assert(open(nullptr));assert(start());
    auto* g=geometryCreate(0,0,rw::Geometry::POSITIONS);auto* n=geometryNative(g);
    assert(g && countG(g)==0 && !getG(g,0));seedEmpty(rw::UserDataExtension::get(n));
    assert(A::geometryAdd(n,"first",rw::USERDATAINT,2)==0);
    assert(A::geometryAdd(n,"second",rw::USERDATAFLOAT,1)==1);
    assert(A::geometryAdd(n,"third",rw::USERDATAINT,1)==2);
    A::geometryGet(n,0)->setInt(0,17);A::geometryGet(n,0)->setInt(1,-29);
    A::geometryGet(n,1)->setFloat(0,1.25f);A::geometryGet(n,2)->setInt(0,99);
    assert(countG(g)==3 && intValue(getG(g,0),1)==-29 && realValue(getG(g,1),0)==1.25);
    assert(arrayName(getG(g,0))==getG(g,0)->name);
    checkMirror(g,Registry::Geometry,rw::UserDataExtension::get(n));
    assert(removeG(g,0)==g && countG(g)==2);
    assert(!std::strcmp(getG(g,0)->name,"second") && intValue(getG(g,1),0)==99);
    assert(getG(g,2) && !getG(g,2)->data); // capacity unchanged, compacted empty tail
    assert(removeG(g,1)==g && countG(g)==1);assert(removeG(g,0)==g && countG(g)==0);
    assert(!getG(g,3) && !getG(g,-1));assert(deleteGeometry(g));
    auto* f=newFrame();assert(f);auto* nf=frameNative(f);seedEmpty(rw::UserDataExtension::get(nf));
    assert(countF(f)==0 && !getF(f,0));assert(A::frameAdd(nf,"text",rw::USERDATASTRING,1)==0);
    A::frameGet(nf,0)->setString(0,"owned");
    assert(countF(f)==1 && !std::strcmp(getF(f,0)->getString(0),"owned"));
    auto* c=clumpCreate();clumpSetFrame(c,f);assert(!cloneClump(c));
    assert(!std::strcmp(A::frameGet(nf,0)->getString(0),"owned"));
    assert(removeF(f,0)==f);
    assert(countF(f)==0 && !getF(f,1));
    assert(!A::frameGet(nf,0)->data);checkMirror(f,Registry::Frame,rw::UserDataExtension::get(nf));assert(deleteClump(c));
    auto* m=materialCreate();auto* nm=materialNative(m);assert(m && countM(m)==0);seedEmpty(rw::UserDataExtension::get(nm));
    assert(A::materialAdd(nm,"int",rw::USERDATAINT,1)==0);A::materialGet(nm,0)->setInt(0,1234);
    assert(countM(m)==1 && intValue(getM(m,0),0)==1234 && !getM(m,1));
    checkMirror(m,Registry::Material,rw::UserDataExtension::get(nm));
    assert(removeM(m,0)==m && countM(m)==0);assert(materialDestroy(m));
    assert(objectsEmpty() && framesEmpty());assert(stop() && close() && term());
    std::puts("rw user-data slots passed");
}
