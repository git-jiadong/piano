#include "ts.h"
#include "jpeg.h"
#include <pthread.h>
#include <semaphore.h>

sem_t play;
void *madplay(void *arg);

int main(void)
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

	int ts = init_touch();
	struct touch buf;

	sem_init(&play, 0, 0);
	pthread_t tid;
	pthread_create(&tid, NULL, madplay, (void *)&buf);
	
	int last_x = 10, now_x;
	int flag = 1;
	while(1)
	{
		wait4touch(ts, &buf);
	//	printf("x: %d\n", buf.x);
	//	printf("y: %d\n", buf.y);
	//	printf("s: %s\n", buf.status == 0? "LEAVE": "PRESS");
		if(buf.x>10 && buf.x<790 && buf.y>50 && buf.y<280)
		{
			now_x = (buf.x-20)/65*65+10;
			if(buf.status == LEAVE || last_x != now_x)
			{
				write_lcd(lcd, &vinfo, leave, &iminfo, last_x, 50);
				flag = 1;
			}

			if(buf.status == PRESS && flag == 1)
			{

				last_x = (buf.x-20)/65*65+10; 
				write_lcd(lcd, &vinfo, press, &iminfo, last_x, 50);
				sem_post(&play);
				flag = 0;
			}

		}

		if(buf.x>690 && buf.x<790 && buf.y>5 && buf.y<40)
		{
			if(buf.status == PRESS)
			{
				write_lcd(lcd, &vinfo, out_on, &out_info, 690, 5);
				flag = 0;
				printf("will out\n");
			}
			if(buf.status == LEAVE && flag == 0)
			{
				break;
			}
		}

	}
	sem_destroy(&play);
	memcpy(lcd, out_ground, lcd_size);

	return 0;
}

void *madplay(void *arg)
{
	struct touch *pbuf = (struct touch *)arg;
	while(1)
	{
		sem_wait(&play);
		int num = (pbuf->x - 20)/65;
	
		char cmd[45];
		snprintf(cmd, 45, "./audio/bin/madplay mp3/d%d.mp3 -a -15 &", num+1);
		system("killall madplay");
		system(cmd);
	}
}
