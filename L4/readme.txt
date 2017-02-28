In this assignment, we were asked to create two programs, one for a client
test web server and one for a server test web server. The client program
takes three arguments, the hostname, port, and file. The client program
establishing a connection with a server using the given hostname and port.
The program then builds a GET request string using the provided filename,
sends the request to the server, reads the response from the server, and
write the response to the screen. The server program takes a single
argument, the port number. The server opens a listening socket using the
given port number, and waits for connections. When a connection is made, the
server reads the request from the client, parses the request, opens the
requested file, and writes the file to the client.


My server does not server dynamic content or create any extra request headers
like Tiny does. All it does is open the file that is requested by the client
and write the file back to the client in it's entirety. If the file is not found,
the server writes an error message back to the client.

I did as much error checking as possible, for opening connections and reading/writing.
Since I am using Open_clientfd and Open_listenfd, these functions take care of
their own error handling, so i am not checking for errors there.


Run a server using the following command:
  make server PORT=<PORT>
	e.g: make server PORT=1737

Run a client using the following command:
  make client HOST=<host> PORT=<port> FILE=<filename>
	e.g: make client HOST=tux1.cs.drexel.edu PORT=1737 FILE=home.html


I have supplied a sample html file for use with the server. This file is called
home.html.

To test my program using this file, first set up a server:

  make server PORT=1467

Then setup a client:

  make client HOST=<host name from server> PORT=1467 FILE=home.html


This should then display the home.html file to the stdout of the client. The server
should also log the request and headers that it receives from the client.
