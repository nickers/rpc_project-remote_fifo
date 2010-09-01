/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "remote_fifo.h"
#include "../remote_fifo.h"
#include <map>
#include <semaphore.h>
#include <string>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../data_queue.h"

#define SAFE_FREE(p); if(p!=NULL) { free(p); p=NULL; }

/**
 * \brief Information about single stream.
 */
class fifo_info {
public:
	/** \brief Stream name.
	 */
	std::string name;
	/** \brief Data written to stream.
	 */
	data_queue	data;
	/** \brief Usage lock.
	 */
	sem_t		lock;

	fifo_info(std::string n)
	{
		name = n;
		sem_init(&lock, 0, 1);
	}

	~fifo_info()
	{
		sem_destroy(&lock);
	}
};

#define fifos_iter std::map<std::string, fifo_info*>::iterator
#define handles_iter std::map<int, fifo_info*>::iterator
std::map<std::string, fifo_info*> fifos;
std::map<int, fifo_info*> handles;

/*
 * Server internal variables.
 */
sem_t fifos_sem;
sem_t handles_sem;
int next_handle = 1;


/**
 * \brief Create rpc-connection back to mini-server created by calling client.
 * \param req RPC request passed to request processing function.
 * \param uid Unique ID of client to which connect.
 * \return RPC client's handle to mini-server.
 */
CLIENT* __aquire_client_callback(struct svc_req* req, int uid)
{
    CLIENT *c = NULL;
    char host[200];

    sockaddr_in* client_addr = svc_getcaller(req->rq_xprt);

	inet_ntop(client_addr->sin_family, &client_addr->sin_addr, host, sizeof(host));

	c = clnt_create (host, REMOTE_FIFO, CLIENT_API+uid, "tcp");
	if (c == NULL)
	{
		clnt_pcreateerror (host);
		exit (1);
	}

    return c;
}

/**
 * \brief Release client's handle.
 * \param c Client's handle.
 */
void __release_client_callback(CLIENT* c)
{
	clnt_destroy (c);
}

//*****************************************************************************
//** Internal utilities
//

typedef clnt_stat (*management_rf_res_func)(management_rf_res*,int*,CLIENT*);
typedef clnt_stat (*data_rf_res_func)(data_rf_res*,int*,CLIENT*);

/**
 * \brief Structure with information needed to create thread calling client
 * back with processing results.
 */
struct client_thread {
	void* res;
	void* func;
	CLIENT* client;
};
typedef client_thread client_thread;

/**
 * \brief Create and fill new 'management_rf_res' structure instance with given data.
 *
 * Returned structure is created by malloc() and must be freed by free() by
 * programmer. All given data if copied, so passed arguments may be released
 * just after returning from functions.
 *
 * \param argp Parameters passed to RPC function.
 * \param code RPC-function's return code.
 * \return Filled 'management_rf_res'.  
 */
management_rf_res* create_management_rf_res(management_rf* argp, int code)
{
	management_rf_res *res = (management_rf_res*)malloc(sizeof(management_rf_res));
	res->code = code;
	res->name = strdup(argp->name);

	res->data.data_len = argp->data.data_len;
	res->data.data_val = (char*)malloc(argp->data.data_len);
	memcpy(res->data.data_val, argp->data.data_val, argp->data.data_len);

	res->callback.callback_len = argp->callback.callback_len;
	res->callback.callback_val = (char*)malloc(argp->callback.callback_len);
	memcpy(res->callback.callback_val, argp->callback.callback_val, argp->callback.callback_len);

	return res;
}

/**
 * \brief Shortcut to free all buffer inside given structure and structure itself.
 *
 * \param res Structure to be deallocated with inside buffers.
 */
void free_rf_res(management_rf_res* res)
{
	SAFE_FREE(res->name);
	SAFE_FREE(res->callback.callback_val);
	SAFE_FREE(res->data.data_val);
	SAFE_FREE(res);
}

/**
 * \brief Create and fill new 'data_rf_res' structure instance with given data.
 *
 * Returned structure is created by malloc() and must be freed by free() by
 * programmer. All given data if copied, so passed arguments may be released
 * just after returning from functions.
 *
 * \param argp Parameters passed to RPC function.
 * \param code RPC-function's return code.
 * \param buf Buffer with data to be passed to client.
 * \param buf_len Length of data in 'buf'.
 * \return Filled 'data_rf_res'.
 */
