#include "csapp.h"

// This function reads the request headers from the client.
// These headers aren't used, so just ignore them.
void read_headers(rio_t *rio)
{
  char buf[MAXLINE];
  size_t linesRead;

  printf("Request Headers:\n");

  linesRead = Rio_readlineb(rio, buf, MAXLINE);
  printf("%s", buf);

  // Read the request headers while there is not an empty line.
  while(strcmp(buf, "\r\n")) {
  	 linesRead = Rio_readlineb(rio, buf, MAXLINE);
	 printf("%s", buf);
  }

  return;
}

// Handle a request from a client.
void handleRequest(int connfd)
{
  rio_t rio;
  char buf[MAXLINE], errMessage[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE], filename[MAXLINE];
  ssize_t bytesread, byteswritten;
  int srcfd;

  // Initialize the rio buffer
  Rio_readinitb(&rio, connfd);

  // Read the request line from the client
  bytesread = Rio_readlineb(&rio, buf, MAXLINE);
  printf("Read request line:\n%s\n", buf);

  // Parse the request line into separate variables
  sscanf(buf, "%s %s %s", method, uri, version);

  // Read all of the headers
  read_headers(&rio);

  // Build the relative file string from the current path
  strcpy(filename, ".");
  strcat(filename, uri);

  // Open the file
  srcfd = open(filename, O_RDONLY, 0);

  // If we can't open the file, tell the user there is an error.
  if (srcfd < 0)
  {
	  sprintf(errMessage, "Unable to find file %s\n", filename);
    rio_writen(connfd, errMessage, sizeof(errMessage));
    return;
  }

  // While there are bytes still in the file, write the line to the client.
  while((bytesread = rio_readn(srcfd, buf, sizeof(buf))) != 0)
  {
    byteswritten = rio_writen(connfd, buf, bytesread); //write to client
    if (byteswritten < 0) // error handling
    {
      fprintf(stderr, "Error writing to client!");
      Close(srcfd);
		  exit(0);
    }
  }

  if (bytesread < 0) // error handling
  {
    fprintf(stderr, "Error reading from client!");
    Close(srcfd);
	  exit(0);
  }

  Close(srcfd);
}

void runServer(int port)
{
  printf("Running server on port %d.\n", port);

  int listenfd, connfd;
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;
  char client_hostname[MAXLINE], client_port[MAXLINE];

  // Register port for listening for connections
  listenfd = Open_listenfd(port);

  // Continue to accept client connections. block until connection is made
  while(1)
  {
    clientlen = sizeof(struct sockaddr_storage);
    connfd = Accept(listenfd, (SA*)&clientaddr, &clientlen); // block until connection
    getnameinfo((SA*) &clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);
    printf("Connected to (%s, %s)\n", client_hostname, client_port); // print info of connection
    handleRequest(connfd); // handle the file request
    Close(connfd); // close the connection, wait for a new one
  }

  Close(listenfd); // close the listening connector
}

int main(int argc, char ** argv)
{
  int port;

  if (argc != 2)
  {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(0);
  }

  port = atoi(argv[1]);
  runServer(port);

  exit(0);
}
