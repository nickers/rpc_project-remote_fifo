#include <queue>
#include <semaphore.h>

class data_queue {
private:

	class data_part {
	public:
		int length;
		int readed;
		char* data;

		data_part(int l, char* buf);
		~data_part();
	};

	std::queue<data_part*> data_stream;

	/**
	* 'Ilosc' elementow do odczytania w kolejce.
	*/
	sem_t data_wait_sem;

	/**
	* Mutex do czytania danych.
	*/
	sem_t read_sem;

	/**
	* Mutex do zmian w samej kolejce (nie jej elementach!).
	*/
	sem_t write_sem;

public:

	data_queue();
	~data_queue();
	void write(int length, char* data);
	char* read(int length, char* buf=NULL);
};

