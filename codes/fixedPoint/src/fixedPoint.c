#include "fixedPoint.h"
uint4 inc128(uint4 u)
{
  // Compute all carries to add
    uint4 h;
    h.x= u.x==0xffffffffu;
    h.y= u.y==0xffffffffu;
    h.z= u.z==0xffffffffu;
    h.w= u.w==0xffffffffu;
    uint4 c; c.x=h.y&h.z&h.w&1u;    c.y=h.z&h.w&1u;    c.z=h.w&1u;    c.w=1u;
    uint4 ret;
    uint4Sum(u,c,ret);
    return ret;
}

uint4 neg128(uint4 u)
{
    uint4 r; r.x=u.x^0xffffffffu; r.y=u.y^0xffffffffu; r.z=u.z^0xffffffffu; r.w=u.w^0xffffffffu;

    return inc128(r); // (1 + ~U) is two's complement
}
uint4 add128(uint4 u, uint4 v){
    uint4 s;uint4Sum(u,v,s);
    uint4 comp; comp.x=0xffffffffu; comp.y=0xffffffffu; comp.z=0xffffffffu; comp.w=0xffffffffu;
    uint4 temp; temp.x=s.x<u.x; temp.y=s.y<u.y; temp.z=s.z<u.z; temp.w=s.w<u.w;
    uint4 h; h.x=temp.x; h.y=temp.y; h.z=temp.z; h.w=temp.w;
    uint4 c1; c1.x=h.y&1u; c1.y=h.z&1u; c1.z=h.w&1u; c1.w=0;
    h.x=s.x==comp.x; h.y=s.y==comp.y; h.z=s.z==comp.z; h.w=s.w==comp.w;
    uint4 c2; c2.x=(c1.y|(c1.z&h.z))&h.y; c2.y=c1.z&h.z; c2.z=0; c2.w=0;
    uint4 ret;
    uint4 ret2;
    uint4Sum(s,c1,ret);
    uint4Sum(ret,c2,ret2);
    return ret2;    
}

unsigned int mul_hi(unsigned int x, unsigned int y){
	uint64_t x1=x;
	uint64_t y1=y;
    return ((x1*y1)>>32);
}


// Return U*V truncated to keep the position of the decimal point.
// U and V MUST be positive.
uint4 mulfpu(uint4 u,uint4 v)
{
  int negV=0;  
  if((v.x>>31)==1){
      negV=1;
      v=neg128(v);
  }
  int negU=0;
  if((u.x>>31)==1){
      negU=1;
      u=neg128(u);
  }
  // Diagonal coefficients
  uint4 s; s.x=(u.x*v.x); s.y=mul_hi(u.y,v.y); s.z=u.y*v.y; s.w=mul_hi(u.z,v.z);
  // Off-diagonal
  uint4 t1; t1.x=mul_hi(u.x,v.y); t1.y=u.x*v.y; t1.z=mul_hi(u.x,v.w); t1.w=u.x*v.w;
  uint4 t2; t2.x=mul_hi(v.x,u.y); t2.y=v.x*u.y; t2.z=mul_hi(v.x,u.w); t2.w=v.x*u.w;
  s = add128(s,add128(t1,t2));
  t1.x=0; t1.y=mul_hi(u.x,v.z); t1.z=u.x*v.z; t1.w=mul_hi(u.y,v.w);
  t2.x=0; t2.y=mul_hi(v.x,u.z); t2.z=v.x*u.z; t2.w=mul_hi(v.y,u.w);
  s = add128(s,add128(t1,t2));
  t1.x=0; t1.y=0; t1.z=mul_hi(u.y,v.z); t1.w=u.y*v.z;
  t2.x=0; t2.y=0; t2.z=mul_hi(v.y,u.z); t2.w=v.y*u.z;
  s = add128(s,add128(t1,t2));
  // Add 3 to compensate truncation
  //end

  //eu fiz isto===========================================
  uint4 p2Uwwzy; p2Uwwzy.x=u.w; p2Uwwzy.y=u.w; p2Uwwzy.z=u.z; p2Uwwzy.w=u.y;
  uint4 p2Vzyzw; p2Vzyzw.x=v.z; p2Vzyzw.y=v.y; p2Vzyzw.z=v.z; p2Vzyzw.w=v.w;
  uint4 restTimes; restTimes.x=p2Vzyzw.x*p2Uwwzy.x; restTimes.y=p2Vzyzw.y*p2Uwwzy.y; restTimes.z=p2Vzyzw.z*p2Uwwzy.z; restTimes.w=p2Vzyzw.w*p2Uwwzy.w;
  uint4 a; a.x=0; a.y=0; a.z=mul_hi(u.z,v.w); a.w= mul_hi(u.w,v.w);
  uint4 b; b.x=0; b.y=0; b.z=mul_hi(u.w,v.z); b.w= u.z*v.w;
  uint4 c; c.x=0; c.y=0; c.z=restTimes.z; c.w= restTimes.x;
  uint4 d; d.x=0; d.y=0; d.z=restTimes.y; d.w= 0;
  uint4 e; e.x=0; e.y=0; e.z=restTimes.w; e.w= 0;
  uint4 f=add128(a,b);
  f=add128(f,c);
  f=add128(f,d);
  f=add128(f,e);
  uint4 t3; t3.x=0; t3.y=0, t3.z=0; t3.w=f.y;
  uint4 ret=add128(s,t3);  
  //fim===================================================
  if(negV!=negU){
      uint4 t4; t4.x=0; t4.y=0; t4.z=0; t4.w=1;
      if( f.y>0 || f.z>0 || f.w>0) ret=add128(ret, t4);      
      return neg128(ret);
  }

  return ret;
}
int greaterThan(uint4 v, uint4 u){
    int negV=0;
    if( (v.x>>31)==1){
        negV=1;
        v=neg128(v);
        
    }

    int negU=0;
    if( (u.x>>31)==1){
        negU=1;
        u=neg128(u);
    }
    
    if(negV==0 && negU==1)
        return 1;
    
    
    if(negV==1 && negU==0)
        return 0;
            

    int uAndVNegative=negV && negU;
    if(v.x>u.x){
        if(uAndVNegative==0)
            return 1;
        else
            return 0;
    }
    else if( v.x!=u.x){
        if(uAndVNegative==0)
            return 0;
        else
            return  1;         
    }

    if(v.y>u.y){
        if(uAndVNegative==0)
            return 1;
        else
            return 0;
    }
    else if( v.y!=u.y){
        if(uAndVNegative==0)
            return 0;
        else
            return  1;
           
    }

    if(v.z>u.z){
        if(uAndVNegative==0)
            return 1;
        else
            return 0;        
    }
    else if( v.z!=u.z){
        if(uAndVNegative==0)
            return 0;
        else
            return  1;

    }

    if(v.w>u.w){
        if(uAndVNegative==0)
            return 1;
        else
            return 0;
    }
    else if( v.w!=u.w){
        if(uAndVNegative==0)
            return 0;
        else
            return  1;
        
    }
    return 0;
}

