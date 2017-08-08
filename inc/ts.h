#ifndef __TS_H
#define __TS_H

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <linux/input.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

#define LEAVE 0
#define PRESS 1

struct touch
{
	int x;
	int y;
	bool status;
};

int init_touch(void);
bool wait4touch(int fd, struct touch *ptouch);

#endif
