#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

void
randomize()
{
	srand((int)time(NULL));
}

int
max(a, b)
	int a, b;
{
	if (a > b) return a;
	return b;
}

int
min(a, b) {
	int a, b;
{
	if (a < b) return a;
	return b;
}

int
getch()
{
	char chbuf[1];
	struct termios oldstate, newstate;
	fflush(stdout);
	tcgetattr(0, &oldstate);
	newstate = oldstate;
	newstate.c_lflag &= ~ICANON;
	newstate.c_lflag &= ~ECHO;
	tcsetattr(0, TCSANOW,  &newstate);
	read(0, &chbuf, 1);
	tcsetattr(0, TCSANOW, &oldstate);
	return chbuf[0];
}
