#include "remote_fifo.h"
#include "remote_fifo_server_instance.h"
#include <rpc/pmap_clnt.h>
#include <pthread.h>

static void remote_fifo_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		management_rf create_rf__1_arg;
		management_rf unlink_rf__1_arg;
		management_rf open_rf__1_arg;
		data_rf close_rf__1_arg;
		data_rf write_rf__1_arg;
		data_rf read_rf__1_arg;
	} argument;
	union {
		int create_rf__1_res;
		int unlink_rf__1_res;
		int open_rf__1_res;
		int close_rf__1_res;
		int write_rf__1_res;
		int read_rf__1_res;
	} result;
	bool_t retval;
	xdrproc_t _xdr_argument, _xdr_result;
	bool_t (*local)(char *, void *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case create_rf_:
		_xdr_argument = (xdrproc_t) xdr_management_rf;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (bool_t (*) (char *, void *,  struct svc_req *))create_rf__1_svc;
		break;

	case unlink_rf_:
		_xdr_argument = (xdrproc_t) xdr_management_rf;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (bool_t (*) (char *, void *,  struct svc_req *))unlink_rf__1_svc;
		break;

	case open_rf_:
		_xdr_argument = (xdrproc_t) xdr_management_rf;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (bool_t (*) (char *, void *,  struct svc_req *))open_rf__1_svc;
		break;

	case close_rf_:
		_xdr_argument = (xdrproc_t) xdr_data_rf;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (bool_t (*) (char *, void *,  struct svc_req *))close_rf__1_svc;
		break;

	case write_rf_:
		_xdr_argument = (xdrproc_t) xdr_data_rf;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (bool_t (*) (char *, void *,  struct svc_req *))write_rf__1_svc;
		break;

	case read_rf_:
		_xdr_argument = (xdrproc_t) xdr_data_rf;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (bool_t (*) (char *, void *,  struct svc_req *))read_rf__1_svc;
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
	//retval = (bool_t) (*local)((char *)&argument, (void *)&result, rqstp);
	retval = TRUE;
	result.close_rf__1_res = 0;
	if (retval > 0 && !svc_sendreply(transp, (xdrproc_t) _xdr_result, (char *)&result)) {
		svcerr_systemerr (transp);
	}

	(*local)((char *)&argument, (void *)&result, rqstp);

	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	if (!remote_fifo_1_freeresult (transp, _xdr_result, (caddr_t) &result))
		fprintf (stderr, "%s", "unable to free results");

	return;
}



void run_rf_server()
{
	register SVCXPRT *transp;

	pmap_unset (REMOTE_FIFO, SERVER_API);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create udp service.");
		exit(1);
	}
	if (!svc_register(transp, REMOTE_FIFO, SERVER_API, remote_fifo_1, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (REMOTE_FIFO, SERVER_API, udp).");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create tcp service.");
		exit(1);
	}
	if (!svc_register(transp, REMOTE_FIFO, SERVER_API, remote_fifo_1, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (REMOTE_FIFO, SERVER_API, tcp).");
		exit(1);
	}

	// TODO w serwerze też wynieść to do osobnego wątku ?
	svc_run ();
}

