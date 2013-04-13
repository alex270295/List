#define BOOST_TEST_MODULE list_testing
#define BOOST_MAIN
#include <boost\test\unit_test.hpp>
#include <stdio.h>
#include <conio.h>


struct node
{
	node* next;
	node* prev;
	int value;

	node()
	{
		this->value=0;
		this->next=nullptr;
		this->prev=nullptr;
	}

	~node(){}

	node(node* _next, node* _prev, int _value)
	{
		this->value=_value;
		this->next=_next;
		this->prev=_prev;
	}

	node(node* _node)
	{
		this->value=_node->value;
		this->next=_node->next;
		this->prev=_node->prev;
	}


	node(int _value)
	{
		this->value=_value;
		this->next=nullptr;
		this->prev=nullptr;
	}

	void operator=(node* const& _node)
	{
		this->value=_node->value;
		this->next=_node->next;
		this->prev=_node->prev;
	}

	bool operator==(node const& _node)
	{
		return (_node.next==next && _node.prev==prev && _node.value==value);
	}

	bool operator!=(node const& _node)
	{
		return !(_node.next==next && _node.prev==prev && _node.value==value);
	}
};

struct iterator
{
	node* cur_node;

	iterator()
	{
		cur_node=nullptr;
	}

	~iterator(){}

	iterator(node* _node)
	{
		cur_node=_node;
	}

	int operator*()const
	{
		return cur_node->value;
	}

	iterator& operator++()
	{
		this->cur_node=this->cur_node->next;
		return *this;
	}

	iterator operator++(int)
	{
		this->cur_node=this->cur_node->next;
		return *this;
	}

	iterator& operator--()
	{
		this->cur_node=this->cur_node->next;
		return *this;	
	}

	iterator operator--(int)
	{
		this->cur_node=this->cur_node->prev;
		return *this;	
	}

	bool operator!=(iterator const& _iter)
	{
		return (this->cur_node!=_iter.cur_node);
	}

};

struct list
{
	node* head;
	node* tail;
	int size;

	list()
	{
		head=NULL;
		tail=NULL;
		size=0;
	}
	~list()
	{
		node* cur=head;
		while (cur!=tail)
		{
			node* tmp=cur;
			tmp=tmp->next;
			delete cur;
			cur=tmp;
		}
		delete tail;

	}

	list(list const& _list)
	{
		size=_list.size;
		head=new node();
		node* _node=_list.head->next;
		node* pred=head;
		node* cur;
		while (_node!=_list.head)
		{
			cur=new node(nullptr,pred,_node->value);
			pred->next=cur;
			pred=cur;
			_node=_node->next;
		}
		tail=cur;
		cur->next=head;
		head->prev=tail;
	}

	list& operator=(list const& _list)
	{
		return list(_list);
	}

	void push_back(int value)
	{
		iterator it;
		if(size==0)
		{
			head=new node(nullptr,nullptr,0);
			tail = new node(head,head,0);
			head->prev=tail;
			head->next=tail;
			it=iterator(tail);
		} 
		else 
		{
			it=end();
		}
		insert_before(it,value);	
	}

	bool pop_back()
	{
		return erase(iterator(tail->prev));
	}

	int back()const
	{
		if (size>0) return tail->prev->value; 
		return 0;	}

	void push_front(int value)
	{
		iterator it;
		if(size==0)
		{
			head=new node(nullptr,nullptr,0);
			tail = new node(head,head,0);
			head->prev=tail;
			head->next=tail;
			it=iterator(head);
		} 
		else 
		{
			it=begin();
		}
		insert_after(it,value);
	}

	bool pop_front()
	{
		return erase(iterator(head->next));
	}

	int front()const
	{
		if (size>0) return head->next->value; 
		return 0;
	}

	iterator begin()
	{
		if (size>0) return this->head->next;
		return nullptr;
	}

	iterator end()
	{
		if (size>0) return this->tail;
		return nullptr;	
	}

	bool erase(iterator iter)
	{
		if (iter.cur_node == NULL)
			return false;
		iter.cur_node->prev->next=iter.cur_node->next;
		iter.cur_node->next->prev=iter.cur_node->prev;
		iter--;
		size--;
		return true;
	}

	node* insert_after(iterator iter, int value)
	{
		if (iter.cur_node == NULL && size != 0) // некорректный указатель
			return NULL;

		node* node_new = new node(value);
		node* node_next =iter.cur_node->next;
		iter.cur_node->next = node_new;
		node_new->prev =iter.cur_node;
		node_new->next = node_next;
		node_next->prev = node_new;
		size++;
		return node_new;
	}

	node* insert_before(iterator iter, int value)
	{
		if (iter.cur_node == NULL && size != 0) // некорретный указатель
			return NULL;

		node* node_new = new node(value);
		node* node_prev = iter.cur_node->prev;
		node_prev->next = node_new;
		node_new->prev = node_prev;
		node_new->next = iter.cur_node;
		iter.cur_node->prev = node_new;
		size++;
		return node_new;
	}
};

BOOST_AUTO_TEST_CASE(test_listback)
{
	list _list;
	_list.push_back(1);
	list _list_tmp;
	_list_tmp.push_back(1);
	BOOST_CHECK_EQUAL(_list.back(),1);
	BOOST_CHECK_EQUAL(_list_tmp.back(),1);
}

BOOST_AUTO_TEST_CASE(test_listpop)
{
	list _list;
	_list.push_back(1);
	_list.push_back(2);
	_list.pop_front();
	list _list_tmp;
	_list_tmp.push_back(1);
	_list_tmp.push_back(2);
	_list_tmp.pop_back();
	BOOST_CHECK_EQUAL(_list.back(),2);
	BOOST_CHECK_EQUAL(_list_tmp.back(),1);
}

BOOST_AUTO_TEST_CASE(test_iterator_inc_dec)
{
	list _list;
	int c[5];
	for (int i=0;i<5;i++)
	{
		_list.push_back(i);
		c[i]=i;
	}
	int a[5];
	int b[5];

	int i=0;
	for (iterator it=_list.begin();it!=_list.end();it++)
		a[i++]=it.cur_node->value;

	i=4;
	iterator begin=_list.end();
	begin--;
	iterator end=_list.begin();
	end--;
	for (iterator it=begin;it!=end;it--)
		b[i--]=it.cur_node->value;
	for (int i=0;i<5;i++)
	{
		BOOST_CHECK_EQUAL(a[i],c[i]);
		BOOST_CHECK_EQUAL(c[i],b[i]);
	}
}

BOOST_AUTO_TEST_CASE(test_iterator_erase)
{
	list _list;
	int c[3];
	for (int i=0;i<6;i++){
		_list.push_back(i+1);
		c[i/2]=i+1;
	}
	int i=0;
	for (iterator it=_list.begin();it!=_list.end();it++,i++)
		if (i%2==0) _list.erase(it);
	int j=0;
	for (iterator it=_list.begin();it!=_list.end();it++,j++)
		BOOST_CHECK_EQUAL(c[j],*it);
}

BOOST_AUTO_TEST_CASE(test_list_copy)
{
	list _list;
	_list.push_back(1);
	_list.push_back(2);
	list _list_new=_list;
	_list.pop_back();
	_list_new.pop_front();
	BOOST_CHECK_EQUAL(_list.back(),1);
	BOOST_CHECK_EQUAL(_list_new.back(),2);

}



