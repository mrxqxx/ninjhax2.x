#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctr/types.h>
#include <ctr/srv.h>
#include <ctr/svc.h>

#include "../../build/constants.h"

#include "svc.h"

int _main(void)
{
	Handle portServer, portClient;
	Result ret;

	ret = svc_createPort(&portServer, &portClient, "hb:SPECIAL", 1);
	if(ret)*(u32*)0xDEADBABE = ret;

	int currentHandleIndex, numSessionHandles;

	Handle sessionHandles[8];

	currentHandleIndex = 0;
	sessionHandles[0] = portServer;
	numSessionHandles = 1;

	ret = svc_replyAndReceive((s32*)&currentHandleIndex, sessionHandles, numSessionHandles, 0);

	while(1)
	{
		if(ret==0xc920181a)
		{
			// //close session handle
			// svc_closeHandle(sessionHandles[currentHandleIndex]);
			// sessionHandles[currentHandleIndex]=sessionHandles[numSessionHandles];
			// sessionHandles[numSessionHandles]=0x0;
			// currentHandleIndex=(numSessionHandles)--; //we want to have replyTarget=0x0
		}else if(!ret){
			switch(currentHandleIndex)
			{
				case 0:
					{
						// receiving new session
						svc_acceptSession(&sessionHandles[numSessionHandles], sessionHandles[currentHandleIndex]);
						currentHandleIndex=numSessionHandles++;
					}
					break;
				default:
					{
						//receiving command from ongoing session
						u32* cmdbuf=getThreadCommandBuffer();
						// u8 cmdIndex=cmdbuf[0]>>16;
						// if(cmdIndex<=NUM_CMD && cmdIndex>0)
						// {
						// 	commandHandlers[cmdIndex-1](cmdbuf);
						// }
						// else
						// {
						// 	cmdbuf[0] = (cmdbuf[0] & 0x00FF0000) | 0x40;
						// 	cmdbuf[1] = 0xFFFFFFFF;
						// }
					}
					break;
			}
		}

		ret = svc_replyAndReceive((s32*)&currentHandleIndex, sessionHandles, numSessionHandles, sessionHandles[currentHandleIndex]);
	}

	while(1)svc_sleepThread(0x0FFFFFFFFFFFFFFFLL);

	return 0;
}