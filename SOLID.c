/***********************************************************************************

  Module :	SOLID.c

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
#include "Graphics.h"
#include "Vector.h"
#include "Matrix.h"
#include <string.h>
#include "Rasteriser.h"
#include "Controller.h"
#include "Object.h"
#include "Power.h"
#include "Plasma.h"
#include "Floor.h"

//**********************************************************************************
//   Local Macros
//**********************************************************************************

//**********************************************************************************
//   Local Constants
//**********************************************************************************
enum EStage
{
	STAGE_PLASMA_FADE_IN,
	STAGE_SHOW_TEXT1,
	STAGE_SHOW_TEXT2,
	STAGE_PLASMA_FADE_OUT,
	STAGE_OBJECT,
	STAGE_END,

	MAX_STAGES
};

//**********************************************************************************
//   Static Prototypes
//**********************************************************************************
void	__exit();

//**********************************************************************************
//   Global Variables
//**********************************************************************************
extern const u16	g_testData[];

//**********************************************************************************
//   Static Variables
//**********************************************************************************
static char		s_StringBuffer[ 1024 ];

static s32		s_StageTimes[ MAX_STAGES ] =
{
	-1,		//	STAGE_PLASMA_FADE_IN,
	-1,		//	STAGE_SHOW_TEXT1,
	-1,		//	STAGE_SHOW_TEXT2,
	-1,		//	STAGE_PLASMA_FADE_OUT,
	-1,//20000,	//	STAGE_OBJECT,
	200,	//	STAGE_END,
};

static u32		s_Stage = STAGE_PLASMA_FADE_IN;
static s32		s_StageTime = 0;

static u16		s_TextAlpha = 0;
static u32		s_TextOffset = 0;

static s16		s_ObjRotX = 90;
static s16		s_ObjRotY = 0;
static s16		s_ObjRotZ = 0;
static V3		s_ObjPos = { 0.f, -200.f, 0.f };

static u32		s_FloorStart = 256;

//*************************************************************************************
//	
//*************************************************************************************
void	Update()
{
	FLOOR_Update();
}

//*************************************************************************************
//	
//*************************************************************************************
void	RenderText( u8 stage, u8 alpha )
{
	switch ( stage )
	{
	case 2:
		{
			pgPrint4( ( 1 * s_TextOffset ) + 128 + 4, 160 + 4, 0x0000, "SOLID", alpha );
			pgPrint4( ( 1 * s_TextOffset ) + 128, 160, 0xffff, "SOLID", alpha );
		}
	case 1:
		{
			if ( stage == 2 )
			{
				alpha = 0xff;
			}

			pgPrint2( ( 3 * s_TextOffset ) + 200 + 2, 100 + 2, 0x0000, "PRESENTS", alpha );
			pgPrint2( ( 3 * s_TextOffset ) + 200, 100, 0xffff, "PRESENTS", alpha );
		}
	case 0:
		{
			if ( stage == 2 )
			{
				alpha = 0xff;
			}

			pgPrint4( ( 2 * s_TextOffset ) + 100 + 4, 60 + 4, 0x0000, "71M", alpha );
			pgPrint4( ( 2 * s_TextOffset ) + 100, 60, 0xffff, "71M", alpha );
		}
		break;
	}
}

//*************************************************************************************
//	
//*************************************************************************************
void	Render()
{
	s_StageTime += 1;

	switch ( s_Stage )
	{
	case STAGE_PLASMA_FADE_IN:
		{
			if ( PLASMA_Render( true ) == true )
			{
				s_StageTime = -1;
			}
		}
		break;

	case STAGE_SHOW_TEXT1:
		{
			s_TextAlpha += 2;

			if ( s_TextAlpha > 0xff )
			{
				s_TextAlpha = 0xff;
				s_StageTime = -1;
			}

			PLASMA_Render( true );

			RenderText( 1, s_TextAlpha );
		}
		break;

	case STAGE_SHOW_TEXT2:
		{
			s_TextAlpha += 4;

			if ( s_TextAlpha > 0xff )
			{
				s_TextAlpha = 0xff;
				s_StageTime = -1;
			}

			PLASMA_Render( true );

			RenderText( 2, s_TextAlpha );
		}
		break;

	case STAGE_PLASMA_FADE_OUT:
		{
			if ( PLASMA_Render( false ) == true )
			{
				s_StageTime = -1;
			}

			RenderText( 2, 0xff );

			s_TextOffset += 4;
		}
		break;

	case STAGE_OBJECT:
		{
			static s32			sin_pos = 90;
			static f32			magnitude = 200.f;
			static const f32	GROUND = 70.f;
			static bool			flat = true;
			static bool			key_down = false;

			if ( s_FloorStart > 128 )
			{
				s_FloorStart--;
				FLOOR_Render( s_FloorStart );
			}
			else
			{
				//s_ObjRotX -= 1;
				//s_ObjRotY += 3;
				s_ObjRotZ += 4;

				if ( s_ObjRotX >= 360 )	s_ObjRotX -= 360;
				if ( s_ObjRotX < 0 )	s_ObjRotX += 360;
				if ( s_ObjRotY >= 360 )	s_ObjRotY -= 360;
				if ( s_ObjRotY < 0 )	s_ObjRotY += 360;
				if ( s_ObjRotZ >= 360 )	s_ObjRotZ -= 360;
				if ( s_ObjRotZ < 0 )	s_ObjRotZ += 360;

				if ( IsLessThan( s_ObjPos.y, 50.f ) )	s_ObjPos.y += 5.f;

				f32	sin = ABSF( SINF( sin_pos ) );

				s_ObjPos.y = GROUND - ABSF( magnitude * sin );

				sin_pos += 16;

				if ( sin_pos >= 360 )	sin_pos -= 360;
				if ( sin_pos < 0 )		sin_pos += 360;

				if ( IsGreaterThan( magnitude, 0.f ) )	magnitude -= 4.f;
				else									magnitude = 0.f;

				FLOOR_Render( s_FloorStart );

				V3	scale = { 0.75f, 0.75f, 0.75f };

				if ( ( CTRL_GetKey() & KEY_CROSS ) )
				{
					if ( key_down == false )
					{
						flat = !flat;

						key_down = true;
					}
				}
				else
				{
					key_down = false;
				}

				OBJECT_Render( &g_DuckObject, &s_ObjPos, &scale, s_ObjRotX, s_ObjRotY, s_ObjRotZ, flat );
//				OBJECT_Render( &g_SphereObject, &s_ObjPos, 1.f, s_ObjRotX, s_ObjRotY, s_ObjRotZ, false );
			}
		}
		break;
	}

	if ( s_StageTime == s_StageTimes[ s_Stage ] )
	{
		s_Stage++;
		s_StageTime = 0;

		s_TextAlpha = 0x00;
		s_TextOffset = 0;

		if ( s_Stage == MAX_STAGES )
		{
			s_Stage = STAGE_PLASMA_FADE_IN;
		}
	}
}

//*************************************************************************************
//	
//*************************************************************************************
int		xmain()
{
	s32		fps, last_frame;

	pgInit();
	pgScreenFrame( 2, 0 );

	pgFillvram( 0x0000 );
	pgScreenFlipV();
	pgFillvram( 0x0000 );
	pgScreenFlipV();

	OBJECT_Init( &g_CubeObject );
	OBJECT_Init( &g_DuckObject );
	OBJECT_Init( &g_SphereObject );

	last_frame = fps = sceKernelLibcClock();

	while ( 1 )
	{
		fps = sceKernelLibcClock();

		Update();
		Render();

		s32	frame_time = ( fps - last_frame ) << 8;

		pgPrint( 0, 0, 0xffff, NumberToAscii( ( frame_time >> 8 ), s_StringBuffer ), 0x80 );

		last_frame = fps;

		pgPrint( 16, 256, 0xffff, "Press START to EXIT", 0x40 );

		pgScreenFlipV();

		if ( CTRL_GetKey() & KEY_START )
		{
			break;
		}
	}

	return 0;
}

//*************************************************************************************
//	
//*************************************************************************************
void pgExit( int n )
{
	__exit();
}

//*************************************************************************************
//	
//*************************************************************************************
void	pgMain( unsigned long args, void * argp )
{
	int ret = xmain();
	pgExit( ret );
}

//*******************************  END OF FILE  ************************************
