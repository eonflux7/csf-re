// Contract-only physics boundary. These slots were labelled RpTeam/Rt2d in
// the database, but neither rendering toolkit has this body/force ABI.
using Body = csf_rw::PhysicsBody;
using Dict = csf_rw::PhysicsDictionary;
using Element = csf_rw::PhysicsElement;
CSF_GLOBAL(int, physicsActivationError, 0x008BE0D4);
CSF_GLOBAL(D, defaultPhysicsShape, 0x00885020);

static bool bodyBit(const uint32_t* bits, const Body* b) {
    return (bits[b->id >> 5] & (uint32_t(1) << (b->id & 31))) != 0;
}
static void bodyBitSet(uint32_t* bits, const Body* b, bool on) {
    auto mask = uint32_t(1) << (b->id & 31);
    if(on) bits[b->id >> 5] |= mask;
    else bits[b->id >> 5] &= ~mask;
}
static bool bodyActive(Body* b) { return b->owner && bodyBit(b->owner->active,b); }
static Dict* __cdecl enableBody(Dict* d,Body* b,int on) {
    bodyBitSet(d->enabled,b,on!=0); return d;
}
static Body* __cdecl activateBody(Dict* d,Body* b) {
    if(bodyActive(b)) return b;
    if(d->activeCount < d->activeLimit) {
        if(reinterpret_cast<int (__cdecl *)(Body*)>(b->methods[4])(b)) {
            bodyBitSet(d->active,b,true); ++d->activeCount;
            physicsActivationError=0; return b;
        }
        if(!physicsActivationError) physicsActivationError=1;
    }
    return nullptr;
}
static Body* __cdecl deactivateBody(Dict* d,Body* b) {
    if(bodyActive(b)) {
        reinterpret_cast<void (__cdecl *)(Body*)>(b->methods[5])(b);
        bodyBitSet(d->active,b,false); --d->activeCount;
    }
    return b;
}
static void* __cdecl bodyBounds(Dict* d,Body* b,void* output) {
    if(!bodyBit(d->bounded,b)) return nullptr;
    auto* spatial=static_cast<csf_rw::PhysicsSpatial*>(d->spatial);
    auto* bounds=spatial->storage+32*b->id;
    for(size_t i=0;i<3;++i) {
        store(output,4*i,load<float>(bounds,4*i)-spatial->margin);
        store(output,16+4*i,load<float>(bounds,16+4*i)+spatial->margin);
    }
    return output;
}
static Dict* __cdecl collisionMatrix(Dict* d,void* matrix) {
    static_cast<csf_rw::PhysicsSpatial*>(d->spatial)->collisionMatrix=matrix;return d;
}

static O* __cdecl defaultShape(O* o) {
    // The default shape initializer leaves payload +40..+4b and matrix
    // padding alone, unlike the primitive radius initializers above.
    o->matrix.right={1,0,0};o->matrix.up={0,1,0};
    o->matrix.at={0,0,1};o->matrix.pos={0,0,0};
    o->matrix.flags |= 0x20003;
    store<uint32_t>(o->payload,12,0);
    o->control0=o->control1=0;
    o->multiplier=o->offset=0x3e4ccccd;
    o->descriptor=&defaultPhysicsShape; return o;
}
static float* __cdecl setTimeStep(float* configuration,float dt) { *configuration=dt;return configuration; }
static void* __cdecl handleDictionary(H* h) { return h->workspace; }
// Shared second-word accessor: configuration pointer on handles, integer count
// on compound shapes. Preserve the untyped return word, including its width in
// native tests; production ABI is always one 32-bit word.
static uintptr_t __cdecl secondWord(void* p) { return load<uintptr_t>(p,sizeof(void*)); }
static O* __cdecl childAt(csf_rw::ToolkitChildren* children,uint32_t index) { return children->objects+index; }
static void* __cdecl bodyLinks(Body* b) { return b->links; }
static void* __cdecl proxyTarget(O* o) { return load<void*>(o->payload,0); }
static double __cdecl dimension64(O* o) { return load<float>(o->payload,0); }
static double __cdecl dimension68(O* o) { return load<float>(o->payload,4); }

static double __cdecl configureMass(csf_rw::PhysicsMass* body,float mass,int fullTensor) {
    O* shape=body->shape;
    csf_rw::PhysicsShapeProperties properties;
    rw::V3d center;
    auto callback=reinterpret_cast<double (__cdecl *)(O*,csf_rw::PhysicsShapeProperties*,rw::V3d*)>(shape->descriptor->methods[2]);
    float volume=float(callback(shape,&properties,&center));
    if(!(shape->descriptor->flags & 2)) {
        properties.rotation=rw::mult(properties.rotation,shape->matrix.getRotation());
        rw::V3d::transformPoints(&center,&center,1,&shape->matrix);
    }
    if(mass<0) mass=-mass*volume; // negative argument denotes density
    body->mass=mass;body->center=center;
    if(fullTensor) {
        body->inertia=mass>1.1754944e-38f?rw::scale(properties.inertia,mass):properties.inertia;
        body->rotation=properties.rotation;body->flags |= 2;
    } else if(mass>1.1754944e-38f) {
        float maximum=properties.inertia.x;
        if(properties.inertia.y>maximum) maximum=properties.inertia.y;
        if(properties.inertia.z>maximum) maximum=properties.inertia.z;
        body->sphericalInertia=maximum*mass;body->flags &= ~2u;
    }
    return volume>=1.1754944e-38f?double(mass)/volume:double(3.4028234663852886e38f);
}

