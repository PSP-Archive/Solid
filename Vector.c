/***********************************************************************************

  Module :	Vector.c

  Description :	

  Last Modified $Date: $

  $Revision: $

  Copyright (C) 28 May 2005 T Swann

***********************************************************************************/

//**********************************************************************************
//   Include Files
//**********************************************************************************
#include "Types.h"
#include "Maths.h"
#include "Vector.h"

//**********************************************************************************
//   Local Macros
//**********************************************************************************

//**********************************************************************************
//   Local Constants
//**********************************************************************************
const V3	gZeroVector = { 0.f, 0.f, 0.f };
const V3	gAtVector = { 0.f, 0.f, 1.f };
const V3	gUpVector = { 0.f, 1.f, 0.f };
const V3	gRightVector = { 1.f, 0.f, 0.f };

//**********************************************************************************
//   Static Prototypes
//**********************************************************************************

//**********************************************************************************
//   Global Variables
//**********************************************************************************

//**********************************************************************************
//   Static Variables
//**********************************************************************************

//**********************************************************************************
//   Class Definition
//**********************************************************************************

//*************************************************************************************
//	
//*************************************************************************************
V3	V3_Add( const V3 * const p_lhs, const V3 * const p_rhs )
{
	V3	res;

	res.x = p_rhs->x + p_lhs->x;
	res.y = p_rhs->y + p_lhs->y;
	res.z = p_rhs->z + p_lhs->z;

	return res;
}

//*************************************************************************************
//	
//*************************************************************************************
V3	V3_Sub( const V3 * const p_rhs, const V3 * const p_lhs )
{
	V3	res;

	res.x = p_rhs->x - p_lhs->x;
	res.y = p_rhs->y - p_lhs->y;
	res.z = p_rhs->z - p_lhs->z;

	return res;
}

//*************************************************************************************
//	
//*************************************************************************************
V3	V3_Mul( const V3 * const p_lhs, f32 val )
{
	V3	res;

	res.x = p_lhs->x * val;
	res.y = p_lhs->y * val;
	res.z = p_lhs->z * val;

	return res;
}

//*************************************************************************************
//	
//*************************************************************************************
V3	V3_Div( const V3 * const p_lhs, f32 val )
{
	V3	res;

	res.x = p_lhs->x / val;
	res.y = p_lhs->y / val;
	res.z = p_lhs->z / val;

	return res;
}

//*************************************************************************************
//	
//*************************************************************************************
f32	V3_Length( const V3 * const p_lhs )
{
	f32	length = V3_LengthSq( p_lhs );

	return SQRTF( length );
}

//*************************************************************************************
//	
//*************************************************************************************
f32	V3_LengthSq( const V3 * const p_lhs )
{
	return ( ( p_lhs->x * p_lhs->x ) + ( p_lhs->y * p_lhs->y ) + ( p_lhs->z * p_lhs->z ) );
}

//*************************************************************************************
//	
//*************************************************************************************
f32	V3_Dot( const V3 * const p_lhs, const V3 * const p_rhs )
{
	return ( p_lhs->x * p_rhs->x + p_lhs->y * p_rhs->y + p_lhs->z * p_rhs->z );
}

//*************************************************************************************
//	
//*************************************************************************************
V3	V3_Cross( const V3 * const p_lhs, const V3 * const p_rhs )
{
	V3	res;

	res.x = p_lhs->y * p_rhs->z - p_lhs->z * p_rhs->y;
	res.y = p_lhs->z * p_rhs->x - p_lhs->x * p_rhs->z;
	res.z = p_lhs->x * p_rhs->y - p_lhs->y * p_rhs->x;

	return res;
}

//*************************************************************************************
//	
//*************************************************************************************
f32	V3_Normalise( V3 * const p_lhs )
{
	f32 length = V3_Length( p_lhs );

	if ( length != 0.f )
	{
		f32	recip = 1.f / length;

		*p_lhs = V3_Mul( p_lhs, recip );

		return length;
	}
	else
	{
		length = 0.f;
	}

	return length;
}

//*************************************************************************************
//	
//*************************************************************************************
V3		RotateX( const V3 * const p_v, u16 x_angle )
{
	V3	r;
	f32	c = COSF( x_angle );
	f32	s = SINF( x_angle );

	r.x = p_v->x;
	r.y = p_v->y * c + p_v->z * s;
	r.z = -p_v->y * s + p_v->z * c;

	return r;
}

V3		RotateY( const V3 * const p_v, u16 y_angle )
{
	V3	r;
	f32	c = COSF( y_angle );
	f32	s = SINF( y_angle );

	r.x = p_v->x * c - p_v->z * s;
	r.y = p_v->y;
	r.z = p_v->x * s + p_v->z * c;

	return r;
}

V3		RotateZ( const V3 * const p_v, u16 z_angle )
{
	V3	r;
	f32	c = COSF( z_angle );
	f32	s = SINF( z_angle );

	r.x = p_v->x * c + p_v->y * s;
	r.y = -p_v->x * s + p_v->y * c;
	r.z = p_v->z;

	return r;
}

//*******************************  END OF FILE  ************************************
