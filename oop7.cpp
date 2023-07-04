// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <random>
#include <string> 
using namespace std;
template<class T>
class Node
{
protected:
	//закрытые переменные Node N; N.data = 10 вызовет ошибку
	T data;

	//не можем хранить Node, но имеем право хранить указатель
	Node* left;
	Node* right;
	Node* parent;

	//переменная, необходимая для поддержания баланса дерева
	int height;

	virtual Node<T>* Min(Node<T>* current) 
	{
		while (current->getLeft() != NULL)
			current = current->getLeft();
		return current;
	}

	virtual Node<T>* Max(Node<T>* current)
	{
		while (current->getRight() != NULL)
			current = current->getRight();
		return current;
	}
public:
	//доступные извне переменные и функции
	virtual void setData(T d) { data = d; }
	virtual T getData() { return data; }
	int getHeight() { return height; }

	virtual Node* getLeft() { return left; }
	virtual Node* getRight() { return right; }
	virtual Node* getParent() { return parent; }
	virtual bool operator< (Node<T> N) { if (data < N.data) return true; return false; }
	virtual bool operator> (Node<T> N) { if (data > N.data) return true; return false; }

	virtual void setLeft(Node* N) { left = N; }
	virtual void setRight(Node* N) { right = N; }
	virtual void setParent(Node* N) { parent = N; }

	//Конструктор. Устанавливаем стартовые значения для указателей
	Node<T>(T n)
	{
		data = n;
		left = right = parent = NULL;
		height = 1;
	}

	Node<T>()
	{
		left = NULL;
		right = NULL;
		parent = NULL;
		height = 1;
	}


	virtual void print() const
	{
		cout << "\n" << data;
	}

	virtual void setHeight(const int& h)
	{
		height = h;
	}

	template<class T> friend ostream& operator<< (ostream& stream, const Node<T>& N);

	Node* successor()
	{
		Node<T>  m = Min(right);
		if (right != NULL) return Min(right);
		Node<T> e = this;
		try {
			Node<T> e = this;
		}
		catch (...) {
			Node<T> d;
			return d;
		}
		while (e!=NULL&& e.getParent() > m) {
			e = e.getParent();
			if (e.getParent()==NULL)
				return e;
		}

		return e;
	}

	Node predecessor()
	{
		if (left != NULL)
			return Max(left);
		Node<T> e = this;
		Node<T> m = Max(left);
		while (e!= NULL && e.getParent() < m) {
			e = e.getParent();
		}
		return e;
	}

	Node* operator++() {
		return successor();
	}
	Node* operator--() {
		return predecessor();
	}
};

template<class T>
ostream& operator<< (ostream& stream, const Node<T>& N)
{
	try {
		stream << "\nNode data: " << N.data << ", height: " << N.height;
	}
	catch(...){
		return stream;
	}
	return stream;
}
template<class T>
void print(Node<T>* N) const { cout << "\n" << N->getData(); }

template<class T>
class Tree
{
protected:
	//корень - его достаточно для хранения всего дерева
	Node<T>* root;
public:
	//доступ к корневому элементу
	virtual Node<T>* getRoot() const { return root; }

	//конструктор дерева: в момент создания дерева ни одного узла нет, корень смотрит в никуда
	Tree<T>() { root = NULL; }

	//функция добавления узла в дерево
	virtual Node<T>* push_R(Node<T>* N)
	{
		return push_R(N, root);
	}

	//рекуррентная функция добавления узла. Интерфейс аналогичен (добавляется корень поддерева, 
	//куда нужно добавлять узел), но вызывает сама себя - добавление в левое или правое поддерево
	virtual Node<T>* push_R(Node<T>* N, Node<T>* Current)
	{
		//не передан добавляемый узел
		if (N == NULL) return NULL;

		if (Current == NULL) Current = root;

		//пустое дерево - добавляем в корень
		if (root == NULL)
		{
			root = N;
			return root;
		}

		if (Current->getData() > N->getData())
		{
			//идем влево
			if (Current->getLeft() != NULL) return push_R(N, Current->getLeft());
			else
			{
				Current->setLeft(N);
				N->setParent(Current);
			}
		}
		if (Current->getData() < N->getData())
		{
			//идем вправо
			if (Current->getRight() != NULL) return push_R(N, Current->getRight());
			else
			{
				Current->setRight(N);
				N->setParent(Current);
			}
		}
		return N;
	}

