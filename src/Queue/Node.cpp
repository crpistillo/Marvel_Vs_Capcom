/*
 * Nodo.cpp
 *
 *  Created on: 12 may. 2019
 *      Author: caropistillo
 */

#include "Node.h"


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



