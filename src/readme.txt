
void open/close_result_func(result_code, name, data=>pass_this_data);
void read/write_result_func(rf_descriptor/handle, result_code, void* buf, int len, data=>pass_this_data);


int create_rf(name=>"/tmp/name", callback=>open/close_result_func, data=>pass_this_data);
    -- server calls:
    open/close_result_func

int unlink_rf(name=>"...", callback=>open/close_result_func, data=>null);
int unlink_rf(name=>"..."); // don't need to care

int open_rf(name=>"...", callback=>open/close_result_func, data=>pass_this_data);
int close_rf(rf_desc=>int, callback=>..., data=>...);
int close_rf(rf_desc=>int); // no return value

int write_rf(rf_desc=>int, buf=>..., size=>..., callback=>write_result_func, data=>pass_this_data);
int read_rf( rf_desc=>int, buf=>..., size=>..., callback=>read_result_func,  data=>pass_this_data);


// --- test ---

char pass_string[] = "przekaz dalej";
create_rf("/test.fifo", create_callback, pass_string);

void create_callback(int code, char* name, void* data) {
	printf("Remote FIFO created: `%s`, code: %d\n", name, code);
	open_rf(name, open_callback, data);
}

void open_callback(int rf, char* name, void* data) {
	char test[] = "probna wiadomosc";
	int len = strlen(test);
	char buf[] = new char[len+sizeof(int)];
	printf("FIFO `%s` opened: %d\n", name, rf);
	printf(" -> writing into %d : [%d][%s]\n", r, len, test);

	memcpy(buf, &len, sizeof(len));
	memcpy(&(buf[sizeof(len)]), test, len);

	write_rf(rf, buf, sizeof(int)+len, write_callback, data);
}

void write_callback(int rf, int code, void* buf, int len, void* data) {
	int *length = new int;
	printf("Message sent, code: %d. Now will read.\n", code);
	read_rf(rf, length, sizeof(int), read_callback_1, data);
}

void read_callback_1(int rf, int code, void* buf, int len, void* data) {
	int* msg_length = (int*)buf;
	printf("Readed %d bytes, should be %d.\n", len, sizeof(int));
	printf("  - message to read length: %d\n", *msg_length);
	printf("Reading message...\n");
	char buf[] = new char[*msg_length];
	read_rf(rf, buf, *msg_length, read_callback_2, data);
	delete msg_length;
}

void read_callback_2(int rf, int code, void* buf, int len, void* data) {
	char* msg = (char*)buf;
	printf("Readed message of length: %d\n", len);
	printf("Message: %s\n", msg);
	close_rf(rf, close_callback, data);
}

void close_callback(int code, char* name, void* data) {
	printf("Remote FIFO `%s` closed, code: %d\n", code);
	unlink_rf(name, unlink_callback, data);

void unlink_callback(int code, char* name, void* data) {
	printf("Remote FIFO `%s` unlinked, code: %d\n", name, code);
	printf("BTW. you passed this string: %s\n", (char*)data);
}


