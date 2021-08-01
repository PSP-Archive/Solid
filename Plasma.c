/***********************************************************************************

  Module :	Plasma.c

  Description :	

  Last Modified $Date: $

  $Revision: $

  Copyright (C) 29 May 2005 T Swann

***********************************************************************************/

//**********************************************************************************
//   Include Files
//**********************************************************************************
#include "Types.h"
#include "Maths.h"
#include "Graphics.h"

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
static u8			s_SinPosX = 9;
static u8			s_SinPosY = 32;

static f32			s_FadeVal = 0.f;

static const RGBA	s_PlasmaCol[ 2 ] =
{
	{ 186, 236, 182 },
	{ 216, 181, 243 },
};

//**********************************************************************************
//   Class Definition
//**********************************************************************************
bool	PLASMA_Render( u32 fade_in )
{
	int		x, y;
	u16 *	p_frame = (u16 *)pgGetVramAddr( 0, 0 );
	u8		col;
	u16		rgb;
	u32		x_sin_offset;
	u32		y_sin_offset;
	u32		sin_val;
	bool	finished = false;

	if ( fade_in != 0 )
	{
		s_FadeVal += 0.01f;

		if ( IsGreaterThan( s_FadeVal, 1.f ) == true )
		{
			s_FadeVal = 1.f;

			finished = true;
		}
	}
	else
	{
		s_FadeVal -= 0.01f;

		if ( IsLessThan( s_FadeVal, 0.f ) == true )
		{
			s_FadeVal = 0.f;

			finished = true;
		}
	}

	const u16	r0 = ( s32 )( s_FadeVal * s_PlasmaCol[ 0 ].r );
	const u16	g0 = ( s32 )( s_FadeVal * s_PlasmaCol[ 0 ].g );
	const u16	b0 = ( s32 )( s_FadeVal * s_PlasmaCol[ 0 ].b );

	const u16	r1 = ( s32 )( s_FadeVal * s_PlasmaCol[ 1 ].r );
	const u16	g1 = ( s32 )( s_FadeVal * s_PlasmaCol[ 1 ].g );
	const u16	b1 = ( s32 )( s_FadeVal * s_PlasmaCol[ 1 ].b );

	for ( y = 0; y < SCREEN_HEIGHT; ++y )
	{
		for ( x = 0; x < ( SCREEN_WIDTH >> 2 ); ++x )
		{
			if ( y & 1 )
			{
				x_sin_offset = s_SinPosX + x;
				y_sin_offset = s_SinPosY + y;

				sin_val = SINU( y_sin_offset + ( SINU( x_sin_offset ) >> 10 ) );

				col = 0x80 + ( ( sin_val >> 8 ) >> 1 );

				rgb = RGB( ( r0 * col ) >> 8, ( g0 * col ) >> 8, ( b0 * col ) >> 8 );
			}
			else
			{
				x_sin_offset = ( s_SinPosX << 2 ) + x + 301;
				y_sin_offset = ( s_SinPosY << 1 ) + y;

				sin_val = SINU( y_sin_offset + ( SINU( x_sin_offset ) >> 9 ) );

				col = 0x80 + ( ( sin_val >> 8 ) >> 1 );

				rgb = RGB( ( r1 * col ) >> 8, ( g1 * col ) >> 8, ( b1 * col ) >> 8 );
			}

			const u32	write_data = rgb + ( rgb << 16 );
			u32 * const	p_write_pos = ( u32 * )( p_frame + ( x << 2 ) + ( y * LINE_SIZE ) );

			p_write_pos[ 0 ] = write_data;
			p_write_pos[ 1 ] = write_data;
		}
	}

	s_SinPosX += 2;
	s_SinPosY += 4;

	return finished;
}

//*******************************  END OF FILE  ************************************
