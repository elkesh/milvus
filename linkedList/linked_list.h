
struct ListNode
{
	int x1;
	int y1;
	int x2;
	int y2;

	ListNode *next;
};



class ListItr {  
	public: 
	   ListItr( ) : current( NULL ) { } 
	   bool isValid( ) const    { return current != NULL; }
	   void advance( )    { if(isValid( ) ) current = current->next; }
	   const int & retrieve( ) const { 
	   if( !isValid( ) ) throw BadIterator( );return current->element; }

	private:    
   		ListNode *current;    // Current position     
   		ListItr( ListNode *theNode ) 
     	: current( theNode ) { } 
    friend class List; //Grant access to constructor 
};


class List{

	public:

	    List( );
	    ~List( );

		bool isEmpty( ) const;
		void makeEmpty( );
		ListItr zeroth( ) const;
		ListItr first( ) const;
		void insert(const int &xa,const int &xb,const int &ya, const int &yb, const  ListItr & p);
		ListItr find(const int &xa,const int &xb,const int &ya, const int &yb) const;
		ListItr findPrevious(const int &xa,const int &xb,const int &ya, const int &yb) const;
		void remove(const int &xa,const int &xb,const int &ya, const int &yb);

    private:
    	ListNode *header;


};