/**
 * @brief This function is just to validate the others functions of fixed point.
 */
void testeFixedPoint(void){
    
    /*uint4 a; a.x=0xfffffff1u; a.y=0x10e97d18u; a.z=0xc6676b29u; a.w=0xe06528acu;//y
    uint4 b; b.x=0xfffffff6u; b.y=0x24d7397eu; b.z=0x41e1eecdu; b.w=0xdfc8e0b4u; //x
    uint4 c=mulfpu(a,neg128(b));
    printf("%08x%08x%08x%08x\n", c.x,c.y,c.z,c.w);*/
    
    uint4 x;   x.x=5;   x.y=0x23421;     x.z=0x0;           x.w=0x0;
    uint4 y;   y.x=2;   y.y=0xaa23421;   y.z=0x0;           y.w=0x0;
    uint4 z;   z.x=1;   z.y=0xfffff21;   z.z=0x0;           z.w=0x0;
    uint4 p;   p.x=28;  p.y=0x0;         p.z=0x0;           p.w=0x0;
    uint4 o;   o.x=10;  o.y=0x0;         o.z=0x0;           o.w=0x0;
    uint4 b;   b.x=2;   b.y=0xaaaaaaaau; b.z=0xaa97e000u;   b.w=0x00000000;
    uint4 dt; dt.x=0;  dt.y=0x03418937u; dt.z=0x4bc6a7f0u;  dt.w=0x00000000;
    int i=0;
    for(i=0;i<3500000;i++){
        uint4 xt=add128( mulfpu(mulfpu(add128(y, neg128(x)),o),dt),x);
        uint4 yt=add128( mulfpu(add128( mulfpu(add128(p, neg128(z)),x) ,neg128(y)),dt), y);
        uint4 zt=add128(mulfpu(add128( mulfpu(x,y) ,neg128(mulfpu(b,z))),dt), z);
        x.x=xt.x;
        x.y=xt.y;
        x.z=xt.z;
        x.w=xt.w;

        y.x=yt.x;
        y.y=yt.y;
        y.z=yt.z;
        y.w=yt.w;

        z.x=zt.x;
        z.y=zt.y;
        z.z=zt.z;
        z.w=zt.w;
    }
    //x=mulfpu(add128(y, neg128(x)),o);
    printf("%08x %08x %08x %08x \n",x.x,x.y,x.z,x.w);
}