data_rf_res* create_data_rf_res(data_rf* argp, int code, void* buf, unsigned long long buf_len)
{
	data_rf_res* res = (data_rf_res*)malloc(sizeof(data_rf_res));
	res->descriptor = argp->descriptor;
	res->code = code;

	res->buf.buf_len = buf_len;
	res->buf.buf_val = (char*)malloc(buf_len);
	memcpy(res->buf.buf_val, buf, buf_len);

	res->data.data_len = argp->data.data_len;
	res->data.data_val = (char*)malloc(argp->data.data_len);
	memcpy(res->data.data_val, argp->data.data_val, argp->data.data_len);

	res->callback.callback_len = argp->callback.callback_len;
	res->callback.callback_val = (char*)malloc(argp->callback.callback_len);
	memcpy(res->callback.callback_val, argp->callback.callback_val, argp->callback.callback_len);

	return res;
}

/**
 * \brief Shortcut to free all buffer inside given structure and structure itself.
 *
 * \param res Structure to be deallocated with inside buffers.
 */
void free_rf_res(data_rf_res* res)
{
	SAFE_FREE(res->buf.buf_val);
	SAFE_FREE(res->callback.callback_val);
	SAFE_FREE(res->data.data_val);
	SAFE_FREE(res);
}

/**
 * \brief Main function for thread calling client back with result of one of
 * MANAGEMENT functions.
 */
void* mngm_result_th(void* d)
{
	client_thread* data = (client_thread*)d;
	management_rf_res_func func = (management_rf_res_func)data->func;
	management_rf_res *res = (management_rf_res*)data->res;

	int val = 0;
	func(res, &val, data->client);

	__release_client_callback(data->client);
	free_rf_res(res);
	free(data);
	return NULL;
}

/**
 * \brief Main function for thread calling client back with result of one of
 * DATA functions.
 */
void* data_result_th(void* d)
{
	client_thread* data = (client_thread*)d;
	data_rf_res_func func = (data_rf_res_func)data->func;
	data_rf_res *res = (data_rf_res*)data->res;

	int val = 0;
	func(res, &val, data->client);

	__release_client_callback(data->client);
	free_rf_res(res);
	free(data);
	return NULL;
}

/**
 *
 */
void call_client_mngm_result(int code, struct svc_req *rqstp, management_rf* argp, management_rf_res_func func)
{
    //DEBUG//printf(" * -call mngm: %d : %s : len %d\n", code, argp->name, argp->data.data_len);fflush(NULL);
	CLIENT *c = __aquire_client_callback(rqstp, argp->uid);
	management_rf_res *res = create_management_rf_res(argp, code);

	client_thread* data = (client_thread*)malloc(sizeof(client_thread));
	data->res = res;
	data->func = (void*)func;
	data->client = c;

	pthread_t th;
	pthread_create(&th, NULL, mngm_result_th, data);
	pthread_detach(th);
}

/**
 *
 */
void call_client_data_result(int code, void* buf, int buf_len, struct svc_req *rqstp, data_rf* argp, data_rf_res_func func)
{
	//DEBUG//printf(" * -call data: %d : %d : buf len: %d, data len %d\n", code, argp->descriptor, buf_len, argp->data.data_len);fflush(NULL);
	CLIENT *c = __aquire_client_callback(rqstp, argp->uid);
	data_rf_res* res = create_data_rf_res(argp, code, buf, buf_len);

	client_thread* data = (client_thread*)malloc(sizeof(client_thread));
	data->res = (void*)res;
	data->func = (void*)func;
	data->client = c;

	pthread_t th;
	pthread_create(&th, NULL, data_result_th, data);
	pthread_detach(th);
}

/*******************************************************************************
 * Client side threads running callback functions.
 ******************************************************************************/

struct callback_th_params {
	int handle;
	int code;
	void* data;
	void* buf;
	unsigned long long buf_len;
	char* name;
	void* func;
};
typedef callback_th_params callback_th_params;

callback_th_params* create_callback_th_params(data_rf_res* argp)
{
	callback_th_params* params = (callback_th_params*)malloc(sizeof(callback_th_params));
	memset(params, 0, sizeof(*params));

	params->handle= argp->descriptor;
	params->code = argp->code;
	memcpy(&params->func, argp->callback.callback_val, argp->callback.callback_len);

	if (argp->buf.buf_val!=NULL && argp->buf.buf_len>0)
	{
		unsigned long long size = 0;
		char* buffer = NULL;
		int delta = sizeof(size) + sizeof(buffer);
		memcpy(&size, argp->buf.buf_val, sizeof(size));
		memcpy(&buffer, &argp->buf.buf_val[sizeof(size)], sizeof(buffer));
		memcpy(buffer, &argp->buf.buf_val[delta], size);

		params->buf = buffer;
		params->buf_len = size;
	}

	assert(sizeof(params->data)==argp->data.data_len);
	memcpy(&params->data, argp->data.data_val, sizeof(params->data));
	
	return params;
}

