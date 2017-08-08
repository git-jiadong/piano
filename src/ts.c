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

	read(fd, &buf, sizeof(buf));
	if(buf.type == EV_ABS)
	{
		if(buf.code == ABS_X)
			ptouch->x = buf.value;
		if(buf.code == ABS_Y)
			ptouch->y = buf.value;
	}

	if(buf.type == EV_KEY)
	{
		if(buf.code == BTN_TOUCH)
			ptouch->status = buf.value;
	}
}