	//функция для добавления числа. Делаем новый узел с этими данными и вызываем нужную функцию добавления в дерево
	virtual Node<T>* push(T value)
	{
		Node<T>* N = new Node<T>(value);
		return push_R(N);
	}

	//удаление узла
	Node <T>* Remove(Node <T>* N) {
		if (N == NULL)
			return NULL;
		if (root == NULL)
			return NULL;
	
		Node<T>* point = Min();
		while (point != NULL) {
			if (point == N) break;
			point++;
		}

		Node <T>* node = point;
		Node <T>* parent = nullptr;
		Node <T>* left = nullptr;
		Node <T>* right = nullptr;
		Node <T>* rightleft = nullptr;

		if (!node->getParent())
			parent = node->getParent();
		if (!node->getLeft())
			left = node->getLeft();
		if (!node->getRight())
			right = node->getRight();
		if (!node->getRight()) {
			if (right->getLeft() != nullptr)
				rightleft = right->getLeft();
		}
		
		if (parent->getRight() == node) {
			if (parent != nullptr)
				parent->setRight(right);
			if (right != nullptr)
				right->setParent(parent);
			if (left != nullptr)
				left->setParent(right);
			if (right != nullptr)
				right->setLeft(left);
			if (rightleft != nullptr)
				this->push(rightleft->getData());
		}
		else if (parent->getLeft() == node) {
			if (parent != nullptr)
				parent->setLeft(right);
			if (right != nullptr)
				right->setParent(parent);
			if (left != nullptr)
				left->setParent(right);
			if (right != nullptr)
				right->setLeft(left);
			if (rightleft != nullptr)
				this->push(rightleft->getData());
		}
		return point;


	}

	//поиск минимума и максимума в дереве
	virtual Node<T>* Min(Node<T>* Current = NULL)
	{
		if (root == NULL)
			return NULL;

		Node<T>* current = root;

		while (current->getLeft() != NULL)
			current = current->getLeft();

		return current;
	}

	virtual Node<T>* Max(Node<T>* Current = NULL)
	{
		if (root == NULL)
			return NULL;

		Node<T>* current = root;

		while (current->getRight() != NULL)
			current = current->getRight();

		return current;
	}

	//поиск узла в дереве
	virtual Node<T>* Find(T data)
	{
		if (root == NULL) return NULL;
		return Find_R(data, root);
	}

	Node <T>* operator[](int n)
	{
		Node <T>* point = Tree <T>::Min();
		for (int i = 0; i < n; ++i) point++;
		if (point != nullptr) {
			return point;
		}
	}
	Node <T>* operator[](T key) {
		//Find_R
	}
	//поиск узла в дереве. Второй параметр - в каком поддереве искать, первый - что искать
	virtual Node<T>* Find_R(T data, Node<T>* Current)
	{
		//база рекурсии
		if (Current == NULL || Current->getData() == data) return Current;
		//рекурсивный вызов
		if (Current->getData() > data) return Find_R(data, Current->getLeft());
		if (Current->getData() < data) return Find_R(data, Current->getRight());
	}

	//три обхода дерева
	virtual void PreOrder(Node<T>* N, void (*f)(Node<T>*))
	{
		if (N != NULL)
			f(N);
		if (N != NULL && N->getLeft() != NULL)
			PreOrder(N->getLeft(), f);
		if (N != NULL && N->getRight() != NULL)
			PreOrder(N->getRight(), f);
	}

	//InOrder-обход даст отсортированную последовательность
	virtual void InOrder(Node<T>* N, void (*f)(Node<T>*))
	{
		if (N != NULL && N->getLeft() != NULL)
			InOrder(N->getLeft(), f);
		if (N != NULL)
			f(N);
		if (N != NULL && N->getRight() != NULL)
			InOrder(N->getRight(), f);
	}

