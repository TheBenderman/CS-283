#include "csapp.h"

// This function creates a connnection with a server at
// host using port. The file held in filename is requested
// from the server. The method then reads from the file from
// the connection, and then displays the file to the screen.
void requestFile(char * host, int port, char * filename)
{
	char buf[MAXBUF];
	int clientfd;
	rio_t rio;

	clientfd = Open_clientfd(host, port);

	Rio_readinitb(&rio, clientfd);

	// Build the request string
	sprintf(buf, "GET /%s HTTP/1.1\r\n", filename);
	sprintf(buf, "%sHost: %s\r\n", buf, host);
	sprintf(buf, "%s\r\n", buf);

	printf("Built Buffer:\n%s\n", buf);

	// Write the request buffer to the server
	ssize_t nwritten = rio_writen(clientfd, buf, strlen(buf));
	if (nwritten < 0) // Error checking
	{
		fprintf(stderr, "Error! Unable to write to server.");
		exit(0);
	}

	// Read from the server back into the buffer, and then print
	// to the screen.
	ssize_t nread;
	while ((nread = rio_readnb(&rio, buf, MAXLINE)) != 0)
	{
		printf("%s", buf);
	}

	if (nread < 0)
	{
		fprintf(stderr, "Error reading from server!");
		exit(0);
	}

	Close(clientfd);
}

int main(int argc, char **argv)
{
  int port;
  char *host, *filename;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s <host> <port> <filename>\n", argv[0]);
    exit(0);
  }

  host = argv[1];
  port = atoi(argv[2]);
  filename = argv[3];

  requestFile(host, port, filename);

  exit(0);
}
