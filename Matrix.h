/***********************************************************************************

  Module :	Matrix.h

  Description :	

  Last Modified $Date: $

  $Revision: $

  Copyright (C) 28 May 2005 T Swann

***********************************************************************************/

#ifndef _MATRIX_H_
#define _MATRIX_H_

//**********************************************************************************
//   Include Files
//**********************************************************************************
#include "Vector.h"

//**********************************************************************************
//   Macros
//**********************************************************************************

//**********************************************************************************
//   Types
//**********************************************************************************
typedef struct
{
	union
	{
		struct
		{
			f32	_11, _12, _13, _14;
			f32	_21, _22, _23, _24;
			f32	_31, _32, _33, _34;
			f32	_41, _42, _43, _44;
		};

		f32	m[ 4 ][ 4 ];
	};
} M44;

//**********************************************************************************
//   Constants
//**********************************************************************************

//**********************************************************************************
//   Class definitions
//**********************************************************************************
void	MTX_GetPos( const M44 * const p_mtx, V3 * const p_pos );
void	MTX_SetPos( M44 * const p_mtx, const V3 * const p_pos );

void	MTX_GetAt( const M44 * const p_mtx, V3 * const p_at );
void	MTX_SetAt( M44 * const p_mtx, const V3 * const p_at );

void	MTX_GetUp( const M44 * const p_mtx, V3 * const p_up );
void	MTX_SetUp( M44 * const p_mtx, const V3 * const p_up );

void	MTX_GetRight( const M44 * const p_mtx, V3 * const p_right );
void	MTX_SetRight( M44 * const p_mtx, const V3 * const p_right );

void	MTX_SetIdentity( M44 * const p_mtx );

void	MTX_SetTranslation( M44 * const p_mtx, f32 x, f32 y, f32 z );

void	MTX_SetScaling( M44 * const p_mtx, f32 sx, f32 sy, f32 sz );

void	MTX_SetRotationX( M44 * const p_mtx, u16 x_rot );
void	MTX_SetRotationY( M44 * const p_mtx, u16 y_rot );
void	MTX_SetRotationZ( M44 * const p_mtx, u16 z_rot );

void	MTX_SetRotationAxis( M44 * const p_mtx, const V3 * const p_axis, u16 degs );

void	MTX_Multiply( M44 * const p_mtx, const M44 * const p_matrix );

void	MTX_Inverse( const M44 * const p_mtx, M44 * const p_inv_mtx );

void	MTX_Transpose( const M44 * const p_mtx, M44 * const p_trans_mtx );

void	MTX_Scale( M44 * const p_mtx, f32 xval, f32 yval, f32 zval );

void	MTX_NormaliseScale( M44 * const p_mtx );

void	MTX_TransformCoord( const M44 * const p_mtx, const V3 * const p_src, V3 * const p_dst );
void	MTX_TransformNormal( const M44 * const p_mtx, const V3 * const p_src, V3 * const p_dst );

//**********************************************************************************
//   Externs
//**********************************************************************************

//**********************************************************************************
//   Prototypes
//**********************************************************************************

#endif /* _MATRIX_H_ */
