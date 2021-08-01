/***********************************************************************************

  Module :	Rasteriser.c

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
#include "Object.h"
#include "Graphics.h"
#include "Rasteriser.h"

//**********************************************************************************
//   Local Macros
//**********************************************************************************
typedef struct
{
	s32		x;
	s32		y;
	RGBA	col;
} SScreenVert;

typedef struct
{
	s32		left;
	s32		inv_delta_y[ 3 ];
	s32		slopeX[ 2 ];
	s32		x[ 2 ];
	RGBA	slopeC[ 2 ];
	RGBA	c[ 2 ];
} SScanConvData;

typedef struct
{
	s32		y;
	s32		x[ 2 ];
	RGBA	c[2];
} SScanline;

//**********************************************************************************
//   Local Constants
//**********************************************************************************
#define	SCREEN_HALF_WIDTH	( LINE_SIZE >> 1 )
#define	SCREEN_HALF_HEIGHT	( SCREEN_HEIGHT >> 1 )

//**********************************************************************************
//   Static Prototypes
//**********************************************************************************

//**********************************************************************************
//   Global Variables
//**********************************************************************************

//**********************************************************************************
//   Static Variables
//**********************************************************************************
static s16				s_LightAngle[ 3 ] = { 0, 0, 0 };
static const f32		s_FocalLength = 400.f;
static const f32		s_CameraZOffset = 350.f;
static const s32		s_ScreenMidX = SCREEN_HALF_WIDTH;
static const s32		s_ScreenMidY = SCREEN_HALF_HEIGHT;
static SScanConvData	s_ScanData;
static SScanline		s_ScanLine;

//**********************************************************************************
//   Class Definition
//**********************************************************************************

//*************************************************************************************
//	
//*************************************************************************************
void	ProjectPoint( V3 * p, SScreenVert * sv )
{
	f32	invZ;
	f32	z = p->z + s_CameraZOffset;

	if ( z != 0.f )
	{
		invZ = 1.0f / z;
	}
	else
	{
		invZ = 1.f;
	}

	sv->x = s_ScreenMidX + ( s32 )( s_FocalLength * p->x * invZ );
	sv->y = s_ScreenMidY + ( s32 )( s_FocalLength * p->y * invZ );
}

//*************************************************************************************
//	
//*************************************************************************************
void	RenderScanLineFlat( SScanline * data, u16 rgb )
{
	u16 *	bitsFrame = pgGetFrame();
	s32		xe, xs;
	s32		i;

	xs = ( data->x[ 0 ] / ONE );
	xe = ( data->x[ 1 ] / ONE ) - 1;

	if ( xe < xs )
	{
		return;
	}

	if ( data->y > SCREEN_HEIGHT || data->y < 0 )
	{
		return;
	}

	if ( xs > SCREEN_WIDTH || xs < 0 )
	{
		return;
	}

	if ( xe > SCREEN_WIDTH || xe < 0 )
	{
		return;
	}

	bitsFrame = bitsFrame + ( data->y * LINE_SIZE ) + xs;

	for ( i = xs; i <= xe; i++ )
	{
		*bitsFrame++ = rgb;
	}
}

//*************************************************************************************
//	
//*************************************************************************************
void	RenderScanLineGouraud( SScanline * data )
{
	u16 *	bitsFrame = pgGetFrame();
	s32		inv_delta_x;
	RGBA	slopeC;
	s32		xe, xs;
	s32		i;

	xs = ( data->x[ 0 ] / ONE );
	xe = ( data->x[ 1 ] / ONE ) - 1;

	if ( xe < xs )
	{
		return;
	}

	if ( data->y > SCREEN_HEIGHT || data->y < 0 )
	{
		return;
	}

	if ( xs > SCREEN_WIDTH || xs < 0 )
	{
		return;
	}

	if ( xe > SCREEN_WIDTH || xe < 0 )
	{
		return;
	}

	inv_delta_x = ( data->x[ 1 ] - data->x[ 0 ] );

	if ( inv_delta_x != 0 )
	{
		inv_delta_x = ( ONE * ONE ) / inv_delta_x;
	}
	else
	{
		inv_delta_x = 1.f;
	}

	slopeC.r = ( ( data->c[ 1 ].r - data->c[ 0 ].r ) * inv_delta_x ) / ONE;
	slopeC.g = ( ( data->c[ 1 ].g - data->c[ 0 ].g ) * inv_delta_x ) / ONE;
	slopeC.b = ( ( data->c[ 1 ].b - data->c[ 0 ].b ) * inv_delta_x ) / ONE;

	bitsFrame = bitsFrame + ( data->y * LINE_SIZE ) + xs;

	for ( i = xs; i <= xe; i++ )
	{
		u8	r = ( data->c[ 0 ].r / ONE );
		u8	g = ( data->c[ 0 ].g / ONE );
		u8	b = ( data->c[ 0 ].b / ONE );

		*bitsFrame++ = RGB( r, g, b );

		data->c[ 0 ].r += slopeC.r;
		data->c[ 0 ].g += slopeC.g;
		data->c[ 0 ].b += slopeC.b;
	}
}

//*************************************************************************************
//	
//*************************************************************************************
void	RenderTriangleFlat( SScreenVert * p1, SScreenVert * p2, SScreenVert * p3 )
{
	SScreenVert *	sp[ 3 ], *temp;
	s32				v1[ 2 ], v2[ 2 ], sub;
	s32				yStart, yEnd;
	s32				i;

	sp[ 0 ] = p1;
	sp[ 1 ] = p2;
	sp[ 2 ] = p3;

	if ( sp[ 0 ]->y > sp[ 1 ]->y )
	{
		temp = sp[ 0 ];
		sp[ 0 ]	= sp[ 1 ];
		sp[ 1 ]	= temp;
	}

	if ( sp[ 0 ]->y > sp[ 2 ]->y )
	{
		temp = sp[ 0 ];
		sp[ 0 ]	= sp[ 2 ];
		sp[ 2 ]	= temp;
	}

	if ( sp[ 1 ]->y > sp[ 2 ]->y )
	{
		temp = sp[ 1 ];
		sp[ 1 ]	= sp[ 2 ];
		sp[ 2 ]	= temp;
	}

	s_ScanData.inv_delta_y[ 0 ] = ( sp[ 2 ]->y - sp[ 0 ]->y );

	if ( s_ScanData.inv_delta_y[ 0 ] == 0.f )
	{
		return;
	}

	s_ScanData.inv_delta_y[ 0 ] = ( ONE / s_ScanData.inv_delta_y[ 0 ] );

	s_ScanData.inv_delta_y[ 1 ] = ( sp[ 1 ]->y - sp[ 0 ]->y );
	s_ScanData.inv_delta_y[ 2 ] = ( sp[ 2 ]->y - sp[ 1 ]->y );

	if ( s_ScanData.inv_delta_y[ 1 ] != 0 )
	{
		s_ScanData.inv_delta_y[ 1 ] = ( ONE / s_ScanData.inv_delta_y[ 1 ] );
	}

	if ( s_ScanData.inv_delta_y[ 2 ] != 0 )
	{
		s_ScanData.inv_delta_y[ 2 ] = ( ONE / s_ScanData.inv_delta_y[ 2 ] );
	}

	v1[ 0 ] = ( sp[ 0 ]->x - sp[ 2 ]->x );
	v1[ 1 ] = ( sp[ 0 ]->y - sp[ 2 ]->y );
	v2[ 0 ] = ( sp[ 1 ]->x - sp[ 0 ]->x );
	v2[ 1 ] = ( sp[ 1 ]->y - sp[ 0 ]->y );

	s32	side = v1[ 0 ] * v2[ 1 ] - v1[ 1 ] * v2[ 0 ];

	if ( IsGreaterThan( side, 0.f ) )
	{
		s_ScanData.left = 0;
	}
	else
	{
		s_ScanData.left = 1;
	}

	u16	color = RGB( p1->col.r, p1->col.g, p1->col.b );

	s_ScanData.x[ 0 ] = ( sp[ 0 ]->x * ONE );
	s_ScanData.slopeX[ 0 ] = ( ( sp[ 2 ]->x - sp[ 0 ]->x ) * s_ScanData.inv_delta_y[ 0 ] );

	if ( s_ScanData.inv_delta_y[ 1 ] != 0 )
	{
		s_ScanData.x[ 1 ] = ( sp[ 0 ]->x * ONE );
		s_ScanData.slopeX[ 1 ] = ( ( sp[ 1 ]->x - sp[ 0 ]->x ) * s_ScanData.inv_delta_y[ 1 ] );

		yStart = sp[ 0 ]->y;
		yEnd = sp[ 1 ]->y - 1;

		sub = yStart - sp[ 0 ]->y;

		s_ScanData.x[ 0 ] += s_ScanData.slopeX[ 0 ] * sub;
		s_ScanData.x[ 1 ] += s_ScanData.slopeX[ 1 ] * sub;

		for ( i = yStart; i <= yEnd; i++)
		{
			s_ScanLine.y = i;
			s_ScanLine.x[ s_ScanData.left ] = s_ScanData.x[ 0 ];
			s_ScanLine.x[ !s_ScanData.left ] = s_ScanData.x[ 1 ];

			RenderScanLineFlat( &s_ScanLine, color );	

			s_ScanData.x[ 0 ] += s_ScanData.slopeX[ 0 ];
			s_ScanData.x[ 1 ] += s_ScanData.slopeX[ 1 ];
		}
	}

	if ( s_ScanData.inv_delta_y[ 2 ] != 0 )
	{
		if ( s_ScanData.inv_delta_y[ 1 ] != 0 )
		{
			s32	dy = ( sp[ 1 ]->y - sp[ 0 ]->y );

			s_ScanData.x[ 0 ] = ( sp[ 0 ]->x * ONE ) + ( s_ScanData.slopeX[ 0 ] * dy );
		}

		s_ScanData.x[ 1 ] = ( sp[ 1 ]->x * ONE );
		s_ScanData.slopeX[ 1 ] = ( ( sp[ 2 ]->x - sp[ 1 ]->x ) * s_ScanData.inv_delta_y[ 2 ] );

		yStart = sp[ 1 ]->y;
		yEnd = sp[ 2 ]->y - 1;

		sub = yStart - sp[ 1 ]->y;

		s_ScanData.x[ 0 ] += s_ScanData.slopeX[ 0 ] * sub;
		s_ScanData.x[ 1 ] += s_ScanData.slopeX[ 1 ] * sub;

		for ( i = yStart; i <= yEnd; i++)
		{
			s_ScanLine.y = i;
			s_ScanLine.x[ s_ScanData.left ]	= s_ScanData.x[ 0 ];
			s_ScanLine.x[ !s_ScanData.left ] = s_ScanData.x[ 1 ];

			RenderScanLineFlat( &s_ScanLine, color );

			s_ScanData.x[ 0 ] += s_ScanData.slopeX[ 0 ];
			s_ScanData.x[ 1 ] += s_ScanData.slopeX[ 1 ];
		}
	}
}

//*************************************************************************************
//	
//*************************************************************************************
void	RenderTriangleGouraud( SScreenVert * p1, SScreenVert * p2, SScreenVert * p3 )
{
	SScreenVert *	sp[ 3 ], *temp;
	s32				v1[ 2 ], v2[ 2 ], sub;
	s32				yStart, yEnd;
	s32				i;

	sp[ 0 ] = p1;
	sp[ 1 ] = p2;
	sp[ 2 ] = p3;

	if ( sp[ 0 ]->y > sp[ 1 ]->y )
	{
		temp = sp[ 0 ];
		sp[ 0 ]	= sp[ 1 ];
		sp[ 1 ]	= temp;
	}

	if ( sp[ 0 ]->y > sp[ 2 ]->y )
	{
		temp = sp[ 0 ];
		sp[ 0 ]	= sp[ 2 ];
		sp[ 2 ]	= temp;
	}

	if ( sp[ 1 ]->y > sp[ 2 ]->y )
	{
		temp = sp[ 1 ];
		sp[ 1 ]	= sp[ 2 ];
		sp[ 2 ]	= temp;
	}

	s_ScanData.inv_delta_y[ 0 ] = ( sp[ 2 ]->y - sp[ 0 ]->y );

	if ( s_ScanData.inv_delta_y[ 0 ] == 0.f )
	{
		return;
	}

	s_ScanData.inv_delta_y[ 0 ] = ( ONE / s_ScanData.inv_delta_y[ 0 ] );

	s_ScanData.inv_delta_y[ 1 ] = ( sp[ 1 ]->y - sp[ 0 ]->y );
	s_ScanData.inv_delta_y[ 2 ] = ( sp[ 2 ]->y - sp[ 1 ]->y );

	if ( s_ScanData.inv_delta_y[ 1 ] != 0 )
	{
		s_ScanData.inv_delta_y[ 1 ] = ( ONE / s_ScanData.inv_delta_y[ 1 ] );
	}

	if ( s_ScanData.inv_delta_y[ 2 ] != 0 )
	{
		s_ScanData.inv_delta_y[ 2 ] = ( ONE / s_ScanData.inv_delta_y[ 2 ] );
	}

	v1[ 0 ] = ( sp[ 0 ]->x - sp[ 2 ]->x );
	v1[ 1 ] = ( sp[ 0 ]->y - sp[ 2 ]->y );
	v2[ 0 ] = ( sp[ 1 ]->x - sp[ 0 ]->x );
	v2[ 1 ] = ( sp[ 1 ]->y - sp[ 0 ]->y );

	s32	side = v1[ 0 ] * v2[ 1 ] - v1[ 1 ] * v2[ 0 ];

	if ( IsGreaterThan( side, 0.f ) )
	{
		s_ScanData.left = 0;
	}
	else
	{
		s_ScanData.left = 1;
	}

	s_ScanData.x[ 0 ] = ( sp[ 0 ]->x * ONE );
	s_ScanData.slopeX[ 0 ] = ( ( sp[ 2 ]->x - sp[ 0 ]->x ) * s_ScanData.inv_delta_y[ 0 ] );

	s_ScanData.slopeC[ 0 ].r = ( sp[ 2 ]->col.r - sp[ 0 ]->col.r ) * s_ScanData.inv_delta_y[ 0 ];
	s_ScanData.slopeC[ 0 ].g = ( sp[ 2 ]->col.g - sp[ 0 ]->col.g ) * s_ScanData.inv_delta_y[ 0 ];
	s_ScanData.slopeC[ 0 ].b = ( sp[ 2 ]->col.b - sp[ 0 ]->col.b ) * s_ScanData.inv_delta_y[ 0 ];

	s_ScanData.c[ 0 ].r = sp[ 0 ]->col.r * ONE;
	s_ScanData.c[ 0 ].g = sp[ 0 ]->col.g * ONE;
	s_ScanData.c[ 0 ].b = sp[ 0 ]->col.b * ONE;

	if ( s_ScanData.inv_delta_y[ 1 ] != 0 )
	{
		s_ScanData.x[ 1 ] = ( sp[ 0 ]->x * ONE );
		s_ScanData.slopeX[ 1 ] = ( ( sp[ 1 ]->x - sp[ 0 ]->x ) * s_ScanData.inv_delta_y[ 1 ] );

		s_ScanData.slopeC[ 1 ].r = ( sp[ 1 ]->col.r - sp[ 0 ]->col.r ) * s_ScanData.inv_delta_y[ 1 ];
		s_ScanData.slopeC[ 1 ].g = ( sp[ 1 ]->col.g - sp[ 0 ]->col.g ) * s_ScanData.inv_delta_y[ 1 ];
		s_ScanData.slopeC[ 1 ].b = ( sp[ 1 ]->col.b - sp[ 0 ]->col.b ) * s_ScanData.inv_delta_y[ 1 ];

		s_ScanData.c[ 1 ].r = sp[ 0 ]->col.r * ONE;
		s_ScanData.c[ 1 ].g = sp[ 0 ]->col.g * ONE;
		s_ScanData.c[ 1 ].b = sp[ 0 ]->col.b * ONE;

		yStart = sp[ 0 ]->y;
		yEnd = sp[ 1 ]->y - 1;

		sub = yStart - sp[ 0 ]->y;

		s_ScanData.x[ 0 ] += s_ScanData.slopeX[ 0 ] * sub;
		s_ScanData.x[ 1 ] += s_ScanData.slopeX[ 1 ] * sub;

		s_ScanData.c[ 0 ].r += s_ScanData.slopeC[ 0 ].r * sub;
		s_ScanData.c[ 0 ].g += s_ScanData.slopeC[ 0 ].g * sub;
		s_ScanData.c[ 0 ].b += s_ScanData.slopeC[ 0 ].b * sub;

		s_ScanData.c[ 1 ].r += s_ScanData.slopeC[ 1 ].r * sub;
		s_ScanData.c[ 1 ].g += s_ScanData.slopeC[ 1 ].g * sub;
		s_ScanData.c[ 1 ].b += s_ScanData.slopeC[ 1 ].b * sub;

		for ( i = yStart; i <= yEnd; i++)
		{
			s_ScanLine.y = i;
			s_ScanLine.x[ s_ScanData.left ] = s_ScanData.x[ 0 ];
			s_ScanLine.x[ !s_ScanData.left ] = s_ScanData.x[ 1 ];
			s_ScanLine.c[ s_ScanData.left ] = s_ScanData.c[ 0 ];
			s_ScanLine.c[ !s_ScanData.left ] = s_ScanData.c[ 1 ];

			RenderScanLineGouraud( &s_ScanLine );	

			s_ScanData.x[ 0 ] += s_ScanData.slopeX[ 0 ];
			s_ScanData.x[ 1 ] += s_ScanData.slopeX[ 1 ];

			s_ScanData.c[ 0 ].r += s_ScanData.slopeC[ 0 ].r;
			s_ScanData.c[ 0 ].g += s_ScanData.slopeC[ 0 ].g;
			s_ScanData.c[ 0 ].b += s_ScanData.slopeC[ 0 ].b;

			s_ScanData.c[ 1 ].r += s_ScanData.slopeC[ 1 ].r;
			s_ScanData.c[ 1 ].g += s_ScanData.slopeC[ 1 ].g;
			s_ScanData.c[ 1 ].b += s_ScanData.slopeC[ 1 ].b;
		}
	}

	if ( s_ScanData.inv_delta_y[ 2 ] != 0 )
	{
		if ( s_ScanData.inv_delta_y[ 1 ] != 0 )
		{
			s32	dy = ( sp[ 1 ]->y - sp[ 0 ]->y );

			s_ScanData.x[ 0 ] = ( sp[ 0 ]->x * ONE ) + ( s_ScanData.slopeX[ 0 ] * dy );
			s_ScanData.c[ 0 ].r = ( sp[ 0 ]->col.r * ONE ) + s_ScanData.slopeC[ 0 ].r * dy;
			s_ScanData.c[ 0 ].g = ( sp[ 0 ]->col.g * ONE ) + s_ScanData.slopeC[ 0 ].g * dy;
			s_ScanData.c[ 0 ].b = ( sp[ 0 ]->col.b * ONE ) + s_ScanData.slopeC[ 0 ].b * dy;
		}

		s_ScanData.x[ 1 ] = ( sp[ 1 ]->x * ONE );
		s_ScanData.slopeX[ 1 ] = ( ( sp[ 2 ]->x - sp[ 1 ]->x ) * s_ScanData.inv_delta_y[ 2 ] );

		s_ScanData.slopeC[ 1 ].r = ( sp[ 2 ]->col.r - sp[ 1 ]->col.r) * s_ScanData.inv_delta_y[ 2 ];
		s_ScanData.slopeC[ 1 ].g = ( sp[ 2 ]->col.g - sp[ 1 ]->col.g) * s_ScanData.inv_delta_y[ 2 ];
		s_ScanData.slopeC[ 1 ].b = ( sp[ 2 ]->col.b - sp[ 1 ]->col.b) * s_ScanData.inv_delta_y[ 2 ];

		s_ScanData.c[ 1 ].r = sp[ 1 ]->col.r * ONE;
		s_ScanData.c[ 1 ].g = sp[ 1 ]->col.g * ONE;
		s_ScanData.c[ 1 ].b = sp[ 1 ]->col.b * ONE;

		yStart = sp[ 1 ]->y;
		yEnd = sp[ 2 ]->y - 1;

		sub = yStart - sp[ 1 ]->y;

		s_ScanData.x[ 0 ] += s_ScanData.slopeX[ 0 ] * sub;
		s_ScanData.x[ 1 ] += s_ScanData.slopeX[ 1 ] * sub;

		s_ScanData.c[ 0 ].r += s_ScanData.slopeC[ 0 ].r * sub;
		s_ScanData.c[ 0 ].g += s_ScanData.slopeC[ 0 ].g * sub;
		s_ScanData.c[ 0 ].b += s_ScanData.slopeC[ 0 ].b * sub;

		s_ScanData.c[ 1 ].r += s_ScanData.slopeC[ 1 ].r * sub;
		s_ScanData.c[ 1 ].g += s_ScanData.slopeC[ 1 ].g * sub;
		s_ScanData.c[ 1 ].b += s_ScanData.slopeC[ 1 ].b * sub;

		for ( i = yStart; i <= yEnd; i++)
		{
			s_ScanLine.y = i;
			s_ScanLine.x[ s_ScanData.left ]	= s_ScanData.x[ 0 ];
			s_ScanLine.x[ !s_ScanData.left ] = s_ScanData.x[ 1 ];
			s_ScanLine.c[ s_ScanData.left ] = s_ScanData.c[ 0 ];
			s_ScanLine.c[ !s_ScanData.left ] = s_ScanData.c[ 1 ];

			RenderScanLineGouraud( &s_ScanLine );

			s_ScanData.x[ 0 ] += s_ScanData.slopeX[ 0 ];
			s_ScanData.x[ 1 ] += s_ScanData.slopeX[ 1 ];

			s_ScanData.c[ 0 ].r += s_ScanData.slopeC[ 0 ].r;
			s_ScanData.c[ 0 ].g += s_ScanData.slopeC[ 0 ].g;
			s_ScanData.c[ 0 ].b += s_ScanData.slopeC[ 0 ].b;

			s_ScanData.c[ 1 ].r += s_ScanData.slopeC[ 1 ].r;
			s_ScanData.c[ 1 ].g += s_ScanData.slopeC[ 1 ].g;
			s_ScanData.c[ 1 ].b += s_ScanData.slopeC[ 1 ].b;
		}
	}
}

//*************************************************************************************
//	
//*************************************************************************************
void	RenderObject( SObject * p_obj, M44 * p_mtx, bool flat )
{
	f32		biggest, dot, light_dot;
	s32		a, b, c, poly, temp, i, j;
	V3		normal;
	RGBA	light_col;
	V3		light_vector;

	s_LightAngle[ 0 ] -= 3;
	s_LightAngle[ 1 ] -= 2;
	s_LightAngle[ 2 ] += 1;

	if ( s_LightAngle[ 0 ] >= 360 )	s_LightAngle[ 0 ] -= 360;
	if ( s_LightAngle[ 0 ] < 0 )	s_LightAngle[ 0 ] += 360;
	if ( s_LightAngle[ 1 ] >= 360 )	s_LightAngle[ 1 ] -= 360;
	if ( s_LightAngle[ 1 ] < 0 )	s_LightAngle[ 1 ] += 360;
	if ( s_LightAngle[ 2 ] >= 360 )	s_LightAngle[ 2 ] -= 360;
	if ( s_LightAngle[ 2 ] < 0 )	s_LightAngle[ 2 ] += 360;

	light_vector.x = SINF( s_LightAngle[ 0 ] );
	light_vector.y = 0.f;//COSF( s_LightAngle[ 1 ] );
	light_vector.z = COSF( s_LightAngle[ 2 ] );

//	u32	lx = SCREEN_HALF_WIDTH + ( s32 )( 100.f * -light_vector.x );
//	u32	ly = SCREEN_HALF_HEIGHT + ( s32 )( 100.f * light_vector.z );
//	pgPrint( lx, ly, RGB( 255, 255, 0 ), "*", 0xff );


	for ( i = 0; i < p_obj->num_verts; ++i )
	{
		MTX_TransformCoord( p_mtx, &p_obj->verts[ i ].pos, &p_obj->verts[ i ].trans_pos );

		if ( flat == false )
		{
			MTX_TransformNormal( p_mtx, &p_obj->verts[ i ].normal, &normal );

			light_dot = ( V3_Dot( &normal, &light_vector ) + 1.f ) * 0.5f;

			if ( IsLessThan( light_dot, 0.f ) )		light_dot = 0.f;
			if ( IsGreaterThan( light_dot, 1.f ) )	light_dot = 1.f;

			light_col.r = ( s32 )( p_obj->diffuse.r * light_dot );
			light_col.g = ( s32 )( p_obj->diffuse.g * light_dot );
			light_col.b = ( s32 )( p_obj->diffuse.b * light_dot );

			p_obj->verts[ i ].col = light_col;
		}
	}

	p_obj->num_visible = 0;

	for ( i = 0; i < p_obj->num_faces; ++i )
	{
		MTX_TransformNormal( p_mtx, &p_obj->faces[ i ].normal, &normal );

		dot = V3_Dot( &normal, &gAtVector );

		if ( IsGreaterThan( dot, 0.f ) == true )
		{
			if ( flat == true )
			{
				light_dot = ( V3_Dot( &normal, &light_vector ) + 1.f ) * 0.5f;

				if ( IsLessThan( light_dot, 0.f ) )		light_dot = 0.f;
				if ( IsGreaterThan( light_dot, 1.f ) )	light_dot = 1.f;

				light_col.r = ( s32 )( p_obj->diffuse.r * light_dot );
				light_col.g = ( s32 )( p_obj->diffuse.g * light_dot );
				light_col.b = ( s32 )( p_obj->diffuse.b * light_dot );

				p_obj->faces[ i ].col = light_col;
			}

			a = p_obj->faces[ i ].verts[ 0 ];
			b = p_obj->faces[ i ].verts[ 1 ];
			c = p_obj->faces[ i ].verts[ 2 ];

			p_obj->faces[ i ].centre_z = ( p_obj->verts[ a ].trans_pos.z + p_obj->verts[ b ].trans_pos.z + p_obj->verts[ c ].trans_pos.z ) / 3;

			p_obj->visible[ p_obj->num_visible++ ] = i;
		}
	}

	if ( p_obj->num_visible > 0 )
	{
		SScreenVert	p1, p2, p3;

		for ( i = 0; i < p_obj->num_visible - 1; i++)
		{
			poly = p_obj->visible[ i ];
			biggest	= p_obj->faces[ poly ].centre_z;

			for ( j = i + 1; j < p_obj->num_visible; j++ )
			{
				poly = p_obj->visible[ j ];

				if ( IsGreaterThan( p_obj->faces[ poly ].centre_z, biggest ) )
				{
					temp = p_obj->visible[ j ];
					p_obj->visible[ j ] = p_obj->visible[ i ];
					p_obj->visible[ i ] = temp;
				}
			}
		}

		for ( i = 0; i < p_obj->num_visible; i++ )
		{
			poly = p_obj->visible[ i ];

			a = p_obj->faces[ poly ].verts[ 0 ];
			b = p_obj->faces[ poly ].verts[ 1 ];
			c = p_obj->faces[ poly ].verts[ 2 ];

			ProjectPoint( &p_obj->verts[ a ].trans_pos, &p1 );
			ProjectPoint( &p_obj->verts[ b ].trans_pos, &p2 );
			ProjectPoint( &p_obj->verts[ c ].trans_pos, &p3 );

			if ( flat == true )
			{
				p1.col = p_obj->faces[ poly ].col;
				p2.col = p_obj->faces[ poly ].col;
				p3.col = p_obj->faces[ poly ].col;

				RenderTriangleFlat( &p1, &p2, &p3 );
			}
			else
			{
				p1.col = p_obj->verts[ a ].col;
				p2.col = p_obj->verts[ b ].col;
				p3.col = p_obj->verts[ c ].col;

				RenderTriangleGouraud( &p1, &p2, &p3 );
			}
		}
	}
}

//*******************************  END OF FILE  ************************************
