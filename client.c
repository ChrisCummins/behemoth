#include <libgen.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SERVER_PORT 12345
#define BUF_SIZE 4096

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
	int c, s, bytes;
	char buf[BUF_SIZE];
	struct hostent *h;
	struct sockaddr_in channel;

	if (argc != 3) {
		fprintf(stderr,
			"Usage: %s <server> <path>\n", basename(argv[0]));
		exit(1);
	}

	/* Lookup host's IP address. */
	h = gethostbyname(argv[1]);
	if (!h)
		fatal("failed to resolve hostname '%s'\n", argv[1]);

	s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s < 0)
		fatal("socket failed\n");

	memset(&channel, 0, sizeof(channel));
	channel.sin_family = AF_INET;
	memcpy(&channel.sin_addr.s_addr, h->h_addr, h->h_length);
	channel.sin_port = htons(SERVER_PORT);

	c = connect(s, (struct sockaddr *)&channel, sizeof(channel));
	if (c < 0)
		fatal("failed to connect to server\n");

	/* Connection is established. Send filename including trailing null '\0'
	 * character. */
	write(s, argv[2], strlen(argv[2]) + 1);

	/* Write file to stdout. */
	do {
		bytes = read(s, buf, BUF_SIZE);
		write(1, buf, bytes);
	} while (bytes <= 0);

	return 0;
}
