/* typing_prac.c */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <termios.h>

#define PASSWORDSIZE 12

typedef unsigned long long int tck64_t;
typedef unsigned long int tick32_t;

tick32_t GetTickCount(){
	tick32_t tick = 0ul;
#if defined(WIN32) || defined(WIN64)
	tick = GeTickCount();
#else
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	tick = (ts.tv_sec*1000ul) + (ts.tv_nsec/1000ul/1000ul);
#endif
	return tick;
}

int main(void){
	int fd, start;
	int nread, cnt = 0, cnt1 = 0, errcnt =0;
	double typing_msec, typing_sec, typing_per_sec;

	char ch, text[] = {"The magic thing is that you can change it."}, ch1, text1[] = {"Hi, This is a 20212981 Typing System"};

	struct termios init_attr, new_attr;

	fd = open(ttyname(fileno(stdin)), O_RDWR);
	tcgetattr(fd, &init_attr);

	new_attr = init_attr;
	new_attr.c_lflag &= ~ICANON;
	new_attr.c_lflag &= ~ECHO;
	new_attr.c_cc[VMIN] = 1;
	new_attr.c_cc[VTIME] = 0;

	if(tcsetattr(fd, TCSANOW, &new_attr) != 0){
		fprintf(stderr, "Unable to set terminal properties.\n");
	}

	printf("Enter the following sentence as it is.\n%s\n", text);
	start = GetTickCount();
	
	while((nread = read(fd, &ch, 1)) > 0 && ch != '\n'){
		if(ch == text[cnt++])
			write(fd, &ch, 1);
		else{
			write(fd, "*", 1);
			errcnt++;
		}
	}
	printf("\n Enter the following sentence as it is.\n%s\n", text1);

	while((nread = read(fd, &ch1, 1)) > 0 && ch != '\n'){
		if(ch == text1[cnt1++])
			write(fd, &ch1, 1);
		else{
			write(fd, "*", 1);
			errcnt++;
		}
	}

	typing_msec = GetTickCount() - start;

	typing_sec = fmod((typing_msec/1000), 60);
	typing_per_sec = (sizeof(text) / sizeof(int)) / typing_sec;

	printf("\n Time taken : %3.lfsec\n", typing_sec);
	printf(" I hit%d per minute.\n\n", (int)(typing_per_sec*60));
	printf(" Number of typing errors : %d\n", errcnt);
	tcsetattr(fd, TCSANOW, &init_attr);
	close(fd);
}
