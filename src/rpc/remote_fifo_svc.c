/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "remote_fifo.h"
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

static void
remote_fifo_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		management_rf create_rf_1_arg;
		management_rf unlik_rf_1_arg;
		management_rf open_rf_1_arg;
		management_rf close_rf_1_arg;
		data_rf write_rf_1_arg;
		data_rf read_rf_1_arg;
	} argument;
	union {
		int create_rf_1_res;
		int unlik_rf_1_res;
		int open_rf_1_res;
		int close_rf_1_res;
		int write_rf_1_res;
		int read_rf_1_res;
	} result;
	bool_t retval;
	xdrproc_t _xdr_argument, _xdr_result;
	bool_t (*local)(char *, void *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case create_rf:
		_xdr_argument = (xdrproc_t) xdr_management_rf;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (bool_t (*) (char *, void *,  struct svc_req *))create_rf_1_svc;
		break;

	case unlik_rf:
		_xdr_argument = (xdrproc_t) xdr_management_rf;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (bool_t (*) (char *, void *,  struct svc_req *))unlik_rf_1_svc;
		break;

	case open_rf:
		_xdr_argument = (xdrproc_t) xdr_management_rf;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (bool_t (*) (char *, void *,  struct svc_req *))open_rf_1_svc;
		break;

	case close_rf:
		_xdr_argument = (xdrproc_t) xdr_management_rf;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (bool_t (*) (char *, void *,  struct svc_req *))close_rf_1_svc;
		break;

	case write_rf:
		_xdr_argument = (xdrproc_t) xdr_data_rf;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (bool_t (*) (char *, void *,  struct svc_req *))write_rf_1_svc;
		break;

	case read_rf:
		_xdr_argument = (xdrproc_t) xdr_data_rf;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (bool_t (*) (char *, void *,  struct svc_req *))read_rf_1_svc;
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
	retval = (bool_t) (*local)((char *)&argument, (void *)&result, rqstp);
	if (retval > 0 && !svc_sendreply(transp, (xdrproc_t) _xdr_result, (char *)&result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	if (!remote_fifo_1_freeresult (transp, _xdr_result, (caddr_t) &result))
		fprintf (stderr, "%s", "unable to free results");

	return;
}

static void
remote_fifo_101(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		management_rf_res create_rf_res_101_arg;
		management_rf_res unlik_rf_res_101_arg;
		management_rf_res open_rf_res_101_arg;
		management_rf_res close_rf_res_101_arg;
		data_rf_res write_rf_res_101_arg;
		data_rf_res read_rf_res_101_arg;
	} argument;
	union {
		int create_rf_res_101_res;
		int unlik_rf_res_101_res;
		int open_rf_res_101_res;
		int close_rf_res_101_res;
		int write_rf_res_101_res;
		int read_rf_res_101_res;
	} result;
	bool_t retval;
	xdrproc_t _xdr_argument, _xdr_result;
	bool_t (*local)(char *, void *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case create_rf_res:
		_xdr_argument = (xdrproc_t) xdr_management_rf_res;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (bool_t (*) (char *, void *,  struct svc_req *))create_rf_res_101_svc;
		break;

	case unlik_rf_res:
		_xdr_argument = (xdrproc_t) xdr_management_rf_res;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (bool_t (*) (char *, void *,  struct svc_req *))unlik_rf_res_101_svc;
		break;

	case open_rf_res:
		_xdr_argument = (xdrproc_t) xdr_management_rf_res;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (bool_t (*) (char *, void *,  struct svc_req *))open_rf_res_101_svc;
		break;

	case close_rf_res:
		_xdr_argument = (xdrproc_t) xdr_management_rf_res;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (bool_t (*) (char *, void *,  struct svc_req *))close_rf_res_101_svc;
		break;

	case write_rf_res:
		_xdr_argument = (xdrproc_t) xdr_data_rf_res;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (bool_t (*) (char *, void *,  struct svc_req *))write_rf_res_101_svc;
		break;

	case read_rf_res:
		_xdr_argument = (xdrproc_t) xdr_data_rf_res;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (bool_t (*) (char *, void *,  struct svc_req *))read_rf_res_101_svc;
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
	retval = (bool_t) (*local)((char *)&argument, (void *)&result, rqstp);
	if (retval > 0 && !svc_sendreply(transp, (xdrproc_t) _xdr_result, (char *)&result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	if (!remote_fifo_101_freeresult (transp, _xdr_result, (caddr_t) &result))
		fprintf (stderr, "%s", "unable to free results");

	return;
}

int
main (int argc, char **argv)
{
	register SVCXPRT *transp;

	pmap_unset (REMOTE_FIFO, SERVER_API);
	pmap_unset (REMOTE_FIFO, CLIENT_API);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create udp service.");
		exit(1);
	}
	if (!svc_register(transp, REMOTE_FIFO, SERVER_API, remote_fifo_1, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (REMOTE_FIFO, SERVER_API, udp).");
		exit(1);
	}
	if (!svc_register(transp, REMOTE_FIFO, CLIENT_API, remote_fifo_101, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (REMOTE_FIFO, CLIENT_API, udp).");
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
	if (!svc_register(transp, REMOTE_FIFO, CLIENT_API, remote_fifo_101, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (REMOTE_FIFO, CLIENT_API, tcp).");
		exit(1);
	}

	svc_run ();
	fprintf (stderr, "%s", "svc_run returned");
	exit (1);
	/* NOTREACHED */
}