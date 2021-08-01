/***********************************************************************************

  Module :	Matrix.cpp

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
#include "Matrix.h"
#include "Vector.h"

//**********************************************************************************
//   Local Macros
//**********************************************************************************

//**********************************************************************************
//   Local Constants
//**********************************************************************************

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
void	MTX_GetPos( const M44 * const p_mtx, V3 * const p_pos )
{
	p_pos->x = p_mtx->m[ 2 ][ 0 ];
	p_pos->y = p_mtx->m[ 2 ][ 1 ];
	p_pos->z = p_mtx->m[ 2 ][ 2 ];
}

void	MTX_SetPos( M44 * const p_mtx, const V3 * const p_pos )
{
	p_mtx->m[ 3 ][ 0 ] = p_pos->x;
	p_mtx->m[ 3 ][ 1 ] = p_pos->y;
	p_mtx->m[ 3 ][ 2 ] = p_pos->z;
	p_mtx->m[ 3 ][ 3 ] = 1.f;
}

//*************************************************************************************
//	
//*************************************************************************************
void	MTX_GetAt( const M44 * const p_mtx, V3 * const p_at )
{
	p_at->x = p_mtx->m[ 2 ][ 0 ];
	p_at->y = p_mtx->m[ 2 ][ 1 ];
	p_at->z = p_mtx->m[ 2 ][ 2 ];
}

void	MTX_SetAt( M44 * const p_mtx, const V3 * const p_at )
{
	p_mtx->m[ 2 ][ 0 ] = p_at->x;
	p_mtx->m[ 2 ][ 1 ] = p_at->y;
	p_mtx->m[ 2 ][ 2 ] = p_at->z;
	p_mtx->m[ 2 ][ 3 ] = 0.f;
}

//*************************************************************************************
//	
//*************************************************************************************
void	MTX_GetUp( const M44 * const p_mtx, V3 * const p_up )
{
	p_up->x = p_mtx->m[ 1 ][ 0 ];
	p_up->y = p_mtx->m[ 1 ][ 1 ];
	p_up->z = p_mtx->m[ 1 ][ 2 ];
}

void	MTX_SetUp( M44 * const p_mtx, const V3 * const p_up )
{
	p_mtx->m[ 1 ][ 0 ] = p_up->x;
	p_mtx->m[ 1 ][ 1 ] = p_up->y;
	p_mtx->m[ 1 ][ 2 ] = p_up->z;
	p_mtx->m[ 1 ][ 3 ] = 0.f;
}

//*************************************************************************************
//	
//*************************************************************************************
void	MTX_GetRight( const M44 * const p_mtx, V3 * const p_right )
{
	p_right->x = p_mtx->m[ 0 ][ 0 ];
	p_right->y = p_mtx->m[ 0 ][ 1 ];
	p_right->z = p_mtx->m[ 0 ][ 2 ];
}

void	MTX_SetRight( M44 * const p_mtx, const V3 * const p_right )
{
	p_mtx->m[ 0 ][ 0 ] = p_right->x;
	p_mtx->m[ 0 ][ 1 ] = p_right->y;
	p_mtx->m[ 0 ][ 2 ] = p_right->z;
	p_mtx->m[ 0 ][ 3 ] = 0.f;
}

//*************************************************************************************
//	
//*************************************************************************************
void	MTX_SetIdentity( M44 * const p_mtx )
{
	p_mtx->m[ 0 ][ 0 ] = 1.f;	p_mtx->m[ 0 ][ 1 ] = 0.f;	p_mtx->m[ 0 ][ 2 ] = 0.f;	p_mtx->m[ 0 ][ 3 ] = 0.f;
	p_mtx->m[ 1 ][ 0 ] = 0.f;	p_mtx->m[ 1 ][ 1 ] = 1.f;	p_mtx->m[ 1 ][ 2 ] = 0.f;	p_mtx->m[ 1 ][ 3 ] = 0.f;
	p_mtx->m[ 2 ][ 0 ] = 0.f;	p_mtx->m[ 2 ][ 1 ] = 0.f;	p_mtx->m[ 2 ][ 2 ] = 1.f;	p_mtx->m[ 2 ][ 3 ] = 0.f;
	p_mtx->m[ 3 ][ 0 ] = 0.f;	p_mtx->m[ 3 ][ 1 ] = 0.f;	p_mtx->m[ 3 ][ 2 ] = 0.f;	p_mtx->m[ 3 ][ 3 ] = 1.f;
}

//*************************************************************************************
//	
//*************************************************************************************
void	MTX_SetTranslation( M44 * const p_mtx, f32 x, f32 y, f32 z )
{
	p_mtx->m[ 0 ][ 0 ] = 1.f;	p_mtx->m[ 0 ][ 1 ] = 0.f;	p_mtx->m[ 0 ][ 2 ] = 0.f;	p_mtx->m[ 0 ][ 3 ] = 0.f;
	p_mtx->m[ 1 ][ 0 ] = 0.f;	p_mtx->m[ 1 ][ 1 ] = 1.f;	p_mtx->m[ 1 ][ 2 ] = 0.f;	p_mtx->m[ 1 ][ 3 ] = 0.f;
	p_mtx->m[ 2 ][ 0 ] = 0.f;	p_mtx->m[ 2 ][ 1 ] = 0.f;	p_mtx->m[ 2 ][ 2 ] = 1.f;	p_mtx->m[ 2 ][ 3 ] = 0.f;
	p_mtx->m[ 3 ][ 0 ] = x;		p_mtx->m[ 3 ][ 1 ] = y;		p_mtx->m[ 3 ][ 2 ] = z;		p_mtx->m[ 3 ][ 3 ] = 1.f;
}

//*************************************************************************************
//	
//*************************************************************************************
void	MTX_SetScaling( M44 * const p_mtx, f32 sx, f32 sy, f32 sz )
{
	p_mtx->m[ 0 ][ 0 ] = sx;	p_mtx->m[ 0 ][ 1 ] = 0.f;	p_mtx->m[ 0 ][ 2 ] = 0.f;	p_mtx->m[ 0 ][ 3 ] = 0.f;
	p_mtx->m[ 1 ][ 0 ] = 0.f;	p_mtx->m[ 1 ][ 1 ] = sy;	p_mtx->m[ 1 ][ 2 ] = 0.f;	p_mtx->m[ 1 ][ 3 ] = 0.f;
	p_mtx->m[ 2 ][ 0 ] = 0.f;	p_mtx->m[ 2 ][ 1 ] = 0.f;	p_mtx->m[ 2 ][ 2 ] = sz;	p_mtx->m[ 2 ][ 3 ] = 0.f;
	p_mtx->m[ 3 ][ 0 ] = 0.f;	p_mtx->m[ 3 ][ 1 ] = 0.f;	p_mtx->m[ 3 ][ 2 ] = 0.f;	p_mtx->m[ 3 ][ 3 ] = 1.f;
}

//*************************************************************************************
//	
//*************************************************************************************
void	MTX_SetRotationX( M44 * const p_mtx, u16 x_rot )
{
	V3	r = RotateX( &gRightVector, x_rot );
	V3	u = RotateX( &gUpVector, x_rot );
	V3	a = RotateX( &gAtVector, x_rot );

	p_mtx->m[ 0 ][ 0 ] = r.x;	p_mtx->m[ 0 ][ 1 ] = r.y;	p_mtx->m[ 0 ][ 2 ] = r.z;	p_mtx->m[ 0 ][ 3 ] = 0.f;
	p_mtx->m[ 1 ][ 0 ] = u.x;	p_mtx->m[ 1 ][ 1 ] = u.y;	p_mtx->m[ 1 ][ 2 ] = u.z;	p_mtx->m[ 1 ][ 3 ] = 0.f;
	p_mtx->m[ 2 ][ 0 ] = a.x;	p_mtx->m[ 2 ][ 1 ] = a.y;	p_mtx->m[ 2 ][ 2 ] = a.z;	p_mtx->m[ 2 ][ 3 ] = 0.f;
	p_mtx->m[ 3 ][ 0 ] = 0.f;	p_mtx->m[ 3 ][ 1 ] = 0.f;	p_mtx->m[ 3 ][ 2 ] = 0.f;	p_mtx->m[ 3 ][ 3 ] = 1.f;
}

void	MTX_SetRotationY( M44 * const p_mtx, u16 y_rot )
{
	V3	r = RotateY( &gRightVector, y_rot );
	V3	u = RotateY( &gUpVector, y_rot );
	V3	a = RotateY( &gAtVector, y_rot );

	p_mtx->m[ 0 ][ 0 ] = r.x;	p_mtx->m[ 0 ][ 1 ] = r.y;	p_mtx->m[ 0 ][ 2 ] = r.z;	p_mtx->m[ 0 ][ 3 ] = 0.f;
	p_mtx->m[ 1 ][ 0 ] = u.x;	p_mtx->m[ 1 ][ 1 ] = u.y;	p_mtx->m[ 1 ][ 2 ] = u.z;	p_mtx->m[ 1 ][ 3 ] = 0.f;
	p_mtx->m[ 2 ][ 0 ] = a.x;	p_mtx->m[ 2 ][ 1 ] = a.y;	p_mtx->m[ 2 ][ 2 ] = a.z;	p_mtx->m[ 2 ][ 3 ] = 0.f;
	p_mtx->m[ 3 ][ 0 ] = 0.f;	p_mtx->m[ 3 ][ 1 ] = 0.f;	p_mtx->m[ 3 ][ 2 ] = 0.f;	p_mtx->m[ 3 ][ 3 ] = 1.f;
}

void	MTX_SetRotationZ( M44 * const p_mtx, u16 z_rot )
{
	V3	r = RotateZ( &gRightVector, z_rot );
	V3	u = RotateZ( &gUpVector, z_rot );
	V3	a = RotateZ( &gAtVector, z_rot );

	p_mtx->m[ 0 ][ 0 ] = r.x;	p_mtx->m[ 0 ][ 1 ] = r.y;	p_mtx->m[ 0 ][ 2 ] = r.z;	p_mtx->m[ 0 ][ 3 ] = 0.f;
	p_mtx->m[ 1 ][ 0 ] = u.x;	p_mtx->m[ 1 ][ 1 ] = u.y;	p_mtx->m[ 1 ][ 2 ] = u.z;	p_mtx->m[ 1 ][ 3 ] = 0.f;
	p_mtx->m[ 2 ][ 0 ] = a.x;	p_mtx->m[ 2 ][ 1 ] = a.y;	p_mtx->m[ 2 ][ 2 ] = a.z;	p_mtx->m[ 2 ][ 3 ] = 0.f;
	p_mtx->m[ 3 ][ 0 ] = 0.f;	p_mtx->m[ 3 ][ 1 ] = 0.f;	p_mtx->m[ 3 ][ 2 ] = 0.f;	p_mtx->m[ 3 ][ 3 ] = 1.f;
}

//*************************************************************************************
//	
//*************************************************************************************
void	MTX_SetRotationAxis( M44 * const p_mtx, const V3 * const p_axis, u16 degs )
{
	f32	c = COSF( degs );
	f32	s = SINF( degs );
	f32	t = 1.f - c;

	p_mtx->m[ 0 ][ 0 ] = t * p_axis->x * p_axis->x + c;
	p_mtx->m[ 0 ][ 1 ] = t * p_axis->x * p_axis->y - s * p_axis->z;
	p_mtx->m[ 0 ][ 2 ] = t * p_axis->x * p_axis->z + s * p_axis->y;
	p_mtx->m[ 0 ][ 3 ] = 0.f;

	p_mtx->m[ 1 ][ 0 ] = t * p_axis->x * p_axis->y + s * p_axis->z;
	p_mtx->m[ 1 ][ 1 ] = t * p_axis->y * p_axis->y + c;
	p_mtx->m[ 1 ][ 2 ] = t * p_axis->y * p_axis->z - s * p_axis->x;
	p_mtx->m[ 1 ][ 3 ] = 0;

	p_mtx->m[ 2 ][ 0 ] = t * p_axis->x * p_axis->z - s * p_axis->y;
	p_mtx->m[ 2 ][ 1 ] = t * p_axis->y * p_axis->z + s * p_axis->x;
	p_mtx->m[ 2 ][ 2 ] = t * p_axis->z * p_axis->z + c;
	p_mtx->m[ 2 ][ 3 ] = 0;

	p_mtx->m[ 3 ][ 0 ] = 0.f;
	p_mtx->m[ 3 ][ 1 ] = 0.f;
	p_mtx->m[ 3 ][ 2 ] = 0.f;
	p_mtx->m[ 3 ][ 3 ] = 1.f;
}

//*************************************************************************************
//	
//*************************************************************************************
void	MTX_Multiply( M44 * const p_mtx, const M44 * const p_matrix )
{
	M44	final;
	u32	r, c;

	for (r = 0; r < 4; r++)
	{
		for (c = 0; c < 4; c++)
		{
			final.m[ r ][ c ] = p_mtx->m[ r ][ 0 ] * p_matrix->m[ 0 ][ c ] +
								p_mtx->m[ r ][ 1 ] * p_matrix->m[ 1 ][ c ] +
								p_mtx->m[ r ][ 2 ] * p_matrix->m[ 2 ][ c ] +
								p_mtx->m[ r ][ 3 ] * p_matrix->m[ 3 ][ c ];
		}
	}

	for (r = 0; r < 4; r++)
	{
		for (c = 0; c < 4; c++)
		{
			p_mtx->m[ r ][ c ] = final.m[ r ][ c ];
		}
	}
}

//*************************************************************************************
//	
//*************************************************************************************
void	MTX_Inverse( const M44 * const p_mtx, M44 * const p_inv_mtx )
{
	p_inv_mtx->m[ 0 ][ 0 ] = p_mtx->m[ 1 ][ 1 ] * p_mtx->m[ 2 ][ 2 ] - p_mtx->m[ 1 ][ 2 ] * p_mtx->m[ 2 ][ 1 ];
	p_inv_mtx->m[ 0 ][ 1 ] = p_mtx->m[ 0 ][ 2 ] * p_mtx->m[ 2 ][ 1 ] - p_mtx->m[ 0 ][ 1 ] * p_mtx->m[ 2 ][ 2 ];
	p_inv_mtx->m[ 0 ][ 2 ] = p_mtx->m[ 0 ][ 1 ] * p_mtx->m[ 1 ][ 2 ] - p_mtx->m[ 0 ][ 2 ] * p_mtx->m[ 1 ][ 1 ];
	p_inv_mtx->m[ 0 ][ 3 ] = 0.f;
	p_inv_mtx->m[ 1 ][ 0 ] = p_mtx->m[ 1 ][ 2 ] * p_mtx->m[ 2 ][ 0 ] - p_mtx->m[ 1 ][ 0 ] * p_mtx->m[ 2 ][ 2 ];
	p_inv_mtx->m[ 1 ][ 1 ] = p_mtx->m[ 0 ][ 0 ] * p_mtx->m[ 2 ][ 2 ] - p_mtx->m[ 0 ][ 2 ] * p_mtx->m[ 2 ][ 0 ];
	p_inv_mtx->m[ 1 ][ 2 ] = p_mtx->m[ 0 ][ 2 ] * p_mtx->m[ 1 ][ 0 ] - p_mtx->m[ 0 ][ 0 ] * p_mtx->m[ 1 ][ 2 ];
	p_inv_mtx->m[ 1 ][ 3 ] = 0.f;
	p_inv_mtx->m[ 2 ][ 0 ] = p_mtx->m[ 1 ][ 0 ] * p_mtx->m[ 2 ][ 1 ] - p_mtx->m[ 1 ][ 1 ] * p_mtx->m[ 2 ][ 0 ];
	p_inv_mtx->m[ 2 ][ 1 ] = p_mtx->m[ 0 ][ 1 ] * p_mtx->m[ 2 ][ 0 ] - p_mtx->m[ 0 ][ 0 ] * p_mtx->m[ 2 ][ 1 ];
	p_inv_mtx->m[ 2 ][ 2 ] = p_mtx->m[ 0 ][ 0 ] * p_mtx->m[ 1 ][ 1 ] - p_mtx->m[ 0 ][ 1 ] * p_mtx->m[ 1 ][ 0 ];
	p_inv_mtx->m[ 2 ][ 3 ] = 0.f;
	p_inv_mtx->m[ 3 ][ 0 ] = 0.f;
	p_inv_mtx->m[ 3 ][ 1 ] = 0.f;
	p_inv_mtx->m[ 3 ][ 2 ] = 0.f;
	p_inv_mtx->m[ 3 ][ 3 ] = 1.f;

	const f32	fDet = p_mtx->m[ 0 ][ 0 ] * p_inv_mtx->m[ 0 ][ 0 ] + p_mtx->m[ 0 ][ 1 ] * p_inv_mtx->m[ 1 ][ 0 ] + p_mtx->m[ 0 ][ 2 ] * p_inv_mtx->m[ 2 ][ 0 ];

	if (ABSF( fDet ) != 0.f)
	{
		u32 r, c;
		f32	fInvDet = 1.0f / fDet;

		for (r = 0; r < 3; r++)
		{
			for (c = 0; c < 3; c++)
			{
				p_inv_mtx->m[ r ][ c ] *= fInvDet;
			}
		}
	}
}

//*************************************************************************************
//	
//*************************************************************************************
void	MTX_Transpose( const M44 * const p_mtx, M44 * const p_trans_mtx )
{
	u32	r, c;

	for (r = 0; r < 4; r++)
	{
		for (c = 0; c < 4; c++)
		{
			p_trans_mtx->m[ r ][ c ] = p_mtx->m[ c ][ r ];
		}
	}
}

//*************************************************************************************
//	
//*************************************************************************************
void	MTX_Scale( M44 * const p_mtx, f32 xval, f32 yval, f32 zval )
{
	p_mtx->m[ 0 ][ 0 ] *= xval;
	p_mtx->m[ 1 ][ 0 ] *= xval;
	p_mtx->m[ 2 ][ 0 ] *= xval;

	p_mtx->m[ 0 ][ 1 ] *= yval;
	p_mtx->m[ 1 ][ 1 ] *= yval;
	p_mtx->m[ 2 ][ 1 ] *= yval;

	p_mtx->m[ 0 ][ 2 ] *= zval;
	p_mtx->m[ 1 ][ 2 ] *= zval;
	p_mtx->m[ 2 ][ 2 ] *= zval;
}

//*************************************************************************************
//	
//*************************************************************************************
void	MTX_NormaliseScale( M44 * const p_mtx )
{
/*	MTX_SetAt( p_mtx, MTX_GetAt( p_mtx ).Normal() );
	MTX_SetUp( GetUp().Normal() );
	MTX_SetRight( GetRight().Normal() );*/
}

