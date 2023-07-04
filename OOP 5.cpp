#include <iostream>
#include <cstring>
#include <fstream>
#include <random>
#include <typeinfo>

using namespace std;

class Exception : public exception
{
protected:
	char* str;
public:

	Exception(const char* str_e)
	{
		while (true)
		{
			str = new char[strlen(str_e) + 1];
			if (str != 0) break;
		}

		for (int i = 0; i < strlen(str_e); i++)
			str[i] = str_e[i];
		str[strlen(str_e)] = '\0';
	}

	Exception(const Exception& e)
	{
		str = new char[strlen(e.str) + 1];
		if (str != nullptr)
		{
		for (int i = 0; i < strlen(e.str); i++)
			str[i] = e.str[i];
		str[strlen(e.str)] = '\0';
		}
	}
	
	~Exception()
	{
		delete[] str;
	}

	virtual void print() = 0;
};



class IndexOutOfBounds : public Exception
{
private:
	int indexh;
	int indexw;
	int sizeh;
	int sizew;
public:

	IndexOutOfBounds(int index_h, int index_w, int size_h, int size_w) : Exception("Attempt to go out of bounds")
	{
		indexh = index_h;
		indexw = index_w;
		sizeh = size_h;
		sizew = size_w;
	}

	void print() const
	{
		cout << "Exception: " << str << '\n' << "Index: (" << indexh << "," << indexw << "), valid indexes : (0-" << sizeh << ",0-" << sizew << ") \n";

	}

};

class WrongDimensions : public Exception
{
private:
	int size_h1;
	int size_h2;
	int size_w1;
	int size_w2;

public:

	WrongDimensions(int sizeh1, int sizew1, int sizeh2, int sizew2) :Exception("Wrong dimensions of matrix")
	{
		size_h1 = sizeh1;
		size_h2 = sizeh2;
		size_w1 = sizew1;
		size_w2 = sizew2;
	}

	WrongDimensions(const char* message) :Exception(message)
	{
		;
	}

	virtual void print()
	{
		cout << "Exception: " << str << "  " << size_h1 << "X" << size_w1 << " and " << size_h2 << "X" << size_w2 << '\n';
	}

	~WrongDimensions()
	{
		;
	}
};

class WrongSize : public WrongDimensions
{
private:
	int size_h;
	int size_w;
public:

	WrongSize(int sizeh, int sizew) : WrongDimensions("Wrong size of matrix")
	{
		size_h = sizeh;
		size_w = sizew;
	}

	void print() const
	{
		cout << "Exception: " << str << "   you try to make (" << size_h << ") X (" << size_w << ") matrix\n";
	}

	~WrongSize()
	{
		;
	}
};


class bad_allocs :public Exception
{
public:

	bad_allocs() :Exception("Memory was not allocated")
	{
		;
	}

	void print() const
	{
		cout << "Exception: " << str << '\n';
	}
};


template<class T>
class MyMatrix
{
private:
	int width;		
	int height;		
	T** arr;		

public:
	template<class T>
	friend ostream& operator << (ostream& out, const MyMatrix<T>& mymatrix);

	template<class T>
	friend istream& operator>>(istream& in, MyMatrix<T>& mymatrix);


	MyMatrix(ifstream& in)
	{
		in >> height;
		in >> width;

		arr = new T * [height];
		if (arr == 0)
			throw bad_allocs();

		for (int i = 0; i < height; i++)
		{
			arr[i] = new T[width];
			if (arr[i] == 0)
				throw bad_allocs();

			for (int g = 0; g < width; g++)
				in >> arr[i][g];
		}

	}

	MyMatrix& readf(ifstream& in)
	{
		in >> height;
		in >> width;

		arr = new T * [height];
		if (arr == 0)
			throw bad_allocs();

		for (int i = 0; i < height; i++)
		{
			arr[i] = new T[width];
			if (arr[i] == 0)
				throw bad_allocs();

			for (int g = 0; g < width; g++)
				in >> arr[i][g];
		}

		return *this;
	}

	MyMatrix(int width_s = 1, int height_s = 1)
	{
		if (height_s < 0 || width_s < 0)
			throw WrongSize(height_s, width_s);

		width = width_s;
		height = height_s;

		arr = new T * [height];
		if (arr == 0)					
			throw bad_allocs();

		for (int i = 0; i < height; i++)
		{
			arr[i] = new T[width];
			if (arr[i] == 0)
				throw bad_allocs();
		}
	}

	
	MyMatrix(const MyMatrix<T>& M)
	{
		height = M.height;
		width = M.width;

		arr = new T * [height];
		if (arr == 0)
			throw bad_allocs();


		for (int i = 0; i < height; i++)
		{
			arr[i] = new T[width];
			if (arr[i] == 0)
				throw bad_allocs();

			for (int g = 0; g < width; g++)
				arr[i][g] = M.arr[i][g];
		}
	}


	MyMatrix<T>& operator=(const MyMatrix<T>& mymatrix)
	{
		if (&mymatrix == this)
			return *this;

		height = mymatrix.height;
		width = mymatrix.width;

		arr = new T * [height];
		if (arr == 0)
			throw bad_allocs();

		for (int i = 0; i < height; i++)
		{
			arr[i] = new T[width];
			if (arr[i] == 0)
				throw bad_allocs();

			for (int g = 0; g < width; g++)
				arr[i][g] = mymatrix.arr[i][g];
		}

		return *this;
	}