callback_th_params* create_callback_th_params(management_rf_res* argp)
{
	callback_th_params* params = (callback_th_params*)malloc(sizeof(callback_th_params));
	memset(params, 0, sizeof(*params));

	params->name = strdup(argp->name);
	params->code = argp->code;
	memcpy(&params->func, argp->callback.callback_val, argp->callback.callback_len);

	assert(sizeof(params->data)==argp->data.data_len);
	memcpy(&params->data, argp->data.data_val, sizeof(params->data));

	return params;
}

void free_callback_th_params(callback_th_params* p)
{
	//NEVER_SAFE_FREE(p->buf); - to ZAWSZE zwalnia programista!
	SAFE_FREE(p->name);
	SAFE_FREE(p);
}

void* data_callback_th(void* data)
{
	callback_th_params* params = (callback_th_params*)data;

	rf_rw_callback callback = (rf_rw_callback)params->func;
	callback(params->handle, params->code, params->buf, params->buf_len, params->data);
	
	free_callback_th_params(params);
	return NULL;
}

void* management_callback_th(void* data)
{
	callback_th_params* params = (callback_th_params*)data;

	rf_man_callback callback = (rf_man_callback)params->func;
	callback(params->code, params->name, params->data);

	free_callback_th_params(params);
	return NULL;
}


int create_callback_thread(void*(callback)(void*), callback_th_params* params)
{
    pthread_t th;
    int r = pthread_create(&th, NULL, callback, params);
    pthread_detach(th);
    return r;
}

/**
 * \brief Initialize server variables.
 */
void server_rf_init()
{
	sem_init(&fifos_sem, 0, 1);
	sem_init(&handles_sem, 0, 1);
	printf("@ Init done @\n");
}


//*****************************************************************************
//** Functions generated  
//*****************************************************************************


bool_t create_rf__1_svc(management_rf *argp, int *result, struct svc_req *rqstp)
{
	sem_wait(&fifos_sem);

	std::string name(argp->name);
	fifos_iter iter = fifos.find(name);
	if (iter!=fifos.end()) {
		sem_post(&fifos_sem);
		*result = -1;
		return TRUE;
	}

	*result = 0;
	fifos[name] = new fifo_info(name);

	sem_post(&fifos_sem);

	printf(" # create: %s : %d :: %d\n", name.c_str(), *result, argp->data.data_len);

	call_client_mngm_result(0, rqstp, argp, create_rf_res__101);

	return TRUE;
}

bool_t unlink_rf__1_svc(management_rf *argp, int *result, struct svc_req *rqstp)
{
	std::string name(argp->name);
	sem_wait(&fifos_sem);
	sem_wait(&handles_sem);
	fifos_iter iter = fifos.find(name);
	if (iter==fifos.end())
	{
		printf(" # unlink failed: %s : %d\n", name.c_str(), *result);
		sem_post(&handles_sem);
		sem_post(&fifos_sem);
		*result = -1;
	}
	else
	{
		fifo_info* item = iter->second;

		*result = sem_wait(&(item->lock));
		sem_post(&handles_sem);

		fifos.erase(iter);
		delete item;

		sem_post(&fifos_sem);

		printf(" # unlink: %s : %d\n", name.c_str(), *result);
	}
	call_client_mngm_result(0, rqstp, argp, unlink_rf_res__101);

	return TRUE;
}

bool_t open_rf__1_svc(management_rf *argp, int *result, struct svc_req *rqstp)
{
	std::string name(argp->name);

	sem_wait(&fifos_sem);
	fifos_iter iter = fifos.find(name);
	if (iter==fifos.end())
	{
		sem_post(&fifos_sem);
		*result = -1;
		return TRUE;
	}

	sem_wait(&(iter->second->lock));
	sem_post(&fifos_sem);

	sem_wait(&handles_sem);
	while (handles.find(next_handle)!=handles.end())
	{
		next_handle++;
	}
	handles[next_handle] = iter->second;
	*result = next_handle;
	next_handle++;
	sem_post(&handles_sem);

	sem_post(&(iter->second->lock));

	printf(" # open: %s : %d\n", name.c_str(), *result);
	call_client_mngm_result(*result, rqstp, argp, open_rf_res__101);

	return TRUE;
}

bool_t close_rf__1_svc(data_rf *argp, int *result, struct svc_req *rqstp)
{
	sem_wait(&handles_sem);
	int handle = argp->descriptor;
	handles_iter iter = handles.find(handle);
	if (iter==handles.end())
	{
		*result = -1;
		sem_post(&handles_sem);
		return TRUE;
	}
	std::string name(iter->second->name);
	handles.erase(iter);
	sem_post(&handles_sem);

	*result = 0;

	printf(" # close: %s : %d\n", name.c_str(), *result);
	
	management_rf argp2;
	argp2.name = (char*)name.c_str();
	argp2.data.data_len = argp->data.data_len;
	argp2.data.data_val = argp->data.data_val;
	argp2.callback.callback_len = argp->callback.callback_len;
	argp2.callback.callback_val = argp->callback.callback_val;
	argp2.uid = argp->uid;
	call_client_mngm_result(*result, rqstp, &argp2, close_rf_res__101);
	return TRUE;
}

