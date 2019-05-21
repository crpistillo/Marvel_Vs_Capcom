/*
 * Cola.h
 *
 *  Created on: 12 may. 2019
 *      Author: caropistillo
 */

#ifndef QUEUE_QUEUE_H_
#define QUEUE_QUEUE_H_

#include "Node.h"

template <typename Data>
class Queue
{
private:
	Node<Data>* first;// Primer elemento de la cola
	Node<Data>* last; // Ultimo elemento de la cola
	int size;

public:
	 Queue();
	 ~Queue();
	 bool empty_queue();
	 void insert(Data data);
	 Data get_data();
	 void delete_data();
	 int get_size();

};

template <typename Data>
Queue<Data>::Queue()
{
	this->first = 0;
	this->last = 0;
	this->size = 0;
}

template <typename Data>
Queue<Data>::~Queue()
{
	while (!(this->empty_queue()))
	this->delete_data();
}

template <typename Data>
bool Queue<Data>::empty_queue()
{
	return (this->first == 0);
}

template <typename Data>
void Queue<Data>::insert(Data data)
{
	Node<Data>* ptrNode = new Node<Data>(data);
	if (this->empty_queue())
	{
		this->first = ptrNode;
	}
	else
	{
		this->last->set_nextPointer(ptrNode);
	}

	this->last = ptrNode;
	this->size++;
}

template <typename Data>
Data Queue<Data>::get_data()
{
    return this->first->get_data();
}

template <typename Data>
void Queue<Data>::delete_data()
{
	Node<Data>* ptrAux = this->first;
	this->first = ptrAux->get_nextPointer();
	delete ptrAux;
	this->size--;
}

template <typename Data>
int Queue<Data>::get_size()
{
	return this->size;
}

#endif /* QUEUE_QUEUE_H_ */
