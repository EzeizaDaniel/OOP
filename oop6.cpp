#include <iostream>
#include <fstream>
#include <string> 
using namespace std;

template<class T>
class LinkedList;

template<class T>
class Stack;

template<class T>
class DoubleLinkedStack;
class Team;

template<class T>
class Element
{
protected:
	Element* next;
	Element* prev;
	T info;
public:
	friend class LinkedList<T>;
	friend class Stack<T>;
	friend class DoubleLinkedStack<T>;
	template<class T>
	friend ostream& operator<<(ostream& s, const LinkedList<T>& el);

	template<class T1>
	friend ostream& operator<<(ostream& s, const DoubleLinkedStack<T1>& el);

	Element(T data)
	{
		next = prev = NULL;
		info = data;
	}

	Element(Element* Next, Element* Prev, T data)
	{
		next = Next;
		prev = Prev;
		info = data;
	}

	/*Element(const Element& el)
	{
		next = el.next;
		prev = el.prev;
		info = el.info;
	}*/

	template<class T1>
	friend ostream& operator<<(ostream& s, const Element<T1>& el);
	const Element<T>* getNext()
	{
		return next;
	}
	const Element<T>* getPrev() {

		return prev;
	}
	void setNext(const Element <T>* value) {
		next = value;
	}
	void  setPrev(const Element <T>* value) {
		prev = value;
	}
	T getInfo() {
		return info;
	}
};

template<class T1>
ostream& operator<<(ostream& s, const Element<T1>& el)
{ 
	s << el.info;
	return s;
}


template<class T>
class LinkedList
{
protected:
	Element<T>* head;
	Element<T>* tail;
	int count;
public:

	LinkedList()
	{
		head = tail = NULL;
		count = 0;
	}

	//удалить первый/последний элемент и вернуть удаленное значение
	virtual Element<T>* pop() = 0;
	//добавить элемент в список
	virtual Element<T>* push(const T& value) = 0;
	virtual Element<T>* find(const string& s) = 0;
	virtual Element<T>* find_r(const T& value, Element<T>* current = NULL) = 0;
	virtual void save(string fileName) {
		ofstream fout(fileName);
		Element<T>* el = head;
		fout << count;
		while (el!=NULL) {
			fout << el;
			el = el->getNext();
		}
	}
	/*virtual void read(string fileName) {
		ifstream fin;
		fin.open(fileName);

	}*/

	virtual void remove() = 0;

	friend class Stack<T>;
	//доступ по индексу
	virtual Element<T>& operator[](int index)
	{
		//if (index < 0 || index >= LinkedList<T>::count)
		//
		//	if (index < 0) throw NegativeIndex("Negative index in operator [] ", index);
		Element<T>* current = head;
		for (int i = 0; current != NULL && i < index; current = current->next, i++);
		return *current;
	}

	template<class T1>
	friend ostream& operator<<(ostream& s, LinkedList<T1>& el);

	
	virtual ~LinkedList()
	{
		cout << "\nLinkedList destructor";
		Element<T>* previous = NULL;
		for (Element<T>* current = head;
			current != NULL; previous = current, current = current->next, delete previous);
	}

	virtual void print(ostream& s)
	{
		for (Element<T>* current = head; current != NULL; current = current->next)
			s << *current << " ";
	}


};

template<class T1>
ostream& operator<<(ostream& s, const LinkedList<T1>& el)
{
	for (Element<T1>* current = el.head; current != NULL; current = current->next)
		s << *current << " ";
	return s;
}

template<class T>
class Stack : public LinkedList<T>
{
public:
	Stack<T>() : LinkedList<T>() {}
	virtual ~Stack() { cout << "\nStack destructor"; }