	virtual void PostOrder(Node<T>* N, void (*f)(Node<T>*))
	{
		if (N != NULL && N->getLeft() != NULL)
			PostOrder(N->getLeft(), f);
		if (N != NULL && N->getRight() != NULL)
			PostOrder(N->getRight(), f);
		if (N != NULL)
			f(N);
	}
};

template<class T>
class AVL_Tree : public Tree<T>
{
protected:
	//определение разности высот двух поддеревьев
	int bfactor(Node<T>* p)
	{
		int hl = 0;
		int hr = 0;
		if (p->getLeft() != NULL)
			hl = p->getLeft()->getHeight();
		if (p->getRight() != NULL)
			hr = p->getRight()->getHeight();
		return (hr - hl);
	}

	//при добавлении узлов в них нет информации о балансе, т.к. не ясно, куда в дереве они попадут
	//после добавления узла рассчитываем его высоту (расстояние до корня) и редактируем высоты в узлах, где это
	//значение могло поменяться
	void fixHeight(Node<T>* p)
	{
		int hl = 0;
		int hr = 0;
		if (p->getLeft() != NULL)
			hl = p->getLeft()->getHeight();
		if (p->getRight() != NULL)
			hr = p->getRight()->getHeight();
		p->setHeight((hl > hr ? hl : hr) + 1);
	}

	//краеугольные камни АВЛ-деревьев - процедуры поворотов
	Node<T>* RotateRight(Node<T>* p) // правый поворот вокруг p
	{
		Node<T>* q = p->getLeft();
		p->setLeft(q->getRight());
		q->setRight(p);

		if (p == Tree<T>::root) Tree<T>::root = q;
		else
		{
			//if(p->getData()<p->getParent()->getData())
			if (p->getParent()->getLeft() == p)
				p->getParent()->setLeft(q);
			else
				p->getParent()->setRight(q);
		}
		q->setParent(p->getParent());
		p->setParent(q);
		if (p->getLeft() != NULL) p->getLeft()->setParent(p);

		fixHeight(p);
		fixHeight(q);
		return q;
	}

	Node<T>* RotateLeft(Node<T>* q) // левый поворот вокруг q
	{
		Node<T>* p = q->getRight();
		q->setRight(p->getLeft());
		p->setLeft(q);

		if (q == Tree<T>::root) Tree<T>::root = p;
		else
		{
			if (q->getParent()->getLeft() == q)
				q->getParent()->setLeft(p);
			else
				q->getParent()->setRight(p);
		}
		p->setParent(q->getParent());
		q->setParent(p);
		if (q->getRight() != NULL) q->getRight()->setParent(q);

		fixHeight(q);
		fixHeight(p);
		return p;
	}

	//балансировка поддерева узла p - вызов нужных поворотов в зависимости от показателя баланса
	Node<T>* Balance(Node<T>* p) // балансировка узла p
	{
		fixHeight(p);
		if (bfactor(p) == 2)
		{
			if (bfactor(p->getRight()) < 0) RotateRight(p->getRight());
			/*{
				p->setRight(RotateRight(p->getRight()));
				p->getRight()->setParent(p);
			}*/
			return RotateLeft(p);
		}
		if (bfactor(p) == -2)
		{
			if (bfactor(p->getLeft()) > 0) RotateLeft(p->getLeft());
			/*{
				p->setLeft(RotateLeft(p->getLeft()));
				p->getLeft()->setParent(p);
			}*/
			return RotateRight(p);
		}

		return p; // балансировка не нужна
	}

public:
	//конструктор AVL_Tree вызывает конструктор базового класса Tree
	AVL_Tree<T>() : Tree<T>() {}

	virtual Node<T>* push_R(Node<T>* N)
	{
		return push_R(N, Tree<T>::root);
	}

