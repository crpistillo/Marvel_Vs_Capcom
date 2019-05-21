/*
 * Nodo.h
 *
 *  Created on: 12 may. 2019
 *      Author: caropistillo
 */

#ifndef QUEUE_NODE_H_
#define QUEUE_NODE_H_

template < typename Data >
class Node
{
 private:
	Data data; // Dato a almacenar
	Node* nextPointer; // Puntero a otro nodo
 public:

 	 Node(Data data);
 	 ~Node(){}
 	 void set_data(Data data);
 	 // Setea el puntero al siguiente nodo
 	 void set_nextPointer(Node* nextPointer);
 	 Data get_data();
 	 // Obtiene el puntero al siguiente nodo
 	 Node* get_nextPointer();
 	 bool hasNext();
 };

template < typename Data >
Node<Data>::Node(Data data)
{
	 this->data = data;
	 this->nextPointer = 0;
}

template < typename Data >
void Node<Data>::set_data(Data data)
{
	 this->data = data;
}

template < typename Data >
void Node<Data>::set_nextPointer(Node* nextPointer)
{
	 this->nextPointer = nextPointer;
}

template < typename Data >
Data Node<Data>::get_data()
{
	 return this->data;
}

template < typename Data >
Node<Data>* Node<Data>::get_nextPointer()
{
	 return this->nextPointer;
}

template < typename Data >
bool Node<Data>::hasNext()
{
	 return (this->nextPointer != 0);
}


#endif /* QUEUE_NODE_H_ */
