/***********************************************************************************

  Module :	Controller.c

  Description :	

  Last Modified $Date: $

  $Revision: $

  Copyright (C) 29 May 2005 T Swann

***********************************************************************************/

//**********************************************************************************
//   Include Files
//**********************************************************************************
#include "Controller.h"
#include "Graphics.h"

//**********************************************************************************
//   Local Macros
//**********************************************************************************

//**********************************************************************************
//   Local Constants
//**********************************************************************************
typedef struct
{
	u32	frame;
	u32	buttons;
	u8	analog[ 4 ];
	u32	unused;
} ctrl_data_t;

#define CTRL_ANALOG_X	0
#define CTRL_ANALOG_Y	1

//**********************************************************************************
//   Static Prototypes
//**********************************************************************************
void	sceCtrlInit( int unknown );
void	sceCtrlSetAnalogMode( int on );
void	sceCtrlRead( ctrl_data_t * paddata, int unknown );

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
void	CTRL_Init()
{
	sceCtrlInit( 0 );
	sceCtrlSetAnalogMode( 1 );
}

//*************************************************************************************
//	
//*************************************************************************************
u32	CTRL_GetKey()
{
	ctrl_data_t	ctl;

	sceCtrlRead( &ctl, 1 );

	return ctl.buttons;
}

//*************************************************************************************
//	
//*************************************************************************************
u8	CTRL_GetAnalogAxis( u32 axis )
{
	ctrl_data_t	ctl;

	sceCtrlRead( &ctl, 1 );

	return ctl.analog[ axis ];
}

//*******************************  END OF FILE  ************************************
