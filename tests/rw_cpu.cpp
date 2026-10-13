#include "globals.h"
#include "boundary/rw/abi.h"
#include <cassert>
#include <cmath>
#include <cstdio>
using M = rw::Matrix;
using V = rw::V3d;
using F = csf_rw::Frame;
CSF_FUNC(M*, __stdcall, createM, 0x00624D40, ());
CSF_FUNC(int, __cdecl, destroyM, 0x00624D10, (M*));
CSF_FUNC(M*, __cdecl, translateM, 0x00624B30, (M*, const V*, rw::CombineOp));
CSF_FUNC(M*, __cdecl, multiplyM, 0x00624630, (M*, const M*, const M*));
CSF_FUNC(M*, __cdecl, invertM, 0x00624950, (M*, const M*));
CSF_FUNC(M*, __cdecl, orthoM, 0x006246A0, (M*, const M*));
CSF_FUNC(M*, __cdecl, rotateCS, 0x006246C0, (M*, const V*, float, float, rw::CombineOp));
CSF_FUNC(M*, __cdecl, rotateM, 0x006248B0, (M*, const V*, float, rw::CombineOp));
CSF_FUNC(M*, __cdecl, scaleM, 0x006249A0, (M*, const V*, rw::CombineOp));
CSF_FUNC(M*, __cdecl, transformM, 0x00624C70, (M*, const M*, rw::CombineOp));
CSF_FUNC(M*, __cdecl, optimizeM, 0x00624560, (M*, M::Tolerance*));
CSF_FUNC(M*, __cdecl, updateM, 0x00624620, (M*));
CSF_FUNC(V*, __cdecl, point, 0x00623010, (V*, const V*, const M*));
CSF_FUNC(V*, __cdecl, vector, 0x00623070, (V*, const V*, const M*));
CSF_FUNC(V*, __cdecl, points, 0x00623040, (V*, const V*, int, const M*));
CSF_FUNC(V*, __cdecl, vectors, 0x006230A0, (V*, const V*, int, const M*));
CSF_FUNC(double, __cdecl, normalize, 0x00623250, (V*, const V*));
CSF_FUNC(double, __cdecl, sqrtRW, 0x00622EE0, (float));
CSF_FUNC(double, __cdecl, invSqrtRW, 0x00622F40, (float));
CSF_FUNC(double, __cdecl, length2, 0x00622FA0, (const rw::V2d*));
CSF_FUNC(double, __cdecl, length3, 0x00623360, (const V*));
CSF_FUNC(F*, __stdcall, createF, 0x006235D0, ());
CSF_FUNC(int, __cdecl, destroyF, 0x00623B10, (F*));
CSF_FUNC(F*, __cdecl, addF, 0x00623B90, (F*, F*));
CSF_FUNC(F*, __cdecl, removeF, 0x00623750, (F*));
CSF_FUNC(F*, __cdecl, rootF, 0x00623740, (F*));
CSF_FUNC(M*, __cdecl, ltmF, 0x00623720, (F*));
CSF_FUNC(F*, __cdecl, updateF, 0x006236A0, (F*));
CSF_FUNC(F*, __cdecl, childrenF, 0x006237C0, (F*, F* (__cdecl *)(F*, void*), void*));
CSF_FUNC(F*, __cdecl, objectsF, 0x00623910, (F*, void* (__cdecl *)(void*, void*), void*));
CSF_FUNC(F*, __cdecl, translateF, 0x00623830, (F*, const V*, rw::CombineOp));
CSF_FUNC(F*, __cdecl, transformF, 0x00623860, (F*, const M*, rw::CombineOp));
CSF_FUNC(F*, __cdecl, rotateF, 0x00623890, (F*, const V*, float, rw::CombineOp));
CSF_FUNC(F*, __cdecl, identityF, 0x006238C0, (F*));
CSF_FUNC(int, __stdcall, syncF, 0x006327D0, ());
CSF_FUNC(int, __stdcall, purge, 0x006265F0, ());
static bool near(float x, float y) { return std::fabs(x-y) < 0.0001f; }
static F* __cdecl stop(F* f, void* data) { ++*static_cast<int*>(data); return nullptr; }
static void* __cdecl stopObj(void* p, void* data) { assert(p); ++*static_cast<int*>(data); return nullptr; }
static int syncCount;
static void __cdecl onSync(csf_rw::ObjectWithFrame*) { ++syncCount; }
static F* syncOrder[3];
static int syncOrderCount;
static void __cdecl orderedSync(csf_rw::ObjectWithFrame* o) {
    assert(syncOrderCount < 3);
    syncOrder[syncOrderCount++] = static_cast<F*>(o->object.parent);
}
static void attachObject(csf_rw::ObjectWithFrame& o, F* f) {
    o.object.parent = f; o.sync = orderedSync;
    o.inFrame.next = o.inFrame.prev = &f->objects;
    f->objects.next = f->objects.prev = &o.inFrame;
}
static void* __cdecl failAlloc(size_t, rw::uint32) { return nullptr; }
int main() {
    // Every slot in the slice is a real link reference, not just an object compile.
    const uintptr_t slots[] = {
        CSF_ADDR_FN(0x00622EE0), CSF_ADDR_FN(0x00622F40), CSF_ADDR_FN(0x00622FA0),
        CSF_ADDR_FN(0x00623010), CSF_ADDR_FN(0x00623040), CSF_ADDR_FN(0x00623070), CSF_ADDR_FN(0x006230A0),
        CSF_ADDR_FN(0x00623250), CSF_ADDR_FN(0x00623360), CSF_ADDR_FN(0x006235D0), CSF_ADDR_FN(0x006236A0),
        CSF_ADDR_FN(0x00623720), CSF_ADDR_FN(0x00623740), CSF_ADDR_FN(0x00623750), CSF_ADDR_FN(0x006237C0),
        CSF_ADDR_FN(0x00623830), CSF_ADDR_FN(0x00623860), CSF_ADDR_FN(0x00623890), CSF_ADDR_FN(0x006238C0),
        CSF_ADDR_FN(0x00623910), CSF_ADDR_FN(0x00623B10), CSF_ADDR_FN(0x00623B90), CSF_ADDR_FN(0x00624560),
        CSF_ADDR_FN(0x00624620), CSF_ADDR_FN(0x00624630), CSF_ADDR_FN(0x006248B0), CSF_ADDR_FN(0x00624950),
        CSF_ADDR_FN(0x006246A0), CSF_ADDR_FN(0x006246C0),
        CSF_ADDR_FN(0x006249A0), CSF_ADDR_FN(0x00624B30), CSF_ADDR_FN(0x00624C70), CSF_ADDR_FN(0x00624D10),
        CSF_ADDR_FN(0x00624D40), CSF_ADDR_FN(0x006265F0), CSF_ADDR_FN(0x006327D0)
    };
    for (auto s : slots) assert(s != 0);
    std::puts("rw CPU: matrices"); std::fflush(stdout);
    M* m = createM(); assert(m && m->flags == 0x20003);
    assert(sqrtRW(9) == 3 && sqrtRW(0) == 0 && invSqrtRW(0) == 0);
    assert(near(float(invSqrtRW(4)),0.5f));
    rw::V2d v2{3,4}; assert(length2(&v2) == 5);
    V t{2,3,4}, v{1,2,3}, p;
    assert(translateM(m, &t, rw::COMBINEPOSTCONCAT) == m);
    assert(point(&p, &v, m) == &p && near(p.x,3) && near(p.y,5) && near(p.z,7));
    vector(&p, &v, m); assert(near(p.x,1) && near(p.y,2) && near(p.z,3));
    V batch[2] = {v, v}, result[2];
    assert(points(result,batch,2,m) == result && near(result[1].z,7));
    assert(vectors(result,batch,2,m) == result && near(result[1].z,3));
    assert(points(result,batch,0,m) == result);
    M inv, product; invertM(&inv, m); multiplyM(&product, m, &inv);
    assert(near(product.pos.x,0) && near(product.right.x,1));
    M scaled = *m; scaled.right.x = 2; scaled.up.x = 1;
    assert(orthoM(&scaled, &scaled) == &scaled && near(scaled.right.x,1) && near(scaled.up.x,0));
    V axis{0,0,1}; rotateCS(&scaled, &axis, 1, 1, rw::COMBINEREPLACE);
    assert(near(scaled.right.x,0) && near(scaled.right.y,1));
    rotateM(&scaled, &axis, 90, rw::COMBINEREPLACE); assert(near(scaled.right.y,1));
    V scl{2,3,4}; scaleM(&scaled, &scl, rw::COMBINEREPLACE);
    assert(near(scaled.right.x,2) && near(scaled.up.y,3));
    transformM(&scaled,m,rw::COMBINEREPLACE); assert(near(scaled.pos.z,4));
    updateM(&scaled); assert((scaled.flags & (unsigned(M::TYPEMASK)|unsigned(M::IDENTITY))) == 0);
    optimizeM(&scaled,nullptr); assert((scaled.flags & M::TYPEMASK) == M::TYPEORTHONORMAL);
    V zero{}; assert(normalize(&p, &zero) == 0 && p.x == 0);
    V nonzero{3,0,4}; assert(length3(&nonzero) == 5);
    assert(near(float(normalize(&p, &nonzero)),5) && near(p.z,0.8f));
    destroyM(m);
    std::puts("rw CPU: frames"); std::fflush(stdout);
    F* a = createF(); F* b = createF(); F* c = createF(); assert(a && b && c);
    assert(a->object.type == rw::Frame::ID && a->object.subType == 0);
    auto savedMalloc = rw::Engine::memfuncs.rwmalloc;
    rw::Engine::memfuncs.rwmalloc = failAlloc;
    assert(createM() == nullptr && createF() == nullptr);
    rw::Engine::memfuncs.rwmalloc = savedMalloc;
    assert(addF(a,b) == a); addF(a,c); assert(rootF(b) == a && b->object.parent == a);
    F* d = createF(); F* e = createF(); assert(d && e);
    addF(d,e); addF(b,d);
    assert(rootF(d) == a && rootF(e) == a);
    a->matrix.pos = V{10,0,0}; a->matrix.update(); updateF(a);
    b->matrix.pos = V{0,5,0}; b->matrix.update(); updateF(b);
    M* ltm = ltmF(b); assert(ltm == &b->ltm && near(ltm->pos.x,10) && near(ltm->pos.y,5));
    assert(near(ltmF(e)->pos.x,10) && near(ltmF(e)->pos.y,5));
    destroyF(e); destroyF(d);
    assert(translateF(c,&t,rw::COMBINEREPLACE) == c && near(c->matrix.pos.z,4));
    M identity; identity.setIdentity(); transformF(c,&identity,rw::COMBINEREPLACE);
    rotateF(c,&axis,90,rw::COMBINEPOSTCONCAT); assert(near(c->matrix.right.y,1));
    assert(identityF(c) == c && near(c->matrix.right.x,1) && c->matrix.pos.x == 0);
    int count = 0; childrenF(a, stop, &count); assert(count == 1);
    csf_rw::ObjectWithFrame obj = {};
    obj.object.parent = b; obj.sync = onSync;
    obj.inFrame.next = obj.inFrame.prev = &b->objects;
    b->objects.next = b->objects.prev = &obj.inFrame;
    int objectCount = 0; assert(objectsF(b,stopObj,&objectCount) == b && objectCount == 1);
    assert(removeF(b) == b && b->root == b && b->object.parent == nullptr);
    updateF(c); assert(syncF() == 1); assert(purge() == 0);
    assert(syncCount == 1); syncF(); assert(syncCount == 1);
    destroyF(b); destroyF(a);
    assert(obj.object.parent == nullptr);
    assert(c->object.parent == nullptr && c->root == c);
    assert(ltmF(c) == &c->ltm);
    destroyF(c);
    // Objects must synchronize in hierarchy order, not unordered_map order.
    F* r = createF(); F* left = createF(); F* right = createF();
    assert(r && left && right); addF(r,right); addF(r,left);
    csf_rw::ObjectWithFrame orderObjects[3] = {};
    attachObject(orderObjects[0],r); attachObject(orderObjects[1],left);
    attachObject(orderObjects[2],right);
    updateF(r); syncF();
    assert(syncOrderCount == 3 && syncOrder[0] == r && syncOrder[1] == left && syncOrder[2] == right);
    destroyF(left); destroyF(right); destroyF(r);
    std::puts("rw CPU: 36 slots linked; vector/matrix/frame tests passed");
}