	T& operator()(const int& h, const int& w)
	{
		if (h < 0 || w < 0 || h >= height || w >= width)	
			throw IndexOutOfBounds(h, w, height, width);						
		return arr[h][w];
	}


	MyMatrix<T>& solve(T(*f)(T))
	{
		for (int i = 0; i < height; i++)
		{
			for (int g = 0; g < width; g++)
			{
				arr[i][g] = f(arr[i][g]);
			}
		}
		return *this;
	}


	MyMatrix<T> operator+(const MyMatrix<T>& matrix)
	{
		if (height != matrix.height || width != matrix.width)						
			throw WrongDimensions(height, width, matrix.height, matrix.width);			


		MyMatrix<T> ans{ height,width };

		for (int i = 0; i < height; i++)
		{
			for (int g = 0; g < width; g++)
			{
				ans.arr[i][g] = arr[i][g] + matrix.arr[i][g];
			}
		}

		return ans;
	}

	void fill()
	{
		for (int i = 0; i < height; i++)
		{
			for (int g = 0; g < width; g++)
			{
				arr[i][g] = static_cast<T>(rand() % 19);
			}
		}
	}

	void print()
	{
		for (int i = 0; i < height; i++)
		{
			for (int g = 0; g < width; g++)
			{
				cout << arr[i][g] << ' ';
			}
			cout << '\n';
		}
		cout << '\n';
	}

	~MyMatrix()
	{
		for (int i = 0; i < height; i++)
			delete[] arr[i];

		delete[] arr;
	}

};

template<class T>
ostream& operator << (ostream& out, const MyMatrix<T>& mymatrix)
{
	if (typeid(out).name() == typeid(ofstream).name())
	{
		out << mymatrix.height << ' ' << mymatrix.width << '\n';

		for (int i = 0; i < mymatrix.height; i++)
		{
			for (int g = 0; g < mymatrix.width; g++)
			{
				out << mymatrix.arr[i][g] << '\n';
			}
		}
	}
	else   
	{
		for (int i = 0; i < mymatrix.height; i++)
		{
			for (int g = 0; g < mymatrix.width; g++)
			{
				out << mymatrix.arr[i][g] << ' ';
			}
			out << '\n';
		}
		out << '\n';
	}
	return out;
}

template<class T>
istream& operator>>(istream& in, MyMatrix<T>& mymatrix)
{
	if (typeid(in).name() == typeid(ifstream).name())
		in >> mymatrix.height >> mymatrix.width;

	for (int i = 0; i < mymatrix.height; i++)
	{
		for (int g = 0; g < mymatrix.width; g++)
		{
			in >> mymatrix.arr[i][g];
		}
	}
	return in;
}

template<class T>
T scal_fun(T x)
{
	return pow(x, 2) + 1;
}


int main()
{
	MyMatrix<double> z{ 3,3 };
	MyMatrix<double> z1{ 3,3 };

	for (int i = 0; i < 3; i++)
	{
		for (int g = 0; g < 3; g++)
		{
			z(i, g) = rand() % 19;
		}
	}
	
	ofstream fout;
	fout.open("test.txt", ios::out);
	if (fout)	
	{
		fout << z;
		fout.close();
	}

	ifstream fin("test.txt");
	if (fin)		
	{
		try
		{
			MyMatrix<double> matr2{ 3,3 };
			matr2.readf(fin);
			MyMatrix<double> matr{ fin };
			fin >> z1;
			fin.close();			
			cout << "\n" << matr2;
		}
		catch (...)
		{
			cout << "\nException: failed to read file";
		}
	}

	MyMatrix<double> matrix;
	matrix = z;
	matrix.print();
	matrix.solve(scal_fun);
	matrix.print();


	try
	{
		MyMatrix<double> wrong_matrix{ -2,0 };

		wrong_matrix + matrix;
	}
	catch (WrongSize ws)
	{
		ws.print();
	}
	catch (IndexOutOfBounds ioob)
	{
		ioob.print();
	}
	catch (WrongDimensions wd)
	{
		wd.print();
	}
	catch (bad_allocs ba)
	{
		ba.print();
	}
	catch (...)
	{
		cout << "\nSomething has happened";
	}


	try
	{
		MyMatrix<double> matrix5{ 5,5 };
		MyMatrix<double> matrix6{ 6,6 };
		matrix5 + matrix6;
	}
	catch (WrongSize ws)
	{
		ws.print();
	}
	catch (IndexOutOfBounds ioob)
	{
		ioob.print();
	}
	catch (WrongDimensions wd)
	{
		wd.print();
	}
	catch (bad_allocs ba)
	{
		ba.print();
	}
	catch (...)
	{
		cout << "\nSomething has happened";
	}

	try
	{
		MyMatrix<double> matrix4{ 5,5 };
		matrix4(0, 0) = 5.5;
		cout << matrix4(0, 0) << '\n';
		cout << matrix4(100, 0);

	}
	catch (WrongSize ws)
	{
		ws.print();
	}
	catch (IndexOutOfBounds ioob)
	{
		ioob.print();
	}
	catch (WrongDimensions wd)
	{
		wd.print();
	}
	catch (bad_allocs ba)
	{
		ba.print();
	}
	catch (...)
	{
		cout << "\nSomething has happened";
	}


	return 0;
}
