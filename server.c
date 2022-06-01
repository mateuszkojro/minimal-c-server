
// Libc headers
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Posix specific headers
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <sys/types.h>

// Some utilities for presentation
#include "utils.h"

#define PORT      8082
#define BUFF_SIZE 1024 * 1024 // 1 MB

clock_t last = {0};
int     socket_file_descriptor, connection_file_descriptor;
bool    should_close = false;

void
http_server(int connection_fd)
{
	char buff[BUFF_SIZE]       = {0};
	buff[BUFF_SIZE - 1]        = '\0';
	char template[BUFF_SIZE]   = {0};
	char   endpoint[BUFF_SIZE] = {0};
	char  *html                = NULL;
	size_t size;

	read(connection_fd, buff, sizeof buff);

	// Our version of a router
	sscanf(buff, "GET %s\n", endpoint);
	if (strcmp(endpoint, "/quit") == 0) {
		printf("Exiting\n");
		should_close = true;
	} else if (strcmp(endpoint, "/hello") == 0) {
		html = read_file("./resources/hello.html", &size);
		bzero(buff, BUFF_SIZE);
		CHECK(html != NULL,
		      "Reading template from: ./resources/hello.html");
		sprintf(template, html, endpoint, buff);
	} else if (strcmp(endpoint, "/google") == 0) {
		sprintf(template, html, endpoint, buff);
		const char *response = "HTTP/1.1 302 OK\r\n"
				       "Server: C_HTTP_server\r\n"
				       "Location: http://google.com\r\n"
				       "Connection: Closed\r\n\r\n%s";

		sprintf(buff, response, template);
		goto send;
	} else {
		html = read_file("./resources/index.html", &size);
		CHECK(html != NULL,
		      "Reading template from: ./resources/index.html");
		sprintf(template, html, endpoint, buff);
	}
	free(html);

	const char *response = "HTTP/1.1 200 OK\r\n"
			       "Server: C_HTTP_server\r\n"
			       "Content-Type: text/html\r\n"
			       "Connection: Closed\r\n\r\n%s";

	sprintf(buff, response, template);

send:
	write(connection_fd, buff, sizeof buff);
	close(connection_fd);
}

void
macro_test()
{
	CHECK(true, "Something succede");
	CHECK(false, "Something failed");
}

int
main()
{
	// macro_test();


	// assign server IP and PORT
	struct sockaddr_in server_addres = {0};
	server_addres.sin_family         = AF_INET;

	// Converts 64 bit number to network byte order - big endian
	server_addres.sin_addr.s_addr    = htonl(INADDR_ANY);
	server_addres.sin_port           = htons(PORT);

	// Assign specified network addres to a socket
	CHECK((bind(socket_file_descriptor, (struct sockaddr *)&server_addres,
		    sizeof(server_addres))) == 0,
	      "Socket bind");

	// Set socket options - in this case allow system to reuse socket after
	// close reuse is automaticaly disabled to prevent confusion
	int reuse_port = 1;
	CHECK(setsockopt(socket_file_descriptor, SOL_SOCKET, SO_REUSEPORT,
			 &reuse_port, sizeof(reuse_port)) == 0,
	      "Socket opt set");

	// We announce that we are willing ro accept connections
	int backlog_size = 5;
	CHECK((listen(socket_file_descriptor, backlog_size)) == 0,
	      "Listening on socket");

	// Should server close
	while (!should_close) {
		// Accept the data packet from client
		struct sockaddr_in client_addres = {0};
		socklen_t          content_size  = sizeof(client_addres);
		connection_file_descriptor =
		    accept(socket_file_descriptor,
			   (struct sockaddr *)&client_addres, &content_size);

		CHECK(connection_file_descriptor > 0, "Accepted connection");

		// Our sever implementation
		http_server(connection_file_descriptor);
	}
	// Close the socket when shuting down the server
	close(socket_file_descriptor);
}
