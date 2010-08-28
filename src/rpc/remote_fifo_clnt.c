/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include <memory.h> /* for memset */
#include "remote_fifo.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 5, 0 };

enum clnt_stat 
create_rf__1(management_rf *argp, int *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, create_rf_,
		(xdrproc_t) xdr_management_rf, (caddr_t) argp,
		(xdrproc_t) xdr_int, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
unlink_rf__1(management_rf *argp, int *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, unlink_rf_,
		(xdrproc_t) xdr_management_rf, (caddr_t) argp,
		(xdrproc_t) xdr_int, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
open_rf__1(management_rf *argp, int *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, open_rf_,
		(xdrproc_t) xdr_management_rf, (caddr_t) argp,
		(xdrproc_t) xdr_int, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
close_rf__1(data_rf *argp, int *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, close_rf_,
		(xdrproc_t) xdr_data_rf, (caddr_t) argp,
		(xdrproc_t) xdr_int, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
write_rf__1(data_rf *argp, int *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, write_rf_,
		(xdrproc_t) xdr_data_rf, (caddr_t) argp,
		(xdrproc_t) xdr_int, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
read_rf__1(data_rf *argp, int *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, read_rf_,
		(xdrproc_t) xdr_data_rf, (caddr_t) argp,
		(xdrproc_t) xdr_int, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
create_rf_res__101(management_rf_res *argp, int *clnt_res, CLIENT *clnt)
{
printf("call\n\n");
	return (clnt_call(clnt, create_rf_res_,
		(xdrproc_t) xdr_management_rf_res, (caddr_t) argp,
		(xdrproc_t) xdr_int, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
unlink_rf_res__101(management_rf_res *argp, int *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, unlink_rf_res_,
		(xdrproc_t) xdr_management_rf_res, (caddr_t) argp,
		(xdrproc_t) xdr_int, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
open_rf_res__101(management_rf_res *argp, int *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, open_rf_res_,
		(xdrproc_t) xdr_management_rf_res, (caddr_t) argp,
		(xdrproc_t) xdr_int, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
close_rf_res__101(management_rf_res *argp, int *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, close_rf_res_,
		(xdrproc_t) xdr_management_rf_res, (caddr_t) argp,
		(xdrproc_t) xdr_int, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
write_rf_res__101(data_rf_res *argp, int *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, write_rf_res_,
		(xdrproc_t) xdr_data_rf_res, (caddr_t) argp,
		(xdrproc_t) xdr_int, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
read_rf_res__101(data_rf_res *argp, int *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, read_rf_res_,
		(xdrproc_t) xdr_data_rf_res, (caddr_t) argp,
		(xdrproc_t) xdr_int, (caddr_t) clnt_res,
		TIMEOUT));
}
