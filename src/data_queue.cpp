#include <string.h>
#include <queue>
#include <semaphore.h>
#include "data_queue.h"


using namespace std;


data_queue::data_part::data_part(int l, char* buf)
{
	data = new char[l];
	memcpy(data, buf, l);
	length = l;
	readed = 0;
}

data_queue::data_part::~data_part()
{
	delete[] data;
	length = readed = 0;
}

//-----------

data_queue::data_queue()
{
	// nic nie czeka..
	sem_init(&data_wait_sem, 0, 0);

	// mozna zmieniac kolejke...
	sem_init(&write_sem, 1, 1);

	// mozna czytac z kolejki (choc sie zawisnie w oczekiwaniu na dane)...
	sem_init(&read_sem, 1, 1);
}

/**
 * \brief Sprzatamy kolejke.
 */
data_queue::~data_queue()
{
	while (!data_stream.empty())
	{
		delete data_stream.front();
		data_stream.pop();
	}
	sem_destroy(&data_wait_sem);
	sem_destroy(&write_sem);
	sem_destroy(&read_sem);
}


void data_queue::write(int length, char* buf)
{
	data_part *item = new data_part(length, buf);

	// synchronizacja - WRITE begin
	sem_wait(&write_sem);

	data_stream.push(item);

	// synchronizacja - kolejka oczekiwania na dane
	sem_post(&data_wait_sem);

	// synchronizacja - WRITE end
	sem_post(&write_sem);
}

char* data_queue::read(int length, char* buf)
{
	int readed = 0;

	// synchronizacja - READ begin
	sem_wait(&read_sem);

	while (readed < length)
	{
		// jesli cos mamy w kolejce...
		sem_wait(&data_wait_sem);

		data_part* item = data_stream.front();

		int item_left_data = item->length - item->readed;
		int buffer_left_space = length - readed;

		int to_read = (item_left_data > buffer_left_space) ? buffer_left_space : item_left_data;

		memcpy(&buf[readed], &(item->data[item->readed]), to_read);
		readed += to_read;

		item->readed += to_read;
		if (item->readed >= item->length)
		{
			// synchronizacja - WRITE begin
			sem_wait(&write_sem);
			delete item;
			data_stream.pop();
			// synchronizacja - WRITE end
			sem_post(&write_sem);
		}
		else
		{
			sem_post(&data_wait_sem);
		}
	}

	//  synchronizacja - READ end
	sem_post(&read_sem);

	return buf;
}