	//удалить первый/последний элемент и вернуть удаленное значение
	override Element<T>* pop()
	{ 
		if (LinkedList<T>::tail == NULL) return NULL;

		Element<T>* res = LinkedList<T>::tail;

		if (LinkedList<T>::head == LinkedList<T>::tail)
		{
			LinkedList<T>::head = LinkedList<T>::tail = NULL;
			LinkedList<T>::count = 0;
			return res;
		}

		Element<T>* current = LinkedList<T>::head;
		for (; current->next != LinkedList<T>::tail; current = current->next);
		LinkedList<T>::tail = current;
		LinkedList<T>::count--;
		LinkedList<T>::tail->next = NULL;
		return res; 
	}

	override void remove()
	{
		//using LinkedList<T>::tail;
		
		Element<T>* current = LinkedList<T>::tail->getPrev();
		Element<T>* res = LinkedList<T>::tail;

		LinkedList<T>::tail->prev = NULL;
		LinkedList<T>::tail = current;
		LinkedList<T>::count--;
		LinkedList<T>::tail->next = NULL;
	}

	virtual Element<T>* push(const T& value)
	{
		if (LinkedList<T>::head == NULL)
		{
			LinkedList<T>::tail = new Element<T>(value);
			LinkedList<T>::head = LinkedList<T>::tail;
		}
		else
		{
			LinkedList<T>::tail->setNext(new Element<T>(value));
			LinkedList<T>::tail = LinkedList<T>::tail->getNext();
		}
		LinkedList<T>::count++;
		return LinkedList<T>::tail;
	}


	override Element<T>* insert(const T& value, Element<T>* previous = NULL)
	{
		if (previous == NULL || LinkedList<T>::tail == NULL) return push(value);
		Element<T>* inserted = new Element<T>(value);
		inserted->next = previous->next;
		previous->next = inserted;
		LinkedList<T>::count++;
		return inserted;
	}
	virtual Element<T>* find_r(const T& value, Element<T>* current = NULL)
	{
		if (current == NULL)
			current = LinkedList<T>::head;
		if (current->getInfo() == value)
			return current;
		if (current->getNext())
			return find_r(value, current->getNext());
		else
			return NULL;
	}

	virtual Element<T>* find(const string& v) {
		Element<T>* p = LinkedList<T>::head;
		while (p != NULL)
		{
			if (p->getInfo() == v)
			{
				return p;
			}
			p = p->getNext();
		}
		return NULL;
	}
	
	virtual void filter(const int& wins, LinkedList<T>* f, const bool& is_greater = true) {
		Element<T>* p = LinkedList<T>::head;
		while (p != NULL)
		{
			if (is_greater)
				if (p->getInfo() >= wins)
					f->push(p->getInfo());
				else
					if (p->getInfo() >= wins)
						f->push(p->getInfo());
			p = p->getNext();
		}
		


	}
	virtual void filter_r(const int& wins, LinkedList<T>* f, const bool& is_greater = true) {

		Element<T>* p = LinkedList<T>::head;
		if (f == NULL) return;
		if (is_greater)
			if (p->getInfo() >= wins)
				f->push(p->getInfo());
			else
				if (p->getInfo() >= wins)
					f->push(p->getInfo());
		if (p->getNext())
			filter_r(wins, f, is_greater);




	}
	

	override Element<T>& operator[](const int& index)
	{

		Element<T>* p = LinkedList<T>::head;
		for (int i = 0; i < index; i++)
		{
			p = p->getNext();
		}
		return *p;
	}
};

template<class T>
class DoubleLinkedStack : public Stack<T>
{
public:
	using LinkedList<T>::tail;
	using LinkedList<T>::head;
	using LinkedList<T>::count;

	DoubleLinkedStack<T>() : Stack<T>() {}
	virtual ~DoubleLinkedStack() { cout << "\nDoubleLinkedStack destructor"; }

