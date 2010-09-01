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
	int len = strlen(test)+1;
	char* buf = new char[len+sizeof(int)];
	printf("FIFO `%s` opened: #%d\n", name, rf);
	printf(" -> writing into #%d : [%d][%s]\n", rf, len, test);

	memcpy(buf, &len, sizeof(len));
	memcpy(&(buf[sizeof(len)]), test, len);

	write_rf(rf, buf, sizeof(int)+len, write_callback, data);
	delete[] buf;
}

void write_callback(int rf, int code, void* buf, int len, void* data) {
	int *length = new int[1];
	printf("Message sent to #%d, code: %d. Now will read.\n", rf, code);
	read_rf(rf, length, sizeof(int), read_callback_1, data);
}

void read_callback_1(int rf, int code, void* buf, int len, void* data) {
	int msg_length = 0;
	memcpy(&msg_length, buf, sizeof(msg_length));
	printf("Readed %d bytes from #%d, should be %ld.\n", len, rf, sizeof(int));
	printf("  - #%d message's to read length: %d\n", rf, msg_length);
	printf("Reading message from #%d...\n", rf);
	char* read_buf = new char[msg_length];
	read_rf(rf, read_buf, msg_length, read_callback_2, data);
	delete[] (char*)buf;
}

void read_callback_2(int rf, int code, void* buf, int len, void* data) {
	char* msg = (char*)buf;
	printf("Readed message #%d of length: %d\n", len, rf);
	printf("Message #%d: [%*s]\n", rf, len-1, msg); // 'len-1' - without '\0'
	delete[] (char*)buf;
	close_rf(rf, close_callback, data);
}

void close_callback(int code, char* name, void* data) {
	printf("Remote FIFO `%s` closed, code: %d\n", name, code);
	unlink_rf(name, unlink_callback, data);
}

void unlink_callback(int code, char* name, void* data) {
	printf("Remote FIFO `%s` unlinked, code: %d -  ", name, code);
	printf("BTW. you passed this string: \"%s\"\n", (char*)data);
	sem_post(&finished_test);
}


int main(int argc, const char* argv[]) {
    char pass_string[100] = "przekaz dalej";
    char fifo_name[200];
    char fifo_name2[200];  
    char host[20] = "127.0.0.1";
    
    // create unique names    
    sprintf(fifo_name, "/test.fifo-%d", getpid());
    sprintf(fifo_name2, "/test2.fifo-%d", getpid());
    
    printf("@ Remote FIFO - test program started @\n");
    sem_init(&finished_test, 0, 0);
    init_rf(host);
    create_rf(fifo_name, create_callback, pass_string);
    create_rf(fifo_name2, create_callback, pass_string);
    fflush(NULL);
    sem_wait(&finished_test);
    sem_wait(&finished_test);
    
    return 0;
}