//*************************************************************************************
//	
//*************************************************************************************
void	MTX_TransformCoord( const M44 * const p_mtx, const V3 * const p_src, V3 * const p_dst )
{
	p_dst->x = p_mtx->m[ 0 ][ 0 ] * p_src->x + p_mtx->m[ 0 ][ 1 ] * p_src->y + p_mtx->m[ 0 ][ 2 ] * p_src->z + p_mtx->m[ 3 ][ 0 ];
	p_dst->y = p_mtx->m[ 1 ][ 0 ] * p_src->x + p_mtx->m[ 1 ][ 1 ] * p_src->y + p_mtx->m[ 1 ][ 2 ] * p_src->z + p_mtx->m[ 3 ][ 1 ];
	p_dst->z = p_mtx->m[ 2 ][ 0 ] * p_src->x + p_mtx->m[ 2 ][ 1 ] * p_src->y + p_mtx->m[ 2 ][ 2 ] * p_src->z + p_mtx->m[ 3 ][ 2 ];
}

void	MTX_TransformNormal( const M44 * const p_mtx, const V3 * const p_src, V3 * const p_dst )
{
	p_dst->x = p_mtx->m[ 0 ][ 0 ] * p_src->x + p_mtx->m[ 0 ][ 1 ] * p_src->y + p_mtx->m[ 0 ][ 2 ] * p_src->z;
	p_dst->y = p_mtx->m[ 1 ][ 0 ] * p_src->x + p_mtx->m[ 1 ][ 1 ] * p_src->y + p_mtx->m[ 1 ][ 2 ] * p_src->z;
	p_dst->z = p_mtx->m[ 2 ][ 0 ] * p_src->x + p_mtx->m[ 2 ][ 1 ] * p_src->y + p_mtx->m[ 2 ][ 2 ] * p_src->z;
}

//*******************************  END OF FILE  ************************************
