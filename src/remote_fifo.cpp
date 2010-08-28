#include "remote_fifo.h"
#include "rpc/remote_fifo_client_instance.h"
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

char* __remote_fifo_host = NULL;
rf_client_instance __client_instance;


// internal
CLIENT* __aquire_client(char* host)
{
    CLIENT *c = NULL;

	if (host==NULL)
	{
		host = __remote_fifo_host;
	}
	else if (__remote_fifo_host==NULL)
	{
		__remote_fifo_host = host;
	}

    #ifndef	DEBUG
	c = clnt_create (host, REMOTE_FIFO, SERVER_API, "udp");
	if (c == NULL)
	{
		clnt_pcreateerror (host);
		exit (1);
	}
    #endif	/* DEBUG */

    return c;
}

// internal
void __release_client(CLIENT* c)
{
    #ifndef	DEBUG
	clnt_destroy (c);
    #endif	 /* DEBUG */
}

// internal -- cleaning
void clean_rf()
{
	stop_rf_client(&__client_instance);
    if (__remote_fifo_host!=NULL)
    {
        free(__remote_fifo_host);
    }
    __remote_fifo_host = NULL;
}

int __call_remote_man_func(char* name, rf_man_callback callback, void* data, char* host, clnt_stat (*func)(management_rf*, int*,CLIENT*))
{
    management_rf  args;
    int result = 0;

    if (host==NULL)
    {
        host = __remote_fifo_host;
    }

    args.name = name;

    args.callback.callback_len = sizeof(callback);
    args.callback.callback_val = (char*)&callback;

    args.data.data_len = sizeof(data);
    args.data.data_val = (char*)data;

    CLIENT *c = __aquire_client(host);
    int ret_val = func(&args, &result, c);
	if (ret_val != RPC_SUCCESS)
	{
		clnt_perror (c, "call failed");
	}
    __release_client(c);

    return result;
}

int __call_remote_rw_func(int handle, void* buffer, unsigned long long size, rf_rw_callback callback, void* data, clnt_stat (*func)(data_rf*, int*,CLIENT*))
{
	data_rf  args;
	int result = 0;

	args.descriptor = handle;

    args.callback.callback_len = sizeof(callback);
    args.callback.callback_val = (char*)&callback;

    args.data.data_len = sizeof(data);
    args.data.data_val = (char*)&data;

	args.buf.buf_len = size;
	args.buf.buf_val = (char*)buffer;

	CLIENT *c = __aquire_client(NULL);
	int ret_val = func(&args, &result, c);
	if (ret_val != RPC_SUCCESS)
	{
		clnt_perror (c, "call failed");
	}
	__release_client(c);

	return result;
}


int init_rf(char* host)
{
    __remote_fifo_host = strdup( (host==NULL) ? "localhost" : host );
    __client_instance = run_rf_client(0/*getpid()*/);
    atexit(clean_rf);
    return 0;
}

int create_rf(char* name, rf_man_callback callback, void* data, char* host)
{
    __call_remote_man_func(name, callback, data, host, create_rf__1);
    //callback(0, name, data);
    return 0;
}

int unlink_rf(char* name, rf_man_callback callback, void* data, char* host)
{
    int r = __call_remote_man_func(name, callback, data, host, unlink_rf__1);
    callback(r, name, data);
    return 0;
}

int  open_rf(char* name, rf_man_callback callback,  void* data, char* host)
{
    int r = __call_remote_man_func(name, callback, data, host, open_rf__1);
    callback(1, name, data);
    return 0;
}

int close_rf(int handle, rf_man_callback callback, void* data)
{
	char name[] = "--name-of-fifo--";
    __call_remote_rw_func(handle, NULL, 0, (rf_rw_callback)callback, data, close_rf__1);
    callback(0, name, data);
    return 0;
}

int  read_rf(int handle, void* buffer, unsigned long long size, rf_rw_callback callback, void* data)
{
// TODO zapakować wskaźnik na "buffer" do tablicy?
    int code = __call_remote_rw_func(handle, buffer, size, (rf_rw_callback)callback, data, read_rf__1);
    callback(handle,  code, buffer, size, data);
    return 0;
}

int write_rf(int handle, void* buffer, unsigned long long size, rf_rw_callback callback, void* data)
{
	int code = __call_remote_rw_func(handle, buffer, size, (rf_rw_callback)callback, data, write_rf__1);
    callback(handle,  code, buffer, size, data);
    return 0;
}
