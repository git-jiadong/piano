#include "ts.h"
#include "jpeg.h"
#include <pthread.h>

void *show_lcd(void *arg);
void *madplay(void *arg);

int main(void)
{
	int ts = init_touch();
	struct touch buf;

	pthread_t tid1, tid2;
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
	struct image iminfo;
	unsigned char *lcd = init_lcd(&vinfo);
	unsigned char *background = load_jpeg("jpeg/jpeg/background.jpg", &iminfo);
	write_lcd(lcd, &vinfo, background, &iminfo, 0, 0);
	free(background);

	unsigned char *key = load_jpeg("jpeg/jpeg/key.jpg", &iminfo);
	write_lcd(lcd, &vinfo, key, &iminfo, 10, 50);
	free(key);

	unsigned char *bar = load_jpeg("jpeg/jpeg/bar.jpg", &iminfo);
	write_lcd(lcd, &vinfo, bar, &iminfo, 0, 0);
	free(bar);

	unsigned char *logo = load_jpeg("jpeg/jpeg/logo.jpg", &iminfo);
	write_lcd(lcd, &vinfo, logo, &iminfo, 229, 350);
	free(logo);

	unsigned char *press = load_jpeg("jpeg/jpeg/key_on.jpg", &iminfo);
	unsigned char *leave = load_jpeg("jpeg/jpeg/key_off.jpg", &iminfo);

	struct touch *pbuf = (struct touch *)arg;

	while(1)
	{
		if(pbuf->x>10 && pbuf->x<790 && pbuf->y>50 && pbuf->y<280)
		{
			
			if(pbuf->status == PRESS)
			{
				
				write_lcd(lcd, &vinfo, press, &iminfo, 
						(pbuf->x-20)/65*65+10, 50);
			}
			if(pbuf->status == LEAVE)
			{
				write_lcd(lcd, &vinfo, leave, &iminfo, 
						(pbuf->x-20)/65*65+10, 50);
			}
		}
	}
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
	while(1)
	{

		if(pbuf->x>10 && pbuf->x<790 && pbuf->y>50 && pbuf->y<280)
		{
			if(pbuf->status == PRESS && flag == 1)
			{
				system("killall madplay");
				system(mp3[(pbuf->x-20)/65]);
				flag = 0;
			}
			if(pbuf->status == LEAVE)
			{
				flag = 1;
			}
		}
	}
}
