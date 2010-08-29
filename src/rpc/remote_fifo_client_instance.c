#include "remote_fifo.h"
#include "remote_fifo_client_instance.h"
#include <rpc/pmap_clnt.h>
#include <pthread.h>
#include <semaphore.h>


static void remote_fifo_101(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		management_rf_res create_rf_res__101_arg;
		management_rf_res unlink_rf_res__101_arg;
		management_rf_res open_rf_res__101_arg;
		management_rf_res close_rf_res__101_arg;
		data_rf_res write_rf_res__101_arg;
		data_rf_res read_rf_res__101_arg;
	} argument;
	union {
		int create_rf_res__101_res;
		int unlink_rf_res__101_res;
		int open_rf_res__101_res;
		int close_rf_res__101_res;
		int write_rf_res__101_res;
		int read_rf_res__101_res;
	} result;
	bool_t retval;
	xdrproc_t _xdr_argument, _xdr_result;
	bool_t (*local)(char *, void *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case create_rf_res_:
		_xdr_argument = (xdrproc_t) xdr_management_rf_res;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (bool_t (*) (char *, void *,  struct svc_req *))create_rf_res__101_svc;
		break;

	case unlink_rf_res_:
		_xdr_argument = (xdrproc_t) xdr_management_rf_res;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (bool_t (*) (char *, void *,  struct svc_req *))unlink_rf_res__101_svc;
		break;

	case open_rf_res_:
		_xdr_argument = (xdrproc_t) xdr_management_rf_res;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (bool_t (*) (char *, void *,  struct svc_req *))open_rf_res__101_svc;
		break;

	case close_rf_res_:
		_xdr_argument = (xdrproc_t) xdr_management_rf_res;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (bool_t (*) (char *, void *,  struct svc_req *))close_rf_res__101_svc;
		break;

	case write_rf_res_:
		_xdr_argument = (xdrproc_t) xdr_data_rf_res;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (bool_t (*) (char *, void *,  struct svc_req *))write_rf_res__101_svc;
		break;

	case read_rf_res_:
		_xdr_argument = (xdrproc_t) xdr_data_rf_res;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (bool_t (*) (char *, void *,  struct svc_req *))read_rf_res__101_svc;
		break;

	default:
		svcerr_noproc (transp);
		return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	retval = 1;
	result.close_rf_res__101_res = 0;
	if (retval > 0 && !svc_sendreply(transp, (xdrproc_t) _xdr_result, (char *)&result)) {
		svcerr_systemerr (transp);
	}
	retval = (bool_t) (*local)((char *)&argument, (void *)&result, rqstp);
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	if (!remote_fifo_101_freeresult (transp, _xdr_result, (caddr_t) &result))
		fprintf (stderr, "%s", "unable to free results");

	return;
}

sem_t starting;


/**
 * Client-side working thread.
 */
void* client_thread(void* param)
{
	long long unique_id = *(long long*)param;
	free(param);

	register SVCXPRT *transp;

	pmap_unset (REMOTE_FIFO, CLIENT_API + unique_id);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create udp service.");
		exit(1);
	}

	if (!svc_register(transp, REMOTE_FIFO, CLIENT_API + unique_id, remote_fifo_101, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (REMOTE_FIFO, CLIENT_API, udp).");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create tcp service.");
		exit(1);
	}
	if (!svc_register(transp, REMOTE_FIFO, CLIENT_API + unique_id, remote_fifo_101, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (REMOTE_FIFO, CLIENT_API, tcp).");
		exit(1);
	}
	printf("Thread created\n");
	sem_post(&starting);
	svc_run();
	printf("HOW??\n\n");
	return NULL;
}

/**
 *
 */
rf_client_instance run_rf_client(long long unique_id)
{
	sem_init(&starting, 0, 0);
	rf_client_instance inst;
	long long *param = (long long*)malloc(sizeof(long long));
	*param = unique_id;
	inst.unique_id = unique_id;
	printf(" RPC thread started: %d\n", pthread_create(&inst.client_thread, NULL, client_thread, param));
	sem_wait(&starting);
	sem_destroy(&starting);
	return inst;
}


void stop_rf_client(rf_client_instance *inst)
{
	pthread_cancel(inst->client_thread);
	pthread_join(inst->client_thread, NULL);
	memset(inst, 0, sizeof(*inst));
}
