// No librw particle toolkit exists. Generate the game's inline keyframe
// records, then let the standard integrator and PTank adapter consume them.
template<class F> bool newestParticles(ParticleEmitter* e,F f) {
    auto* b=e->activeBatches;
    if(!b || b->newParticle<0) return true;
    if(!e->particleClass || b->newParticle>b->numParticles) return false;
    auto* p=reinterpret_cast<unsigned char*>(b)+b->dataOffset+size_t(b->newParticle)*e->particleClass->objectSize;
    for(int i=b->newParticle;i<b->numParticles;i++,p+=e->particleClass->objectSize) if(!f(p)) return false;
    return true;
}
template<int Id,int Target,int Dimensions,bool Stepped=false>
bool multiGenerate(ParticleEmitter* e,unsigned char* particle,uint32_t& seed) {
    auto* emitter=particleProperty(e,Id);int off=propOffset(e->particleClass->properties,Id);
    if(!emitter || off<0) return true;
    int target=propOffset(e->particleClass->properties,Target);if(target<0) return false;
    int n=payloadGet<int>(emitter);if(n<=0) return true;
    if(n>(propertyBytes(e,Id)-multiHeader<Id>())/multiElement<Id>()) return false;
    auto* p=particle+off;auto* value=particle+target;auto* keys=payloadGet<unsigned char*>(emitter,4);
    float end[Dimensions],initial[Dimensions];
    for(int d=0;d<Dimensions;d++) {
        initial[d]=payloadGet<float>(value,4*d);
        end[d]=initial[d]+payloadGet<float>(value,4*(d+Dimensions));
    }
    auto sample=[&](int i,double& time,std::array<float,Dimensions>& values) {
        auto* data=keys+size_t(i)*multiElement<Id>();
        time=vary(seed,payloadGet<float>(data),payloadGet<float>(data,4));
        for(int d=0;d<Dimensions;d++) {
            int base,bias;
            if constexpr(Id==0x1000002) { base=2+d;bias=6+d; }
            else { base=2+(d/2)*4+d%2;bias=base+2; }
            values[d]=vary(seed,payloadGet<float>(data,base*4),payloadGet<float>(data,bias*4));
        }
    };
    std::array<float,Dimensions> current{},next{};double time,nextTime=0;sample(0,time,current);
    double firstTime=time;auto firstValue=current;
    auto store=[&](int segment,int dimension,bool slope,float v) {
        int base=4+4*n,index;
        if constexpr(Dimensions==4 && Target==3) index=(dimension/2)*2+int(slope),base+=index*8*n,index=dimension%2;
        else { base+=int(slope)*Dimensions*4*n;index=dimension; }
        payloadSet(p,base+segment*(Target==3 ? 8 : Dimensions*4)+index*4,v);
    };
    for(int k=0;k<n;k++) {
        payloadSet<float>(p,4+4*k,float(time*payloadGet<float>(particle,8)));
        if constexpr(Stepped) {
            // Step UV stores two vector arrays rather than intercept/slope.
            for(int d=0;d<Dimensions;d++) payloadSet<float>(p,4+4*n+(d/2)*8*n+k*8+(d%2)*4,current[d]);
        } else {
            if(k+1<n) sample(k+1,nextTime,next);
            else { nextTime=1;std::copy_n(end,Dimensions,next.begin()); }
            double inv=nextTime-time<=0 ? 0 : 1/(nextTime-time);
            for(int d=0;d<Dimensions;d++) {
                float slope=float((next[d]-current[d])*inv);
                store(k,d,false,float(current[d]-time*slope));store(k,d,true,slope);
            }
        }
        if constexpr(Stepped) { if(k+1<n) sample(k+1,nextTime,next); }
        time=nextTime;current=next;
    }
    payloadSet<uint32_t>(p,0,0);
    for(int d=0;d<Dimensions;d++) payloadSet<float>(value,4*(d+Dimensions),Stepped ? 0 : float((firstValue[d]-initial[d])/firstTime));
    return true;
}
template<bool Stepped> ParticleEmitter* __cdecl multiEmit(Atomic*,ParticleEmitter* e,void*) {
    auto* s=static_cast<EmitterStandard*>(particleProperty(e,1));if(!s) return nullptr;
    if(!(s->currentTime>=s->nextEmit) || !e->activeBatches || e->activeBatches->newParticle<0) return e;
    uint32_t seed=s->seed*0x0bb38435u+0x3619636bu;
    bool ok=newestParticles(e,[&](unsigned char* p) {
        if(payloadGet<float>(p,4)!=0) return true;
        if constexpr(Stepped) return multiGenerate<0x1000004,3,4,true>(e,p,seed);
        else return multiGenerate<0x1000002,2,4>(e,p,seed) && multiGenerate<0x1000003,3,4>(e,p,seed) && multiGenerate<0x1000005,5,2>(e,p,seed);
    });
    s->seed=seed;return ok ? e : nullptr;
}
ParticleEmitter* __cdecl subEmitterEmit(Atomic*,ParticleEmitter* e,void*) {
    auto* s=static_cast<EmitterStandard*>(particleProperty(e,1));auto* config=static_cast<float*>(particleProperty(e,0x1000001));
    if(!s || !config || !e->particleClass) return nullptr;
    int off=propOffset(e->particleClass->properties,0x1000001);if(off<0) return nullptr;
    uint32_t seed=s->seed;
    bool ok=newestParticles(e,[&](unsigned char* p) {
        payloadSet<float>(p,off,float(vary(seed,config[0],config[1])+payloadGet<float>(p,4)));
        payloadSet<float>(p,off+4,vary(seed,config[2],config[3]));return true;
    });
    s->seed=seed;return ok ? e : nullptr;
}
template<bool Circle> ParticleEmitter* __cdecl shapedEmit(Atomic*,ParticleEmitter* e,void*) {
    auto* s=static_cast<EmitterStandard*>(particleProperty(e,1));if(!s || !e->particleClass) return nullptr;
    auto* config=particleProperty(e,Circle ? 0x1000007 : 0x1000006);if(!config) return nullptr;
    auto* b=e->activeBatches;if(!b || b->newParticle<0) return e;
    int count=b->numParticles-b->newParticle;if(count<0) return nullptr;
    int position=propOffset(e->particleClass->properties,1),velocity=propOffset(e->particleClass->properties,6);
    auto* tank=static_cast<EmitterTank*>(particleProperty(e,6));unsigned char* cursor=nullptr;int stride=0;
    if(tank && tank->output && tank->strides) {
        int cluster=0;
        if(tank->dataFlags&1) { cursor=tank->output[0];stride=tank->strides[0];cluster++; }
        if(tank->dataFlags&8) { cursor=tank->output[cluster];stride=tank->strides[cluster]; }
        if(cursor) cursor-=size_t(count)*stride;
    }
    uint32_t seed=s->seed;
    bool ok=newestParticles(e,[&](unsigned char* p) {
        rw::V3d local{},world{};double sinAngle=0,cosAngle=0;int point=0;
        if(velocity>=0) {
            seed=seed*0x0bb38435u+0x3619636bu;
            if constexpr(Circle) {
                double angle=(double(seed)*0x1p-32*2-1)*3.141592502593994140625;
                sinAngle=std::sin(angle);cosAngle=std::cos(angle);
                seed=seed*0x0bb38435u+0x3619636bu;
                double radius=(double(int32_t(seed))*0x1p-32+0.5)*payloadGet<float>(config,4)+payloadGet<float>(config);
                local.x=float(radius*sinAngle);local.y=float(radius*cosAngle);
                local.z=vary(seed,0,payloadGet<float>(config,8));
            } else {
                int n=payloadGet<int>(config);if(n<=0) return false;
                if(payloadGet<int>(config,12)) point=int((double(int32_t(seed))*0x1p-32+0.5)*n);
                else { point=payloadGet<int>(config,4);payloadSet<int>(config,4,point+1<n ? point+1 : 0); }
                if(point<0 || point>=n) return false;
                auto* points=payloadGet<rw::V3d*>(config,16);if(!points) return false;
                local={vary(seed,points[point].x,s->positionBias.x),vary(seed,points[point].y,s->positionBias.y),vary(seed,points[point].z,s->positionBias.z)};
            }
            world=local;if(s->ltm) particlePoint(&world,&local,s->ltm);
            if(payloadGet<int>(config,Circle ? 12 : 8)==1) {
                rw::V3d direction;
                if constexpr(Circle) {
                    float angle=payloadGet<float>(config,16);double sn=std::sin(angle),cs=std::cos(angle);
                    direction={vary(seed,float(sn*sinAngle),s->directionBias.x),vary(seed,float(sn*cosAngle),s->directionBias.y),vary(seed,float(cs),s->directionBias.z)};
                } else {
                    auto* normals=payloadGet<rw::V3d*>(config,20+pointerGrowth);if(!normals) return false;
                    direction={vary(seed,normals[point].x,s->directionBias.x),vary(seed,normals[point].y,s->directionBias.y),vary(seed,normals[point].z,s->directionBias.z)};
                }
                particleNormalize(&direction,&direction);if(s->ltm) particleVector(&direction,&direction,s->ltm);
                double speed=vary(seed,s->speed,s->speedBias);
                payloadSet(p,velocity,rw::V3d{float(direction.x*speed),float(direction.y*speed),float(direction.z*speed)});
            }
        }
        if(position>=0) payloadSet(p,position,world);
        if(cursor && tank && (tank->dataFlags&uint32_t(tank->flags)&1)) { std::memcpy(cursor,&world,12);cursor+=stride; }
        return true;
    });
    s->seed=seed;return ok ? e : nullptr;
}
