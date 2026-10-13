// Standalone only: ABI adaptation to the MIT librw checkout, not RW library hooks.
#ifdef CSF_STANDALONE
#include "globals.h"
#ifdef CSF_RW_NATIVE_TEST
// The hook header contains x86-only naked entry stubs; a native CPU test needs
// only the identical standalone address-slot definition, not those entry stubs.
#define HOOK(addr, fn, module) template <> void* const ::csf::fn_slot<(addr)>::p = (void*)&fn
#else
#include "hook.h"
#endif
#include "abi.h"
#include <cstdlib>
#include <unordered_map>
#include <vector>
#include <new>
#include <cmath>

namespace {
using csf_rw::Frame;
using M = rw::Matrix;
using V = rw::V3d;
using Op = rw::CombineOp;
using FrameCB = Frame* (__cdecl *)(Frame*, void*);
using ObjectCB = void* (__cdecl *)(void*, void*);

// NULL backend has no device. Lifecycle slots in the next slice must replace this
// bootstrap with the game's explicit Init/Open/Close/Term sequence.
bool cpuEngine() {
    if (rw::Engine::state == rw::Engine::Started) return true;
    if (rw::Engine::state == rw::Engine::Dead && !rw::Engine::init()) return false;
    if (rw::Engine::state == rw::Engine::Initialized && !rw::Engine::open(nullptr)) return false;
    return rw::Engine::start() != 0;
}
void allocator() {
    if (rw::Engine::state == rw::Engine::Dead)
        rw::Engine::memfuncs = rw::defaultMemfuncs;
}
std::unordered_map<Frame*, rw::Frame*>& frames() {
    static std::unordered_map<Frame*, rw::Frame*> map;
    return map;
}
std::unordered_map<rw::Frame*, Frame*>& facades() {
    static std::unordered_map<rw::Frame*, Frame*> map;
    return map;
}
rw::Frame* native(Frame* f) { return frames().at(f); }
Frame* facade(rw::Frame* f) {
    if (!f) return nullptr;
    auto it = facades().find(f);
    return it == facades().end() ? nullptr : it->second;
}
void exportFrames() {
    for (auto& e : frames()) {
        Frame* a = e.first; rw::Frame* b = e.second;
        a->object.privateFlags = b->object.privateFlags;
        a->object.parent = facade(b->getParent());
        a->matrix = b->matrix; a->ltm = b->ltm;
        a->child = facade(b->child); a->next = facade(b->next); a->root = facade(b->root);
    }
}
// Direct matrix writes are authoritative in the game-visible facade. They become
// dirty only on UpdateObjects, like the game API; GetLTM does not invent dirtiness.
void importFrames() {
    for (auto& e : frames()) e.second->matrix = e.first->matrix;
}
static Frame* __stdcall frameCreate() {
    if (!cpuEngine()) return nullptr;
    auto* a = static_cast<Frame*>(std::calloc(1, sizeof(Frame)));
    if (!a) return nullptr;
    rw::Frame* b = rw::Frame::create();
    if (!b) { std::free(a); return nullptr; }
    try { frames().emplace(a, b); facades().emplace(b, a); }
    catch (...) { frames().erase(a); b->destroy(); std::free(a); return nullptr; }
    a->object.type = b->object.type;
    a->object.subType = b->object.subType;
    a->object.flags = b->object.flags;
    a->objects.next = a->objects.prev = &a->objects;
    a->dirty.next = a->dirty.prev = &a->dirty;
    exportFrames();
    return a;
}
static Frame* __cdecl frameUpdate(Frame* f) {
    importFrames(); native(f)->updateObjects(); exportFrames(); return f;
}
static M* __cdecl frameLTM(Frame* f) {
    importFrames(); native(f)->getLTM(); exportFrames(); return &f->ltm;
}
static Frame* __cdecl frameRoot(Frame* f) { return f->root; }
static Frame* __cdecl frameRemove(Frame* f) {
    importFrames(); native(f)->removeChild(); exportFrames(); return f;
}
static Frame* __cdecl frameAdd(Frame* f, Frame* child) {
    importFrames(); native(f)->addChild(native(child));
    // Upstream addChild assigns the parent's root to the child, but assigns
    // the parent itself to deeper descendants. Repair this when the parent
    // is not a root, without changing the checked-out library.
    native(child)->setHierarchyRoot(native(f)->root);
    exportFrames(); return f;
}
struct CallbackData { FrameCB cb; void* data; };
rw::Frame* childCB(rw::Frame* f, void* data) {
    auto* d = static_cast<CallbackData*>(data);
    return d->cb(facade(f), d->data) ? f : nullptr;
}
static Frame* __cdecl frameChildren(Frame* f, FrameCB cb, void* data) {
    CallbackData d{cb, data};
    native(f)->forAllChildren(childCB, &d); return f;
}
static Frame* __cdecl frameObjects(Frame* f, ObjectCB cb, void* data) {
    for (auto* l = f->objects.next; l != &f->objects;) {
        auto* next = l->next;
        auto* obj = reinterpret_cast<char*>(l) - offsetof(csf_rw::ObjectWithFrame, inFrame);
        if (!cb(obj, data)) break;
        l = next;
    }
    return f;
}
static Frame* __cdecl frameTranslate(Frame* f, const V* v, Op op) {
    importFrames(); native(f)->translate(v, op); exportFrames(); return f;
}
static Frame* __cdecl frameTransform(Frame* f, const M* m, Op op) {
    importFrames(); native(f)->transform(m, op); exportFrames(); return f;
}
static Frame* __cdecl frameRotate(Frame* f, const V* v, float angle, Op op) {
    importFrames(); native(f)->rotate(v, angle, op); exportFrames(); return f;
}
static Frame* __cdecl frameIdentity(Frame* f) {
    importFrames(); native(f)->matrix.setIdentity(); native(f)->updateObjects(); exportFrames(); return f;
}
static int __cdecl frameDestroy(Frame* f) {
    // Attached game objects use the facade list. Detach without destroying them.
    while (f->objects.next != &f->objects) {
        auto* l = f->objects.next;
        auto* obj = reinterpret_cast<csf_rw::ObjectWithFrame*>(
            reinterpret_cast<char*>(l) - offsetof(csf_rw::ObjectWithFrame, inFrame));
        l->prev->next = l->next; l->next->prev = l->prev;
        obj->object.parent = nullptr;
    }
    // librw destroy clears children's parent pointers but doesn't repair their
    // root pointers. Detach through the hierarchy API before freeing the root.
    rw::Frame* b = native(f);
    while (b->child) b->child->removeChild();
    b->destroy(); frames().erase(f); facades().erase(b); std::free(f); exportFrames(); return 1;
}
void collectSyncFrames(rw::Frame* f, std::vector<Frame*>& order) {
    order.push_back(facade(f));
    for (auto* child = f->child; child; child = child->next)
        collectSyncFrames(child, order);
}
static int __stdcall frameSync() {
    if (!cpuEngine()) return 0;
    importFrames();
    std::vector<Frame*> dirty;
    // Preserve librw's dirty-root list and child preorder, not hash-map order:
    // callbacks may observe objects previously synchronized in this pass.
    FORLIST(link, rw::engine->frameDirtyList) {
        auto* root = LLLinkGetData(link, rw::Frame, inDirtyList);
        collectSyncFrames(root, dirty);
    }
    rw::Frame::syncDirty(); exportFrames();
    // No native ObjectWithFrame sidecars yet. Sync callbacks see facade pointers.
    for (Frame* f : dirty) {
        if (!frames().contains(f)) continue;
        frameObjects(f, [](void* p, void*) -> void* {
            auto* o = static_cast<csf_rw::ObjectWithFrame*>(p);
            if (o->sync) o->sync(o);
            return p;
        }, nullptr);
    }
    return 1;
}
static double __cdecl sqrtRW(float x) { return std::sqrt(x); }
static double __cdecl invSqrtRW(float x) { return x == 0 ? 0 : 1.0 / std::sqrt(x); }
static double __cdecl length2(const rw::V2d* v) { return rw::length(*v); }
static double __cdecl length3(const V* v) { return rw::length(*v); }
static double __cdecl normalize3(V* out, const V* v) {
    float len = rw::length(*v);
    *out = len == 0 ? rw::makeV3d(0, 0, 0) : rw::scale(*v, 1.0f / len);
    return len;
}
static V* __cdecl points(V* out, const V* in, int n, const M* m) {
    rw::V3d::transformPoints(out, in, n, m); return out;
}
static V* __cdecl vectors(V* out, const V* in, int n, const M* m) {
    rw::V3d::transformVectors(out, in, n, m); return out;
}
static V* __cdecl point(V* out, const V* in, const M* m) { return points(out, in, 1, m); }
static V* __cdecl vector(V* out, const V* in, const M* m) { return vectors(out, in, 1, m); }
static M* __stdcall matrixCreate() {
    allocator();
    // librw Matrix::create dereferences an allocation failure. Use its allocator
    // and initializer separately to preserve the boundary's nullable result.
    auto* m = static_cast<M*>(rw::malloc_LOC(sizeof(M), rw::MEMDUR_EVENT | rw::ID_MATRIX, "CSF matrix boundary"));
    if (m) m->setIdentity();
    return m;
}
static int __cdecl matrixDestroy(M* m) { m->destroy(); return 1; }
static M* __cdecl matrixOptimize(M* m, M::Tolerance* t) { m->optimize(t); return m; }
static M* __cdecl matrixUpdate(M* m) { m->update(); return m; }
static M* __cdecl matrixMultiply(M* out, const M* a, const M* b) { return M::mult(out, a, b); }
static M* __cdecl matrixInvert(M* out, const M* m) { return M::invert(out, m); }
static M* __cdecl matrixRotate(M* m, const V* v, float a, Op op) { return m->rotate(v, a, op); }
static M* __cdecl matrixScale(M* m, const V* v, Op op) { return m->scale(v, op); }
static M* __cdecl matrixTranslate(M* m, const V* v, Op op) { return m->translate(v, op); }
static M* __cdecl matrixTransform(M* m, const M* other, Op op) { return m->transform(other, op); }
// Two CPU APIs absent from librw's public Matrix interface. Adapt with librw
// vector primitives / the Rodrigues representation used by Matrix::makeRotation.
// These are boundary operations, not translations of original library code.
static M* __cdecl matrixOrthoNormalize(M* out, const M* in) {
    M copy = *in;
    copy.right = rw::normalize(copy.right);
    copy.at = rw::normalize(rw::cross(copy.right, copy.up));
    copy.up = rw::cross(copy.at, copy.right);
    copy.flags = (copy.flags & ~M::TYPEMASK) | M::TYPEORTHONORMAL;
    *out = copy;
    return out;
}
static M* __cdecl matrixRotateCS(M* m, const V* axis, float t, float s, Op op) {
    // Unlike rotate(angle), this API receives an already normalized axis and
    // 1-cos(angle), sin(angle). Do not reconstruct the angle or normalize again.
    const V v = *axis;
    const float c = 1.0f - t;
    M rot = {};
    rot.right = rw::makeV3d(c+v.x*v.x*t, v.x*v.y*t+v.z*s, v.z*v.x*t-v.y*s);
    rot.up = rw::makeV3d(v.x*v.y*t-v.z*s, c+v.y*v.y*t, v.y*v.z*t+v.x*s);
    rot.at = rw::makeV3d(v.z*v.x*t+v.y*s, v.y*v.z*t-v.x*s, c+v.z*v.z*t);
    rot.flags = M::TYPEORTHONORMAL;
    return m->transform(&rot, op);
}
// librw allocates directly, so there are no spare freelist blocks to purge.
static int __stdcall purge() { return 0; }
}

