#include "ts.h"
#include "jpeg.h"
#include <pthread.h>
#include <semaphore.h>


void *show_lcd(void *arg);
void *madplay(void *arg);
void *show_key(void *arg);

int main(void)
{
	int ts = init_touch();
	struct touch buf;
	
	pthread_t tid1, tid2, tid3;
	pthread_create(&tid1, NULL, show_lcd, (void *)&buf);
	pthread_create(&tid2, NULL, madplay, (void *)&buf);
	

	while(1)
	{
		wait4touch(ts, &buf);
		printf("x: %d\n", buf.x);
		printf("y: %d\n", buf.y);
		printf("s: %s\n", buf.status == 0? "LEAVE": "PRESS");
	}

	return 0;
}

void *show_lcd(void *arg)
{
	struct fb_var_screeninfo vinfo;
	struct image iminfo, out_info;
	unsigned char *lcd = init_lcd(&vinfo);
	
	int lcd_size = vinfo.xres*vinfo.yres*vinfo.bits_per_pixel/8;
	unsigned char *out_ground = calloc(1, lcd_size);
	memcpy(out_ground, lcd, lcd_size);

	unsigned char *background = load_jpeg("jpeg/jpeg/background1.jpg", &iminfo);
	write_lcd(lcd, &vinfo, background, &iminfo, 0, 0);
	free(background);

	unsigned char *out_off = load_jpeg("jpeg/jpeg/out_off.jpg", &out_info);
	unsigned char *out_on = load_jpeg("jpeg/jpeg/out_on.jpg", &out_info);
	write_lcd(lcd, &vinfo, out_off, &out_info, 690, 5);
	free(out_off);

	unsigned char *press = load_jpeg("jpeg/jpeg/key_on.jpg", &iminfo);
	unsigned char *leave = load_jpeg("jpeg/jpeg/key_off.jpg", &iminfo);
				
	struct touch *pbuf = (struct touch *)arg;

	int last_x = 10, now_x;
	while(1)
	{
		if(pbuf->x>10 && pbuf->x<790 && pbuf->y>50 && pbuf->y<280)
		{
			if(pbuf->status == PRESS)
			{
				last_x = (pbuf->x-20)/65*65+10; 
				write_lcd(lcd, &vinfo, press, &iminfo, last_x, 50);
			}

			now_x = (pbuf->x-20)/65*65+10;
			if(pbuf->status == LEAVE || last_x != now_x)
			{
				write_lcd(lcd, &vinfo, leave, &iminfo, last_x, 50);
			}
		}
		if(pbuf->x>690 && pbuf->x<790 && pbuf->y>5 && pbuf->y<40)
		{
			int flag = 1;
			if(pbuf->status == PRESS)
			{
				write_lcd(lcd, &vinfo, out_on, &out_info, 690, 5);
				flag = 0;
				printf("will out\n");
			}
			if(pbuf->status == LEAVE && flag == 0)
			{
				break;
			}
		}
	}
	memcpy(lcd, out_ground, lcd_size);
	exit(0);
}

void *madplay(void *arg)
{
	struct touch *pbuf = (struct touch *)arg;
	char *mp3[] = {"./audio/bin/madplay mp3/d1.mp3 &", "./audio/bin/madplay mp3/d2.mp3 &", 
			"./audio/bin/madplay mp3/d3.mp3 &", "./audio/bin/madplay mp3/d4.mp3 &", 
			"./audio/bin/madplay mp3/d5.mp3 &", "./audio/bin/madplay mp3/d6.mp3 &", 
			"./audio/bin/madplay mp3/d7.mp3 &", "./audio/bin/madplay mp3/d8.mp3 &", 
			"./audio/bin/madplay mp3/d9.mp3 &", "./audio/bin/madplay mp3/d10.mp3 &", 
			"./audio/bin/madplay mp3/d11.mp3 &", "./audio/bin/madplay mp3/d12.mp3 &"};
	int flag = 1;
	int last_x, now_x;
	while(1)
	{
		if(pbuf->x>10 && pbuf->x<790 && pbuf->y>50 && pbuf->y<280)
		{
			if(pbuf->status == PRESS && flag == 1)
			{
				usleep(2000);
				last_x = (pbuf->x-20)/65;
				system("killall madplay");
				system(mp3[last_x]);
				flag = 0;
			}
			now_x = (pbuf->x-20)/65;
			if(pbuf->status == LEAVE || now_x != last_x)
			{
				flag = 1;
			}
		}
	}
}

/*
void *play(void *n)
{
	system("killall madplay");

	char cmd[20];
	snprintf(cmd, 20, "./audio/bin/madplay mp3/d%d.mp3", n);)
	system(cmd);

	pthread_exit();

}
*/
