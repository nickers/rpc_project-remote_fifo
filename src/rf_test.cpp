#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <string.h>
#include "remote_fifo.h"
using namespace std;


/*
 * functions definitions
 */
void create_callback(int code, char* name, void* data);
void open_callback(int rf, char* name, void* data);
void write_callback(int rf, int code, void* buf, int len, void* data);
void read_callback_1(int rf, int code, void* buf, int len, void* data);
void read_callback_2(int rf, int code, void* buf, int len, void* data);
void close_callback(int code, char* name, void* data);
void unlink_callback(int code, char* name, void* data);
// -- end of definitions --

void create_callback(int code, char* name, void* data) {
	printf("Remote FIFO created: `%s`, code: %d\n", name, code);
	open_rf(name, open_callback, data);
}

void open_callback(int rf, char* name, void* data) {
	char test[] = "probna wiadomosc";
	int len = strlen(test);
	char* buf = new char[len+sizeof(int)];
	printf("FIFO `%s` opened: %d\n", name, rf);
	printf(" -> writing into %d : [%d][%s]\n", rf, len, test);

	memcpy(buf, &len, sizeof(len));
	memcpy(&(buf[sizeof(len)]), test, len);

	write_rf(rf, buf, sizeof(int)+len, write_callback, data);
}

void write_callback(int rf, int code, void* buf, int len, void* data) {
	delete[] (char*)buf;
	int *length = new int[1];
	printf("Message sent, code: %d. Now will read.\n", code);
	read_rf(rf, length, sizeof(int), read_callback_1, data);
}

void read_callback_1(int rf, int code, void* buf, int len, void* data) {
	int* msg_length = (int*)buf;
	printf("Readed %d bytes, should be %ld.\n", len, sizeof(int));
	printf("  - message to read length: %d\n", *msg_length);
	printf("Reading message...\n");
	*msg_length = 10; // TODO remove this!
	char* read_buf = new char[*msg_length];
	read_rf(rf, read_buf, *msg_length, read_callback_2, data);
	delete[] (int*)buf;
}

void read_callback_2(int rf, int code, void* buf, int len, void* data) {
	char* msg = (char*)buf;
	printf("Readed message of length: %d\n", len);
	printf("Message: %s\n", msg);
	delete[] (char*)buf;
	close_rf(rf, close_callback, data);
}

void close_callback(int code, char* name, void* data) {
	printf("Remote FIFO `%s` closed, code: %d\n", name, code);
	unlink_rf(name, unlink_callback, data);
}

void unlink_callback(int code, char* name, void* data) {
	printf("Remote FIFO `%s` unlinked, code: %d\n", name, code);
	printf("BTW. you passed this string: %s\n", (char*)data);
}


int main(int argc, const char* argv[]) {
  char pass_string[] = "przekaz dalej";
  char fifo_name[] = "/test.fifo";
  char host[] = "localhost";

  printf("Remote FIFO - test program\n");

  init_rf(host);
  create_rf(fifo_name, create_callback, pass_string);

  return 0;
}
