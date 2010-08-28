
typedef struct __rf_client_instance {
	long long unique_id;
	pthread_t client_thread;
} rf_client_instance;

rf_client_instance run_rf_client(long long unique_id);
void stop_rf_client(rf_client_instance *inst);