static uint32_t* __cdecl createBitMatrix(uint32_t rows,uint32_t columns) {
    uint32_t words=(rows*columns+31u)>>5;
    auto alloc=load<void* (__cdecl *)(uint32_t,uint32_t,uint32_t)>(engine,69*sizeof(void*));
    auto p=static_cast<uint32_t*>(alloc(words+3,4,198912));
    if(!p) return nullptr;
    p[0]=rows;p[1]=columns;p[2]=words;return p;
}
static int __cdecl destroyBitMatrix(void* p) { release(p);return 1; }
static uint32_t* __cdecl initPhysicsParameters(uint32_t* p) {
    const uint32_t defaults[]={~0u,3,0,0,0,0,0,100,100,10,400,400,1000,100,1000,40};
    std::memcpy(p,defaults,sizeof defaults);return p;
}
static uint32_t* __cdecl physicsLimits(uint32_t* p,uint32_t a,uint32_t b,uint32_t c,uint32_t d) {
    p[13]=a;p[12]=b;p[14]=c;p[15]=d;return p;
}
static uint32_t* __cdecl physicsPair(uint32_t* p,uint32_t n) { p[10]=p[11]=n;return p; }
static float* __cdecl scalePhysicsBounds(float* bounds,float factor) {
    // Two padded V3d endpoints, NOT the 24-byte RwBBox or a 2D rectangle.
    for(unsigned i=0;i<3;++i) {
        double center=(double(bounds[i])+bounds[i+4])*.5;
        bounds[i+4]=float((bounds[i+4]-center)*factor+center);
        bounds[i]=float((bounds[i]-center)*factor+center);
    }
    return bounds;
}
static Element* __cdecl addForceAtPoint(Element* e,const rw::V3d* force,const rw::V3d* point) {
    auto* state=e->aggregate->state;
    float* accum=state->forces+8*e->index;
    accum[0]+=force->x;accum[1]+=force->y;accum[2]+=force->z;
    auto arm=rw::sub(*point,state->matrices[e->index].pos);
    // Executable uses arm cross force (the opposite convention to force cross arm).
    auto torque=rw::cross(arm,*force);
    accum[4]+=torque.x;accum[5]+=torque.y;accum[6]+=torque.z;
    return e;
}
static void* __cdecl setBodyPartTransform(Body* b,uint16_t index) {
    if(!b->owner) return nullptr;
    if(!bodyActive(b)) return b->owner->active; // exact inactive return carrier
    auto& part=static_cast<csf_rw::PhysicsPart*>(b->parts)[index];
    auto* e=part.element;auto* s=e->aggregate->state;
    auto& m=s->matrices[e->index];
    m=*part.matrix;
    // Game writes matrices directly without updating the cached IDENTITY bit.
    // Matrix::translate's identity shortcut would discard that translation.
    rw::V3d position;
    rw::V3d::transformPoints(&position,&e->translation,1,&m);
    m.pos=position;m.flags &= ~uint32_t(rw::Matrix::IDENTITY);
    s->rotations[e->index]=m.getRotation();
    return e;
}

HOOK(0x007FA460,configureMass,"boundary-rw");
HOOK(0x007FA5B0,defaultShape,"boundary-rw");
HOOK(0x007FAFD0,setTimeStep,"boundary-rw");
HOOK(0x007FDAD0,handleDictionary,"boundary-rw");
HOOK(0x007FDB00,createBitMatrix,"boundary-rw");
HOOK(0x007FDB40,destroyBitMatrix,"boundary-rw");
HOOK(0x007FEDC0,bodyBounds,"boundary-rw");
HOOK(0x007FEE10,enableBody,"boundary-rw");
HOOK(0x007FEE60,activateBody,"boundary-rw");
HOOK(0x007FEF10,deactivateBody,"boundary-rw");
HOOK(0x007FF0A0,collisionMatrix,"boundary-rw");
HOOK(0x007FF0C0,bodyLinks,"boundary-rw");
HOOK(0x007FFB90,initPhysicsParameters,"boundary-rw");
HOOK(0x007FFBF0,physicsLimits,"boundary-rw");
HOOK(0x007FFC20,physicsPair,"boundary-rw");
HOOK(0x008006D0,scalePhysicsBounds,"boundary-rw");
HOOK(0x008013C0,secondWord,"boundary-rw");
HOOK(0x008013D0,childAt,"boundary-rw");
HOOK(0x00801520,proxyTarget,"boundary-rw");
HOOK(0x00802410,addForceAtPoint,"boundary-rw");
HOOK(0x00802650,dimension68,"boundary-rw");
HOOK(0x00802660,dimension64,"boundary-rw");
HOOK(0x00803770,setBodyPartTransform,"boundary-rw");
