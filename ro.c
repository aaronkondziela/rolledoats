/*
The MIT License (MIT)

Copyright (c) 2016 Aaron Kondziela <aaron@aaronkondziela.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <errno.h>

void usage()
{
	fprintf(stderr, "Rolled Oats - raw serial to stdout bridge\n");
	fprintf(stderr, "Usage: ro <-d serial_device> <-b baud_rate> [-h]\n");
	fprintf(stderr, "See <man 3 cfsetispeed> for list of supported baudrates\n");
}

int main(int argc, char** argv) {
	int f;
	ssize_t len;
	unsigned char buf[8192];
	struct termios t;
	int c;
	char* devname = NULL;
	int baudrate = 0;

	while ((c = getopt(argc, argv, "d:b:h")) != -1) {
		switch (c) {
			case 'd':
				devname = optarg;
				break;
			case 'b':
				sscanf(optarg, "%u", &baudrate);
				break;
			case 'h':
				usage();
				return 0;
			default:
				usage();
				return 1;
		}
	}

	if ( devname == NULL | baudrate == 0 ) {
		usage();
		return 1;
	}

	switch (baudrate) {
		case 50:
		case 75:
		case 110:
		case 134:
		case 150:
		case 200:
			fprintf(stderr, "Note: FTDI FT232R USB to serial chips only go down to 300 baud; continuing...\n");
			break;
		case 300:
		case 600:
		case 1200:
		case 1800:
		case 2400:
		case 4800:
		case 9600:
		case 19200:
		case 38400:
		case 57600:
		case 115200:
			break;
		default:
			fprintf(stderr, "Note: Your baudrate %u looks unusual, did you mistype? Continuing...\n", baudrate);
	}
	f = open(devname, O_RDONLY | O_NOCTTY | O_NONBLOCK);
	cfmakeraw(&t);
	cfsetispeed(&t, (speed_t)baudrate);
	cfsetospeed(&t, (speed_t)baudrate);

	if (tcsetattr(f, TCSANOW, &t) == -1) {
		switch (errno) {
			case EBADF:
				fprintf(stderr, "tcsetattr error: bad file descriptor. Check device name\n");
				return 2;
			case ENOTTY:
				fprintf(stderr, "tcsetattr error: device is not a terminal\n");
				return 3;
			default:
				fprintf(stderr, "tcsetattr error %u\n", errno);
				return 4;
		}
	}

	while (1) {
		len = read(f, buf, 8192);
		if (len>0) write(1, buf, len);
		usleep(1000);
	}
	return 0;
}