	//рекуррентная функция добавления узла. Устроена аналогично, но вызывает сама себя - добавление в левое или правое поддерево
	virtual Node<T>* push_R(Node<T>* N, Node<T>* Current)
	{
		//вызываем функцию push_R из базового класса
		Node<T>* pushedNode = Tree<T>::push_R(N, Current);
		//применяем к добавленному узлу балансировку
		if (Current != NULL)
			return Balance(Current);
		return pushedNode;
	}

	//функция для добавления числа. Делаем новый узел с этими данными и вызываем нужную функцию добавления в дерево
	virtual Node<T>* push(T n)
	{
		Node<T>* N = new Node<T>;
		N->setData(n);
		return push_R(N);
	}

	//удаление узла
	virtual void Remove(Node<T>* N) {

		Tree<T> ::Remove(N);
		Balance(Tree <T> ::root);
	}

};
class Airline {
public:
	string name, country, webspointe;
	int code, lines, rating;
	Airline() { name = "Arline" + to_string(1 + rand() % 100);  country = "c" + to_string(1 + rand() % 100); lines = 1 + rand() % 100; webspointe = "https://arline-" + to_string(rand() % 10) + ".com"; rating = 10 - rand() % 10; code = rand() % 1000; }
	Airline(string namet, string countryt, string webspointet, int codet, int linest, int ratingt) { name = namet; country = countryt; webspointe = webspointet; code = codet; lines = linest; rating = ratingt; }
	friend ostream& operator<<(ostream& stream, Airline t);

	bool operator==(Airline T) {
		if (T.code == code)
			return true;
		return false;

	}
	bool operator>(Airline T) {
		if (code > T.code)
			return true;
		return false;

	}
	bool operator<(Airline T) {
		if (code < T.code)
			return true;
		return false;

	}
};
ostream& operator<<(ostream& stream, Airline t) { stream << "\n Arline\n" << "name: " << t.name << "\ncountry : " << t.country<<"\ncode: "<<t.code << "\nwebspointe : " << t.webspointe << "\nlines : " << t.lines << " \nrating:" << t.rating; return stream; }


template <class T>
class Heap
{
private:
	//массив
	Node<T>* arr;
	//сколько элементов добавлено
	int len;
	//сколько памяти выделено
	int size;
public:
	//доступ к вспомогательным полям кучи и оператор
	int getCapacity() { return size; }
	int getCount() { return len; }
	Node<T>& operator[](int index)
	{
		if (index < 0 || index >= len)
			;//?             
		return arr[index];
	}
	//конструктор
	Heap<T>(int MemorySize = 100)
	{
		arr = new Node<T>[MemorySize];
		len = 0;
		size = MemorySize;
	}

	void Swap(int index1, int index2)
	{
		if (index1 <= 0 || index1 >= len)
			;
		if (index2 <= 0 || index2 >= len)
			;
		//здесь нужна защита от дурака
		Node<T> temp;
		temp = arr[index1];
		arr[index1] = arr[index2];
		arr[index2] = temp;
	}
	//скопировать данные между двумя узлами
	void Copy(Node<T>* dest, Node<T>* source)
	{
		dest->setData(source->getData());
	}
	Node<T>* GetLeftChild(int index)
	{
		if (index < 0 || index * 2 >= len)
			;
		//здесь нужна защита от дурака
		return &arr[index * 2 + 1];
	}
	Node<T>* GetRightChild(int index)
	{
		if (index < 0 || index * 2 >= len);
		//здесь нужна защита от дурака
		return &arr[index * 2 + 2];
	}
	Node<T>* GetParent(int index)
	{
		if (index <= 0 || index >= len)
			;
		//здесь нужна защита от дурака
		if (index % 2 == 0)
			return &arr[index / 2 - 1];
		return &arr[index / 2];
	}
	int GetLeftChildIndex(int index)
	{
		if (index < 0 || index * 2 >= len)
			;
		//здесь нужна защита от дурака
		return index * 2 + 1;
	}
	int GetRightChildIndex(int index)
	{
		if (index < 0 || index * 2 >= len)
			;
		//здесь нужна защита от дурака
		return index * 2 + 2;
	}
	int GetParentIndex(int index)
	{
		if (index <= 0 || index >= len)
			;
		//здесь нужна защита от дурака
		if (index % 2 == 0)
			return index / 2 - 1;
		return index / 2;
	}
	//просеять элемент вверх
	void SiftUp(int index = -1)
	{
		if (index == -1) index = len - 1;
		int parent = GetParentIndex(index);
		int index2 = GetLeftChildIndex(parent);
		if (index2 == index) index2 =
			GetRightChildIndex(parent);
		int max_index = index;
		if (index < len && index2 < len && parent >= 0)
		{
			if (arr[index] > arr[index2])
				max_index = index;
			if (arr[index] < arr[index2])
				max_index = index2;
		}
		if (parent < len && parent >= 0 &&
			arr[max_index]>arr[parent])
		{
			//нужно просеивание вверх
			Swap(parent, max_index);
			SiftUp(parent);
		}
	}

