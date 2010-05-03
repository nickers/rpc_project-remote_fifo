
/*
 * callbacks types
 */

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
int create_rf(char* name, rf_man_callback callback, void* data);
int unlink_rf(char* name, rf_man_callback callback, void* data);

int  open_rf(char* name, rf_man_callback open_callback,  void* data);
int close_rf(int handle, rf_man_callback close_callback, void* data);

int  read_rf(int handle, void* buffer, unsigned long long size, rf_rw_callback callback, void* data);
int write_rf(int handle, void* buffer, unsigned long long size, rf_rw_callback callback, void* data);
