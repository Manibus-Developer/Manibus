#include "musmart_cho.h"
#include "musmart.h"

int main(void)
{	
	 localUsartConfig();
   Bytes_BUS_RecInit();
	
 while(1)
  {	
		Msg_Filtrate();
	}
	
}