	template <class T>
	void push(T v)
	{
		Node<T>* N = new Node<T>;
		N->setData(v);
		push(N);
	}
	template <class T>
	void push(Node<T>* N)
	{
		if (len < size)
		{
			Copy(&arr[len], N);
			len++; SiftUp();
		}
	}

	void Straight(void(*f)(Node<T>*))
	{
		int i;
		for (i = 0; i < len; i++)
		{
			f(&arr[i]);
		}
	}
	//перебор элементов, аналогичный проходам бинарного
	void InOrder(void(*f)(Node<T>*), int index = 0)
	{
		if (GetLeftChildIndex(index) < len)
			PreOrder(f, GetLeftChildIndex(index));
		if (index >= 0 && index < len)
			f(&arr[index]);
		if (GetRightChildIndex(index) < len)
			PreOrder(f, GetRightChildIndex(index));
	}
	///
	Node<T>* ExtractMax()
	{

		if (len == 0)
			return NULL;

		Node<T>* res = new Node<T>;
		Copy(res, &arr[0]);
		Swap(0, len - 1);
		len--;
		Heapify();

		return res;
	}


	//поменять местами элементы arr[index1], arr[index2]




	//функции получения левого, правого дочернего элемента, родителя или их индексов в массиве






//восстановление свойств кучи после удаления или добавления элемента
	void Heapify(int index = 0)
	{
		//то же, что и SiftDown
		//нужно сравнить элементы и при необходимости произвести просеивание вниз
		int left_child_index = GetLeftChildIndex(index);
		int right_child_index = GetRightChildIndex(index);

		if (len <= left_child_index) return;
		if (right_child_index >= len && arr[index] > arr[left_child_index])
		{
			Swap(index, left_child_index);
			return;
		}
		int min_index = arr[left_child_index] < arr[right_child_index] ? left_child_index : right_child_index;
		if (arr[min_index] < arr[index])
		{
			Swap(index, min_index);
			Heapify(min_index);
		}
	}




};

int main()
{
	AVL_Tree<Airline> T;
	for (int i = 0; i < 45; i += 3)
	{
		Airline d("Name" + to_string(i), "c" + to_string(i), "https://arline-" + to_string(i * 30) + ".com", i * 100, i * 10,  i);
		T.push(d);
	}
	cout << *T[0];





	Node<Airline>* M = T.getRoot();
	while (M != NULL)
	{
		cout << *M;
		M = M->getLeft();
	}
	cout << "\n \n" << "right";
	Node<Airline>* D = T.Max();
	cout << "\n\n";
	while (D != NULL)
	{

		cout << *D;
		D--;
		
		
	}


	void (*f_ptr)(Node<Airline>*); f_ptr = print;


	cout << "\n\nHEAP\n\n";
	Heap<Airline> H;
	for (int i = 0; i < 45; i += 3)
	{
		Airline d("Name" + to_string(i), "c" + to_string(i), "https://arline-" + to_string(i * 30) + ".com", cos(i)*10, i * 10, 10 - i / 5);
		H.push(d);
	}
	cout << H.ExtractMax()->getData();
	
	char c; cin >> c;
	return 0;
}

//succe и pred без поддерева, operator++ --, [] i элемент получить через successor, remove если есть два ребенка
