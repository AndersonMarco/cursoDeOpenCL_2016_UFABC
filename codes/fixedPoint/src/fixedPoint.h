#ifndef FIXED_POINT_H
#define FIXED_POINT_H
#include <stdio.h>
#include <stdint.h>

/**
 *@brief Definition of fixed point and functions to handle fixed points
 *Definition of the fixed point used in this work. The fixed points in
 *this work has 96 bits for represent the decimal places after the 
 *dot and 32 bits for represent the decimals places before  the dot
 */

/**
 *@brief struct for fixed point 
 */ 
typedef struct{
    unsigned int x;
    unsigned int y;
    unsigned int z;
    unsigned int w;
        
}uint4;

/**
 *@brief macro to help the write of functions add128 and mulfpu
 */
#define uint4Sum(u,v,ret)\
  ret.x=u.x+v.x;\
  ret.y=u.y+v.y;\
  ret.z=u.z+v.z;\
  ret.w=u.w+v.w;

/**
 *@brief increment u at 2^{-96} 
 *@param u argument to increment
 *@return u+2^{-96}
 */
uint4 inc128(uint4 u);

/**
 *@brief negation of a fixed-point
 *@param u argument to negarion
 *@return  -u
 */
uint4 neg128(uint4 u);

/**
 *@brief operation addition
 *@param u argument to adition
 *@param v argument to adition
 *@return  u+v
 */
uint4 add128(uint4 u, uint4 v);

/**
 *@brief operation that returns the  32 bits most signficants of the 64 bits integer that is resulted of the operation x*y
 *The function is used by mulfpu.
 *@param x argument to operation
 *@param y argument to opetarion
 *@return  (x*y) >>32
 */

unsigned int mul_hi(unsigned int x, unsigned int y);

/**
 *@brief Returns u*v truncated to keep the position of the decimal point.
 *@param u argument to multiplication
 *@param v argument to multiplication
 *@return  u*v
 */
uint4 mulfpu(uint4 u,uint4 v);

/**
 *@brief Check if v>u 
 *@param u argument to operation
 *@param v argument to operation
 *@return  1 if v>u  or 0 otherwise
 */
int greaterThan(uint4 v, uint4 u);

#endif
