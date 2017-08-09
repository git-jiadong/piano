#include "ts.h"

int init_touch(void)
{
	int fd = open("/dev/input/event0", O_RDONLY);
	if(fd == -1)
	{
		printf("open the touch failed: %s", strerror(errno));
		exit(0);
	}

	return fd;
}

bool wait4touch(int fd, struct touch *ptouch)
{
	struct input_event buf;	
	bzero(&buf, sizeof(buf));
	int flag = 0;

	ptouch->status = 1;
	while(flag != 3)
	{
//		printf("xxx\n");
		read(fd, &buf, sizeof(buf));
		if(buf.type == EV_ABS)
		{
			if(buf.code == ABS_X)
			{
				ptouch->x = buf.value;
				flag |= 1;
//				printf("flag: %d\n", flag);
			}
			if(buf.code == ABS_Y)
			{
				ptouch->y = buf.value;
				flag |= 2;
//				printf("flag: %d\n", flag);
			}
		}

		if(buf.type == EV_KEY)
		{
			if(buf.code == BTN_TOUCH && buf.value == 0)
			{
				ptouch->status = buf.value;
				break;
//				printf("flag: %d\n", flag);
			}
		}

	}


}
