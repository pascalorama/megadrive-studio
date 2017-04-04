///////////////////////////////////////////////////////////////////////////////
// mdList
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __mdList__
#define	__mdList__

//////////////////////////////////////////////////////////////////////////////
// Class definition : tgNODE
//////////////////////////////////////////////////////////////////////////////

template<class T> 
class mdListNode
{
	public:	
	T*	object;
	int index;		
	
	mdListNode<T> *prev,*next;
	mdListNode();
};

//////////////////////////////////////////////////////////////////////////////
// Class definition : mdList
///////////////////////////////////////////////////////////////////////////////

template <class T> 
class mdList
{	
	///////////////////////////////////////////////////////////////////////////
	public:
	///////

	int	Id;
	mdList();
	~mdList();

	int PushBack(T*);
	int Insert(int,T*);
	T* PopBack(void);
	T* Remove(mdListNode<T>*);
	T* Remove(int);

	mdListNode<T>*  MoveFirst(void);
	mdListNode<T>*  MoveNext(mdListNode<T>*);
	mdListNode<T>*  MoveNext(mdListNode<T>*,int);
	mdListNode<T>*  MoveLast(void);
	mdListNode<T>*  MovePrevious(mdListNode<T>*);
	mdListNode<T>*	GetNode(int index);

	T*	Item(int index);
	int	GetCount(void);
	
	///////////////////////////////////////////////////////////////////////////
	private:
	////////

	void RecomputeNodeIndex(mdListNode<T>*,int);
	mdListNode<T> head,tail;
	mdListNode<T>* last_node;	
	int Count;
};

///////////////////////////////////////////////////////////////////////////////
// Constructor mdListNode

template<class T> mdListNode<T>::mdListNode()
{
;
}

///////////////////////////////////////////////////////////////////////////////
// Contructor

template<class T> mdList<T>::mdList()
{
	Id=0;
	Count = 0;

	head.index=-1;
	tail.index=-2;
	last_node=NULL;

	head.prev=NULL;
	head.next=&tail;

	tail.next=NULL;
	tail.prev=&head;
}

///////////////////////////////////////////////////////////////////////////////
// Destructor

template<class T> mdList<T>::~mdList(void)
{

	while(PopBack()!=NULL)
		;
}


///////////////////////////////////////////////////////////////////////////////
// Push an object on the stack

template<class T> int mdList<T>::PushBack(T* obj)
{
	int Index = Id;	
	mdListNode<T>	*node;

	if(obj!=NULL)
	{	
		node=new mdListNode<T>;

		node->next=&tail;
		node->prev=tail.prev;
		node->object=obj;
		node->index = Count;
		
		Id ++;
		Count ++;
		
		(tail.prev)->next=node;
		tail.prev=node;
	}

	return Index;
}

///////////////////////////////////////////////////////////////////////////////
// Insert object

template<class T> int mdList<T>::Insert(int index,T* obj)
{
	mdListNode<T>	*new_node;
	mdListNode<T>	*node=GetNode(index);

	if(node!=NULL)
	{
		new_node=new mdListNode<T>;

		new_node->next=node;
		new_node->prev=node->prev;
		new_node->object=obj;
		new_node->index = index;

		node->prev = new_node;
		(new_node->prev)->next = new_node;

		RecomputeNodeIndex(node,index+1);
			
		Count ++;	

		return index;
	}
	else
	{
		return -1;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Pop a object

template<class T> T* mdList<T>::PopBack(void)
{
	mdListNode<T>	*node=tail.prev;
	T* obj;

	if(node!=&head)
	{
		tail.prev = node->prev;
		(node->prev)->next = &tail;
		obj = node->object;
		delete(node);

		Count--;
					
		return obj;		
	}	
	else
	{
		return NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Move first

template<class T> mdListNode<T>*  mdList<T>::MoveFirst(void)
{
	if(head.next!=&tail)
		return (mdListNode<T>*) head.next;
	else
		return NULL;
}


///////////////////////////////////////////////////////////////////////////////
// Move next

template<class T> mdListNode<T>*  mdList<T>::MoveNext(mdListNode<T>* node)
{
	if(node->next!= &tail)
		return (mdListNode<T>*) node->next;
	else
		return NULL;
}

template<class T> mdListNode<T>* mdList<T>::MoveNext(mdListNode<T>* node,int i)
{	
	while(node->next!= &tail && i>0)
	{	node=node->next;
	}		
	return node;
}

///////////////////////////////////////////////////////////////////////////////
// Move last

template<class T> mdListNode<T>*  mdList<T>::MoveLast(void)
{
	if(tail.prev!=&head)
		return (mdListNode<T>*) tail.prev;
	else
		return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Move previous

template<class T> mdListNode<T>*  mdList<T>::MovePrevious(mdListNode<T>* node)
{
	if(node->next!= &tail)
		return (mdListNode<T>*) node->next;
	else
		return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Remove node

template<class T> T*  mdList<T>::Remove(mdListNode<T>* node)
{
	T* obj=NULL;

	if(node!=NULL) 
		obj = node->object;		
	else
	{	return NULL;}
	Count--;

	RecomputeNodeIndex( (node->next),node->index);
	
	(node->prev)->next = node->next;
	(node->next)->prev = node->prev;
	node->next=NULL;
	node->prev=NULL;
	
	delete(node);
				
	return obj;
}

template<class T> T*  mdList<T>::Remove(int index)
{
	T* obj=NULL;
	mdListNode<T>* node = GetNode(index);
				
	return Remove(node);	
}

///////////////////////////////////////////////////////////////////////////////
// Recompute index

template<class T> void  mdList<T>::RecomputeNodeIndex(mdListNode<T>* node,int start_index)
{
	mdListNode<T>* ptr=node;

	while(ptr!=NULL)
	{
		ptr->index = start_index++;
		ptr=MoveNext(ptr);
	}
}

///////////////////////////////////////////////////////////////////////////////
// Get The item

template<class T> T*  mdList<T>::Item(int index)
{
	mdListNode<T>* ptr=GetNode(index);
	if(ptr==NULL)
		return NULL;
	else
		return ptr->object;
}

///////////////////////////////////////////////////////////////////////////////
// Get the node

template<class T> mdListNode<T>*  mdList<T>::GetNode(int index)
{
	int loop=1;	
	mdListNode<T>* ptr=NULL;
		
	if(last_node!=NULL)
	{	if(last_node->index<index)		
			ptr=last_node;			
		else
			ptr=MoveFirst();			
	}	
	else
	{	ptr=MoveFirst();}
	
	if(index>(Count-1))
		return NULL;
	
	while(ptr!=NULL && loop==1)
	{	
		if(ptr->index==index)
		{
			loop=0;
			last_node=ptr;
			return ptr;
		}
		else
			ptr = MoveNext(ptr);
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Count

template<class T> int mdList<T>::GetCount(void)
{
	return Count;
}


#endif