	override Element<T>* push(const T& value)
	{
		Element<T>* tail_before_push = tail;
		Stack<T>::push(value);
		tail->prev = tail_before_push;
		return tail;
	}
	override Element<T>* pop()
	{
		if (head == tail) return Stack<T>::pop();

		Element<T>* current = tail->prev;
		Element<T>* res = tail;

		tail->prev = NULL;
		tail = current;
		count--;
		tail->next = NULL;
		return res;
	}
	override void remove()
	{


		Element<T>* current = tail->prev;
		Element<T>* res = tail;

		tail->prev = NULL;
		tail = current;
		count--;
		tail->next = NULL;

	}
	virtual Element<T>* insert(const T& value, Element<T>* previous = NULL)
	{
		Element<T>* inserted = Stack<T>::insert(value, previous);
		if (inserted != NULL)
		{
			inserted->next->prev = inserted;
			inserted->prev = previous;
		}
		return inserted;
	}
	template<class T1>
	friend ostream& operator<<(ostream& s, DoubleLinkedStack<T1>& el);

	void print(ostream& s)
	{
		for (Element<T>* current = tail; current != NULL; current = current->prev)
			s << *current << " ";
	}
};

template<class T1>
ostream& operator<<(ostream& s, DoubleLinkedStack<T1>& el)
{
	Element<T1>* current = el.head;
	for (; current->getNext() != NULL; current = current->getNext()) {
		s << *current << ", ";
		
	}
	s << *current;
	s << "\n";
	return s;
}

class Team
{
private:
	string city, name;
	int wins, loses, draws, amount;
public:
	Team(const string& cityt,const string& namet,const int& winst,const int& losest,const int& drawst,const int& amountt) { city = cityt; name = namet; wins = winst; draws = drawst; amount = amountt; cout << "\nTeam constructor 1"; }
	Team() { city = "City" + std::to_string(rand() % 10); name = "Name" + to_string(rand() % 10); wins = rand() % 100; draws = rand() % 100; amount = rand() % 100; cout << "\nTeam default constructor"; }
	Team(const Team& m) { city = m.city; name = m.name; wins = m.wins; draws = m.draws; amount = m.amount; cout << "\nTeam copy constructor"; }
	~Team() { cout << "\nTeam destructor"; }
	string getCity() { return city; }
	string getName() { return name; }
	int getWins() { return wins; }
	int getLoses() { return loses; }
	int getDraws() { return draws; }
	int getAmount() { return amount; }
	bool operator==(const Team& o) {
		if (*this == o)
			return true;
		return false;
	}
	bool operator== (const string& n) {
		if (name == n)
			return true;
		return false;
	}
	bool operator >=(const int& n) {
		if (wins >= n)
			return true;
		else return false;
	}
	bool operator <=(const int& n) {
		if (wins <= n)
			return true;
		else return false;
	}
	friend ostream& operator<<(ostream& s, Team& m) { s << m.name << " " << m.city << " " << m.wins << " " << m.loses << " " << m.draws << " " << m.amount << " "; return s; }
};



int main()
{

	DoubleLinkedStack<Team>* ptr = new DoubleLinkedStack<Team>;
	for (int i = 0; i < 50; i += 10)
		ptr->push(Team());
	cout << "\n"; ptr->print(cout);
	cout << "\n\nFind: " << *ptr->find("Name7") << "\n\n";
	DoubleLinkedStack<Team>* ptr2 = new DoubleLinkedStack<Team>;
	ptr->filter(10, ptr2, true);
	cout << "\n\nFilter (>10 by wins): " <<*ptr2  << "\n\n";
	ptr->save("test.txt");
	DoubleLinkedStack<Team>* castedList = dynamic_cast<DoubleLinkedStack<Team>*>(ptr);
	cout << *castedList;
	DoubleLinkedStack<Team>*ptr3 = new DoubleLinkedStack<Team>;
	castedList->filter(10, ptr3, true);
	cout << "\n\nFilter (>10 by wins): " << *ptr3 << "\n\n";
	delete ptr;
	//delete castedList;


	return 0;
}
