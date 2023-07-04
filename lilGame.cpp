#include <iostream>
using namespace std;
class MyArrayParent
{
protected:
    int capacity;
    int count;
    double* ptr;
public:
    explicit MyArrayParent(int Dimension = 100)
    {
        cout << "\nMyArray constructor";
        ptr = new double[Dimension];
        capacity = Dimension;
        count = 0;
    }
    MyArrayParent(const double* arr, int len)
    {
        cout << "\nMyArray constructor";
        ptr = new double[len];
        capacity = len;
        count = len;
        for (int i = 0; i < len; i++)
        {
            ptr[i] = arr[i];
        }
    }
    ~MyArrayParent()
    {
        cout << "\nMyArray destructor";
        if (ptr != nullptr)
        {
            delete[]ptr;
            ptr = nullptr;
        }
    }
    void extend_capacity(int len)
    {
        double tmp[capacity+len];
        for(int i = 0; i < capacity; ++i){tmp[i] = ptr[i];}
        delete []ptr;
        ptr = new double[capacity+len];
        for(int i = 0; i < capacity; ++i){ptr[i] = tmp[i];}
        capacity += len;
    }
    int Capacity() const { return capacity; }
    int Size() const { return count; }
    double GetComponent(int index)
    {
        if (index >= 0 && index < count)
            return ptr[index];
        else
            cout << "out of range" << endl;
        return 0;
    }
    void SetComponent(int index, double value)
    {
        if (index >= 0 && index < capacity)
        {
            ptr[index] = value;
            if(count-1<index)
                count = index+1;
        }
        else
            cout << "out range" << endl;
    }
    virtual void push(double value)
    {
        if (count < capacity)
        {
            ptr[count] = value;
            count++;
        }
        else
            cout << "list is full" << endl;
    }
    void RemoveLastValue()
    {
        if (count > 0)
        {
            ptr[count-1] = 0.0;
            count--;
        }
        else
            cout << "list does not contain anything" << endl;
    }
    double& operator [] (int index)
    {
        return ptr[index];
    }
    MyArrayParent& operator = (MyArrayParent& V)
    {
        if(this != &V) {
            cout << "\noperator = ";
            count = V.count;
            capacity = V.capacity;
            ptr = V.ptr;
            return *this;
        }
    }

    MyArrayParent (const MyArrayParent& V)
    {
        cout << "\nCopy constructor";
        capacity = V.capacity;
        count = V.count;
        ptr = V.ptr;
    }
    void print()
    {
        cout << "\nMyArr, size: " << count << ", values: {";
        for (int i = 0; i < count; i++)
        {
            cout << ptr[i];
            if (i != count - 1)
                cout << ", ";
        }
        cout << "}";
    }

    MyArrayParent index_of()
    {
        double max = 0;
        for(int i = 0; i < count; i++)
        {
            if(ptr[i] > max) {max = ptr[i];}
        }
        double tmp[count];
        int inner = 0;
        for(int i = 0; i < count; i++)
        {
            if(ptr[i] == max) {tmp[inner] = i; ++inner;}
        }
        return {tmp, inner};
    }
};

class MyArrayChild : public MyArrayParent
{
public:
    explicit MyArrayChild (int Dimensions  = 100) : MyArrayParent (Dimensions)
    {
        cout << "\nMyArrayChild constructor";
    }

    MyArrayChild(const MyArrayParent& V) : MyArrayParent(V) {
        cout << "\nMyArrayChild copy constructor";}

    MyArrayChild (const double *arr, int len) : MyArrayParent(arr, len) { cout << "\nMyArrayChild constructor";}

    MyArrayChild(const MyArrayChild& V) : MyArrayParent(V) {
        cout << "\nMyArrayChild copy constructor";}

    ~MyArrayChild() {cout << "\nMyArrayChild destructor";}

    void remove_at(int index)
    {
        for(int i = index; i < count-1; i++ ){ptr[i] = ptr [i+1];}
        count--;
    }

    MyArrayChild index_of() {return {MyArrayParent::index_of()};}

    void insert_at(double element, int index)
    {
        MyArrayParent::SetComponent(index, element);
    }
    MyArrayChild sub_sequence (int start, int len)
    {
        double tmp[len];
        tmp [0] = ptr[start];
        int index = 1;
        for(int i = start+1; i < count && index < len; i++)
        {
            if(ptr[i] > tmp[index-1])
            {
                tmp[index] = ptr[i];
                index++;
            }
        }
        return {tmp, index};
    }

    void operator+ (double element)
    {
        MyArrayParent::push(element);
    }
};

class MySortedArray : public MyArrayChild
{
public:
    MySortedArray(const MyArrayChild& V) : MyArrayChild(V){cout << "\nMySortedArray constructor";}

    MySortedArray(const double *arr, int len) : MyArrayChild(arr, len){cout << "\nMySortedArray constructor";}

    ~MySortedArray(){cout << "\nMySortedArray destructor";}

    MySortedArray index_of ()
    {
        double tmp[count];
        int inner = 0;
        for(int i = count-1; ptr[count-1] == ptr[i]; --i)
        {
            tmp[inner] = i;
            inner++;
        }
        return {tmp, inner};
    }

    void push (double element) override {
        if (count < capacity) {
            bool checker = false;
            int inner = 0;
            while (!checker && inner < count) {
                if (ptr[inner] > element) {
                    for (int i = count; i > inner; i--) {
                        ptr[i] = ptr[i - 1];
                    }
                    ptr[inner] = element;
                    checker = true;
                } else { inner++; }
            }
            if (!checker) { ptr[count] = element; }
            count++;
        }
        else{cout << "\nlist is full";}
    }
};

int main(){
    MyArrayParent arr;
    for (int i = 0; i < 15; i++)
    {
        arr.push(i);
    }
    for (int i = 0; i < 10; i+=2)
    {
        arr.SetComponent(i,5);
    }
    for (int i = 1; i < 10; i+=2)
    {
        arr.SetComponent(i,i);
    }
    arr.print();
    MyArrayChild arr2(arr);
    arr2.insert_at(81,5);
    arr2.print();
    MyArrayChild arrr(arr.index_of());
    arrr.print();
    arr2+15;
    arr2+15;
    arr2.print();
    arr2.RemoveLastValue();
    arr2.print();

    MySortedArray list_sort (arr2.sub_sequence(0,5));
    list_sort.print();
    list_sort.extend_capacity(1);
    list_sort.push(2);
    list_sort.print();
    MySortedArray ar (list_sort.index_of());
    ar.print();
    return 0;
}