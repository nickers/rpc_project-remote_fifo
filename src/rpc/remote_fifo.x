struct management_rf {
	string name<>;
	hyper callback;
	hyper data;
};

struct data_rf {
	int descriptor;
	char buf<>;
	hyper callback;
	hyper data;
};

struct management_rf_res {
	int code;
	string name<>;
	hyper callback;
	hyper data;
};

struct data_rf_res {
	int descriptor;
	int code;
	char buf<>;
	hyper callback;
	hyper data;
};

program REMOTE_FIFO {
	version SERVER_API {
		int create_rf(management_rf in) 	= 1;
		int unlik_rf(management_rf in) 	= 2;
		int open_rf(management_rf in) 	= 101;
		int close_rf(management_rf in) 	= 102;
		int write_rf(data_rf in) 		    = 103;
		int read_rf(data_rf in) 		    = 104;
	} = 1;

	version CLIENT_API {
		int create_rf_res(management_rf_res in) 	= 1;
		int unlik_rf_res(management_rf_res in) 	= 2;
		int open_rf_res(management_rf_res in) 	= 101;
		int close_rf_res(management_rf_res in) 	= 102;
		int write_rf_res(data_rf_res in) 		    = 103;
		int read_rf_res(data_rf_res in) 		    = 104;
	} = 101;

} = 0x20197364;
