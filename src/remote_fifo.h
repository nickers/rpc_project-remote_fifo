#include "rpc/remote_fifo.h"

/* ============================================================================
 * == callbacks types
 * ============================================================================ */

/**
 * rf_man_callback is for management operations: rf_create, rf_open, rf_close, rf_unlink.
 */
typedef void (*rf_man_callback)(int code, char* name, void* data);

/**
 * rf_rw_callback is for read/write operations rf_read, rf_write.
 */
typedef void (*rf_rw_callback)(int handle, int code, void* buf, int len, void* data);

/**
 *
 */
int init_rf(char* host);

/**
 * \brief Create new remote fifo.
 * \param name Name of remote-fifo to be created.
 * \param callback Function where to pass results.
 * \param data Pointer which will be passed to 'callback' function.
 * \param host Explicite server host which should be used. Give NULL to use host given in 'init_rf' function. [default: NULL]
 * \return 0 on success, <0 on error ( -1 - already exists ).
 */
extern int create_rf(char* name, rf_man_callback callback, void* data, char* host=NULL);
extern int unlink_rf(char* name, rf_man_callback callback, void* data, char* host=NULL);

extern int  open_rf(char* name, rf_man_callback callback,  void* data, char* host=NULL);
extern int close_rf(int handle, rf_man_callback callback, void* data);

extern int  read_rf(int handle, void* buffer, unsigned long long size, rf_rw_callback callback, void* data);
extern int write_rf(int handle, void* buffer, unsigned long long size, rf_rw_callback callback, void* data);
