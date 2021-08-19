#include "linked_list.h"

List::List( ) 
{ 
   header = new ListNode; } /* Test if the list is logically empty. 
 * return true if empty, false otherwise.*/ 


/* Test if the list is logically empty. 
 * return true if empty, false otherwise.*/
bool List::isEmpty( ) const { 
	return header->next == NULL; 
}

/* Return an iterator representing the header node. */ 
ListItr List::zeroth( ) const {    
	return ListItr( header ); 
}

/* Return an iterator representing the first node in 
the list. This operation is valid for empty lists.*/
ListItr List::first( ) const {
	return ListItr( header->next ); 
}


ListItr List::find(const int &xa,const int &xb,const int &ya, const int &yb) const{ 
 	ListNode *itr = header->next;
 	while(itr != NULL &&( itr->x1 != xa || itr->x2 != xb))      
 		itr = itr->next;
 	return ListItr( itr );
 }


void List::remove( const int & x ) 
{    ListItr p = findPrevious( x );
	 if( p.current->next != NULL ){
	 	ListNode *oldNode = p.current->next;
	 	p.current->next = p.current->next->next;
	 	delete oldNode;    
	 	} 
}

ListItr List::findPrevious( const int & x ) const {
    ListNode *itr = header;
    while(itr->next!=NULL && itr->next->element!=x)
    	itr = itr->next;
    return ListItr( itr ); }

void List::insert(const int & x, const ListItr & p ){
	if( p.current != NULL )
		p.current->next = new ListNode(x,p.current->next ); 
}

void List::makeEmpty( ){
	while( !isEmpty( ) )
		remove( first( ).retrieve( ) );
}

List::~List( ){
	makeEmpty( );
	delete header;
}

