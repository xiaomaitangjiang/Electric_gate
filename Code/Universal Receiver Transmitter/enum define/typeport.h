#ifndef _typeport_
#define _typeport_

#include "stm32f1xx_hal.h"
enum action
		{
			openswitch=1,
			
			opensafefuntion=2,
			
			openexpandfuntion=3,
			
			closeswitch=4,
			closesafefuntion=5,
			closeexpandfuntion=6,
			closesafemode=7,
			byteerror=0,
			null=12
		};

		
	
#endif
