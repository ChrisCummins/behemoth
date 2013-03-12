#include <netdb.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SERVER_PORT 12345
#define BUF_SIZE 4096
#define QUEUE_SIZE 10

__attribute__((noreturn))
static void fatal(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	vfprintf(stderr, format, ap);
	va_end(ap);

	abort();
}

int main(int argc, char **argv)
{
	int s, b, l, on =1;
	char buf[BUF_SIZE];
	struct sockaddr_in channel;

	memset(&channel, 0, sizeof(channel));
	channel.sin_family = AF_INET;
	channel.sin_addr.s_addr = htonl(INADDR_ANY);
	channel.sin_port = htons(SERVER_PORT);

	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s < 0)
		fatal("socket failed\n");
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));

	b = bind(s, (struct sockaddr *)&channel, sizeof(channel));
	if (b < 0)
		fatal("bind failed\n");

	l = listen(s, QUEUE_SIZE);
	if (l < 0)
		fatal("listen failed\n");

	while(1) {
		int sa, fd, bytes;

		/* Block for connection request. */
		sa = accept(s, 0, 0);
		if (sa < 0)
			fatal("accept failed\n");

		/* Read file name from socket. */
		read(sa, buf, BUF_SIZE);

		/* Get and return the file. */
		fd = open(buf, O_RDONLY);
		if (fd < 0)
			fatal("open failed\n");

		do {
			/* Read from file. */
			bytes = read(fd, buf, BUF_SIZE);

			/* Write bytes to socket. */
			write(sa, buf, bytes);
		} while (bytes); /* Until end of file. */

		close(fd);
		close(sa);
	}

	return 0;
}
