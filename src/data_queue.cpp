#include <string.h>
#include <queue>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>


using namespace std;


class data_queue {
private:

	class data_part {
	public:
		int length;
		int readed;
		char* data;

		data_part(int l, char* buf) {
			data = new char[length];
			memcpy(data, buf, length);
			length = length;
			readed = 0;
		}

		~data_part() {
			delete[] data;
			length = readed = 0;
		}
	};

	std::queue<data_part*> data_stream;
	//std::queue<int>	data_wait;
	int data_wait_sem;
	int read_sem;
	int write_sem;

	void sem_change(int sem, int val) {
		struct sembuf op;
		op.sem_num = 0;
		op.sem_op = val;
		op.sem_flg = 0;
		semop(sem, &op, 1);
	}

	void wait_for_sem(int sem) {
		sem_change(sem, -1);
	}

	void free_sem(int sem) {
		sem_change(sem, 1);
	}

public:

	data_queue() {
		data_wait_sem = semget(IPC_PRIVATE, 1, IPC_CREAT);
		semctl(data_wait_sem, 0, SETVAL, 0);
		write_sem = semget(IPC_PRIVATE, 1, IPC_CREAT);
		semctl(write_sem, 0, SETVAL, 1);
		read_sem = semget(IPC_PRIVATE, 1, IPC_CREAT);
		semctl(read_sem, 0, SETVAL, 1);
	}

	void add(int length, char* data);
	char* read(int length, char* buf=NULL);
};


void data_queue::add(int length, char* buf)
{
	data_part *item = new data_part(length, buf);
	// TODO synchronizacja - WRITE
	wait_for_sem(write_sem);
	data_stream.push(item);

	// TODO sync. - kolejka oczekiwania na dane
	semctl(data_wait_sem, 0, SETVAL, 1);

	// TODO synchronizacja - WRITE end
	free_sem(write_sem);
}

char* data_queue::read(int length, char* buf)
{
	int readed = 0;
	// TODO synchronizacja - READ
	wait_for_sem(read_sem);

	while (readed<length) {
		if (data_stream.empty()) {
			wait_for_sem(data_wait_sem);
		}
		data_part* item = data_stream.front();
		
		int to_read = item->length - item->readed;
		if (to_read>length-readed) {
			to_read = length-readed;
		}
		memcpy(&buf[readed], &(item->data[item->readed]), to_read);
		readed += to_read;

		item->readed += to_read;
		if (item->readed>=item->length) {
			wait_for_sem(write_sem);
			data_stream.pop();
			free_sem(write_sem);
		}
	}

	// TODO synchronizacja - READ end
	free_sem(read_sem);
}
