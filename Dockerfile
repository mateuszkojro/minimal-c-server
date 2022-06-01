FROM ubuntu:latest
WORKDIR /server
RUN apt update && apt upgrade -y
RUN apt install -y gcc make
COPY Makefile .
COPY resources/ resources/
COPY server.c .
COPY utils.h .
RUN make server
CMD ["make" , "run"] 