bool_t write_rf__1_svc(data_rf *argp, int *result, struct svc_req *rqstp)
{
	sem_wait(&handles_sem);
	int handle = argp->descriptor;
	handles_iter iter = handles.find(handle);
	if (iter!=handles.end())
	{
		sem_wait(&(iter->second->lock));
		sem_post(&handles_sem);
		iter->second->data.write(argp->buf.buf_len, argp->buf.buf_val);
		sem_post(&(iter->second->lock));
		*result = 0;
		printf(" # write: %d, len: %d\n", handle, argp->buf.buf_len);
	}
	else
	{
		sem_post(&handles_sem);
		*result = -1;

		printf(" # write: %d, len: %d, FAILED: %d\n", argp->descriptor, argp->buf.buf_len, *result);
	}

	call_client_data_result(*result, NULL, 0, rqstp, argp, write_rf_res__101);

	return TRUE;
}

bool_t read_rf__1_svc(data_rf *argp, int *result, struct svc_req *rqstp)
{
	int handle = argp->descriptor;

	unsigned long long size = 0;
	memcpy(&size, argp->buf.buf_val, sizeof(size));

	printf(" # read: %d, len: %d (real len: %llu)\n", argp->descriptor, argp->buf.buf_len, size);

	sem_wait(&handles_sem);
	handles_iter iter = handles.find(handle);
	if (iter!=handles.end())
	{
		unsigned long long size = 0;
		int delta = argp->buf.buf_len;
		void* buffer = NULL;
		memcpy(&size, argp->buf.buf_val, sizeof(size));
		memcpy(&buffer, &argp->buf.buf_val[sizeof(size)], sizeof(buffer));
		argp->buf.buf_val = (char*)realloc(argp->buf.buf_val, size + argp->buf.buf_len);
		argp->buf.buf_len += size;

		*result = size;
		sem_wait(&(iter->second->lock));
		sem_post(&handles_sem);
		iter->second->data.read(size, &argp->buf.buf_val[delta]);
		sem_post(&(iter->second->lock));
	}
	else
	{
		sem_post(&handles_sem);
		*result = -1;
	}
	call_client_data_result(*result, argp->buf.buf_val, argp->buf.buf_len, rqstp, argp, read_rf_res__101);
	return TRUE;
}

int remote_fifo_1_freeresult (SVCXPRT *transp, xdrproc_t xdr_result, caddr_t result)
{
	xdr_free (xdr_result, result);

	/*
	 * Insert additional freeing code here, if needed
	 */

	return 1;
}

bool_t create_rf_res__101_svc(management_rf_res *argp, int *result, struct svc_req *rqstp)
{
    callback_th_params* params = create_callback_th_params(argp);
	create_callback_thread(management_callback_th, params);
	
	*result = 0;
	return TRUE;
}

bool_t unlink_rf_res__101_svc(management_rf_res *argp, int *result, struct svc_req *rqstp)
{
    callback_th_params* params = create_callback_th_params(argp);
	create_callback_thread(management_callback_th, params);
	
	*result = 0;
	return TRUE;
}

bool_t open_rf_res__101_svc(management_rf_res *argp, int *result, struct svc_req *rqstp)
{
    callback_th_params* params = create_callback_th_params(argp);
	create_callback_thread(management_callback_th, params);
	
	*result = 0;
	return TRUE;
}

bool_t close_rf_res__101_svc(management_rf_res *argp, int *result, struct svc_req *rqstp)
{
	callback_th_params* params = create_callback_th_params(argp);
	create_callback_thread(management_callback_th, params);

	*result = 0;
	return TRUE;
}

bool_t write_rf_res__101_svc(data_rf_res *argp, int *result, struct svc_req *rqstp)
{
	callback_th_params* params = create_callback_th_params(argp);
	create_callback_thread(data_callback_th, params);

	*result = 0;
	return TRUE;
}

bool_t read_rf_res__101_svc(data_rf_res *argp, int *result, struct svc_req *rqstp)
{
    callback_th_params* params = create_callback_th_params(argp);
    create_callback_thread(data_callback_th, params);
	
	*result = 0;
	return TRUE;
}

int remote_fifo_101_freeresult (SVCXPRT *transp, xdrproc_t xdr_result, caddr_t result)
{
	xdr_free (xdr_result, result);

	/*
	 * Insert additional freeing code here, if needed
	 */

	return 1;
}
