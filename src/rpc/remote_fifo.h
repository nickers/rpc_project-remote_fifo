/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _REMOTE_FIFO_H_RPCGEN
#define _REMOTE_FIFO_H_RPCGEN

#include <rpc/rpc.h>

#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif


struct management_rf {
	char *name;
	quad_t callback;
	quad_t data;
};
typedef struct management_rf management_rf;

struct data_rf {
	int descriptor;
	struct {
		u_int buf_len;
		char *buf_val;
	} buf;
	quad_t callback;
	quad_t data;
};
typedef struct data_rf data_rf;

struct management_rf_res {
	int code;
	char *name;
	quad_t callback;
	quad_t data;
};
typedef struct management_rf_res management_rf_res;

struct data_rf_res {
	int descriptor;
	int code;
	struct {
		u_int buf_len;
		char *buf_val;
	} buf;
	quad_t callback;
	quad_t data;
};
typedef struct data_rf_res data_rf_res;

#define REMOTE_FIFO 0x20197364
#define SERVER_API 1

#if defined(__STDC__) || defined(__cplusplus)
#define create_rf_ 1
extern  enum clnt_stat create_rf__1(management_rf *, int *, CLIENT *);
extern  bool_t create_rf__1_svc(management_rf *, int *, struct svc_req *);
#define unlink_rf_ 2
extern  enum clnt_stat unlink_rf__1(management_rf *, int *, CLIENT *);
extern  bool_t unlink_rf__1_svc(management_rf *, int *, struct svc_req *);
#define open_rf_ 101
extern  enum clnt_stat open_rf__1(management_rf *, int *, CLIENT *);
extern  bool_t open_rf__1_svc(management_rf *, int *, struct svc_req *);
#define close_rf_ 102
extern  enum clnt_stat close_rf__1(data_rf *, int *, CLIENT *);
extern  bool_t close_rf__1_svc(data_rf *, int *, struct svc_req *);
#define write_rf_ 103
extern  enum clnt_stat write_rf__1(data_rf *, int *, CLIENT *);
extern  bool_t write_rf__1_svc(data_rf *, int *, struct svc_req *);
#define read_rf_ 104
extern  enum clnt_stat read_rf__1(data_rf *, int *, CLIENT *);
extern  bool_t read_rf__1_svc(data_rf *, int *, struct svc_req *);
extern int remote_fifo_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define create_rf_ 1
extern  enum clnt_stat create_rf__1();
extern  bool_t create_rf__1_svc();
#define unlink_rf_ 2
extern  enum clnt_stat unlink_rf__1();
extern  bool_t unlink_rf__1_svc();
#define open_rf_ 101
extern  enum clnt_stat open_rf__1();
extern  bool_t open_rf__1_svc();
#define close_rf_ 102
extern  enum clnt_stat close_rf__1();
extern  bool_t close_rf__1_svc();
#define write_rf_ 103
extern  enum clnt_stat write_rf__1();
extern  bool_t write_rf__1_svc();
#define read_rf_ 104
extern  enum clnt_stat read_rf__1();
extern  bool_t read_rf__1_svc();
extern int remote_fifo_1_freeresult ();
#endif /* K&R C */
#define CLIENT_API 101

#if defined(__STDC__) || defined(__cplusplus)
#define create_rf_res_ 1
extern  enum clnt_stat create_rf_res__101(management_rf_res *, int *, CLIENT *);
extern  bool_t create_rf_res__101_svc(management_rf_res *, int *, struct svc_req *);
#define unlink_rf_res_ 2
extern  enum clnt_stat unlink_rf_res__101(management_rf_res *, int *, CLIENT *);
extern  bool_t unlink_rf_res__101_svc(management_rf_res *, int *, struct svc_req *);
#define open_rf_res_ 101
extern  enum clnt_stat open_rf_res__101(management_rf_res *, int *, CLIENT *);
extern  bool_t open_rf_res__101_svc(management_rf_res *, int *, struct svc_req *);
#define close_rf_res_ 102
extern  enum clnt_stat close_rf_res__101(management_rf_res *, int *, CLIENT *);
extern  bool_t close_rf_res__101_svc(management_rf_res *, int *, struct svc_req *);
#define write_rf_res_ 103
extern  enum clnt_stat write_rf_res__101(data_rf_res *, int *, CLIENT *);
extern  bool_t write_rf_res__101_svc(data_rf_res *, int *, struct svc_req *);
#define read_rf_res_ 104
extern  enum clnt_stat read_rf_res__101(data_rf_res *, int *, CLIENT *);
extern  bool_t read_rf_res__101_svc(data_rf_res *, int *, struct svc_req *);
extern int remote_fifo_101_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define create_rf_res_ 1
extern  enum clnt_stat create_rf_res__101();
extern  bool_t create_rf_res__101_svc();
#define unlink_rf_res_ 2
extern  enum clnt_stat unlink_rf_res__101();
extern  bool_t unlink_rf_res__101_svc();
#define open_rf_res_ 101
extern  enum clnt_stat open_rf_res__101();
extern  bool_t open_rf_res__101_svc();
#define close_rf_res_ 102
extern  enum clnt_stat close_rf_res__101();
extern  bool_t close_rf_res__101_svc();
#define write_rf_res_ 103
extern  enum clnt_stat write_rf_res__101();
extern  bool_t write_rf_res__101_svc();
#define read_rf_res_ 104
extern  enum clnt_stat read_rf_res__101();
extern  bool_t read_rf_res__101_svc();
extern int remote_fifo_101_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_management_rf (XDR *, management_rf*);
extern  bool_t xdr_data_rf (XDR *, data_rf*);
extern  bool_t xdr_management_rf_res (XDR *, management_rf_res*);
extern  bool_t xdr_data_rf_res (XDR *, data_rf_res*);

#else /* K&R C */
extern bool_t xdr_management_rf ();
extern bool_t xdr_data_rf ();
extern bool_t xdr_management_rf_res ();
extern bool_t xdr_data_rf_res ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_REMOTE_FIFO_H_RPCGEN */
