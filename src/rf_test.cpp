#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
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


sem_t finished_test;


void create_callback(int code, char* name, void* data) {
	printf("Remote FIFO created: `%s`, code: %d, data: %s\n", name, code, (char*)data);
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
	int msg_length = 0;
	memcpy(&msg_length, buf, sizeof(msg_length));
	printf("Readed %d bytes, should be %ld.\n", len, sizeof(int));
	printf("  - message to read length: %d\n", msg_length);
	printf("Reading message...\n");
	//msg_length = 10; // TODO remove this!
	char* read_buf = new char[msg_length];
	read_rf(rf, read_buf, msg_length, read_callback_2, data);
	delete[] (char*)buf;
}

void read_callback_2(int rf, int code, void* buf, int len, void* data) {
	char* msg = (char*)buf;
	printf("Readed message of length: %d\n", len);
	printf("Message: %*s\n", len, msg);
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
	sem_post(&finished_test);
}


int main(int argc, const char* argv[]) {
  char *pass_string = strdup("przekaz dalej");
  char *fifo_name = strdup("/test.fifo");
  char *host = strdup("127.0.0.1");

  printf("Remote FIFO - test program\n");
  sem_init(&finished_test, 0, 0);
  init_rf(host);
  create_rf(fifo_name, create_callback, pass_string);
  fflush(NULL);
  sem_wait(&finished_test);
  return 0;
}