HOOK(0x00622EE0, sqrtRW, "boundary-rw");
HOOK(0x00622F40, invSqrtRW, "boundary-rw");
HOOK(0x00622FA0, length2, "boundary-rw");
HOOK(0x00623010, point, "boundary-rw");
HOOK(0x00623040, points, "boundary-rw");
HOOK(0x00623070, vector, "boundary-rw");
HOOK(0x006230A0, vectors, "boundary-rw");
HOOK(0x00623250, normalize3, "boundary-rw");
HOOK(0x00623360, length3, "boundary-rw");
HOOK(0x006235D0, frameCreate, "boundary-rw");
HOOK(0x006236A0, frameUpdate, "boundary-rw");
HOOK(0x00623720, frameLTM, "boundary-rw");
HOOK(0x00623740, frameRoot, "boundary-rw");
HOOK(0x00623750, frameRemove, "boundary-rw");
HOOK(0x006237C0, frameChildren, "boundary-rw");
HOOK(0x00623830, frameTranslate, "boundary-rw");
HOOK(0x00623860, frameTransform, "boundary-rw");
HOOK(0x00623890, frameRotate, "boundary-rw");
HOOK(0x006238C0, frameIdentity, "boundary-rw");
HOOK(0x00623910, frameObjects, "boundary-rw");
HOOK(0x00623B10, frameDestroy, "boundary-rw");
HOOK(0x00623B90, frameAdd, "boundary-rw");
HOOK(0x00624560, matrixOptimize, "boundary-rw");
HOOK(0x00624620, matrixUpdate, "boundary-rw");
HOOK(0x00624630, matrixMultiply, "boundary-rw");
HOOK(0x006246A0, matrixOrthoNormalize, "boundary-rw");
HOOK(0x006246C0, matrixRotateCS, "boundary-rw");
HOOK(0x006248B0, matrixRotate, "boundary-rw");
HOOK(0x00624950, matrixInvert, "boundary-rw");
HOOK(0x006249A0, matrixScale, "boundary-rw");
HOOK(0x00624B30, matrixTranslate, "boundary-rw");
HOOK(0x00624C70, matrixTransform, "boundary-rw");
HOOK(0x00624D10, matrixDestroy, "boundary-rw");
HOOK(0x00624D40, matrixCreate, "boundary-rw");
HOOK(0x006265F0, purge, "boundary-rw");
HOOK(0x006327D0, frameSync, "boundary-rw");
#endif
