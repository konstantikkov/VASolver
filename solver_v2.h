#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <stack>
using namespace std;

class Node {
public:
	virtual string calculate() = 0;
};
class Variable : public Node {
public:
	virtual string calculate() = 0;
	virtual string get_name() = 0;
};

class Matrix : public Variable {
public:
	//Matrix(size_t row, size_t column, vector<double> _matrix, string new_name)
	//	: column_num(column), row_num(row)
	//{
	//	name = new_name;
	//	column_num = column;
	//	row_num = row;
	//	matrix_pointer = new double* [row];
	//	int k = 0;
	//	for (size_t i = 0; i < row; i++) {
	//		matrix_pointer[i] = new double[column];
	//		for (size_t j = 0; j < column; j++) {
	//			*(*(matrix_pointer + i) + j) = _matrix[k];
	//			k++;
	//		}
	//	}
	//}


	Matrix(size_t row, size_t column)
		: column_num(column), row_num(row)
	{
		column_num = column;
		row_num = row;
		matrix_pointer = new double* [row];
		for (size_t i = 0; i < row; i++) {
			matrix_pointer[i] = new double[column];
		}
	}

	Matrix(const Matrix& temp)
		: column_num(temp.column_num), row_num(temp.row_num)
	{
		column_num = temp.column_num;
		row_num = temp.row_num;
		matrix_pointer = new double* [row_num];
		for (size_t i = 0; i < row_num; i++) {
			matrix_pointer[i] = new double[column_num];
			for (size_t j = 0; j < column_num; j++) {
				*(*(matrix_pointer + i) + j) = *(*(temp.matrix_pointer + i) + j);
			}
		}
	}


	~Matrix() {
		for (size_t i = 0; i < row_num; i++) {
			delete[] matrix_pointer[i];
		}
		delete[] matrix_pointer;
	}

	double& operator() (size_t i, size_t j) {
		return *(*(matrix_pointer + i) + j);
	}

	double& operator() (size_t i, size_t j) const {
		return *(*(matrix_pointer + i) + j);
	}

	size_t get_row() const {
		return row_num;
	}
	size_t get_column() const {
		return column_num;
	}

	string get_name() override {
		return name;
	}

	string calculate() override {
		string temp = "[ ";
		for (int i = 0; i < row_num; i++) {
			for (int j = 0; j < column_num; j++) {
				temp += to_string((*(*(this->matrix_pointer + i) + j))) + ' ';
			}
			if (row_num - 1 != i) {
				temp += "; ";
			}
		}
		return temp + ']';
	}



	Matrix operator*(const Matrix& other) {
		Matrix temp(this->row_num, other.get_column());
		for (size_t i = 0; i < this->row_num; i++) {
			for (size_t i_ = 0; i_ < other.get_column(); i_++) {
				double temp_num = 0;
				for (size_t j = 0; j < this->column_num; j++) {
					temp_num += other(j, i_) * (*(*(this->matrix_pointer + i) + j));
				}
				temp(i, i_) = temp_num;
			}
		}
		return move(temp);
	}

	//matrix operator*(Rational& other) {
	//	matrix temp(this->row_num, this->column_num);
	//	for (size_t i = 0; i < this->row_num; i++) {
	//		for (size_t j = 0; j < this->column_num; j++) {
	//			temp(i, j) = static_cast<double>(other.Numerator()) / other.Denominator() * (*(*(this->matrix_pointer + i) + j));
	//		}
	//	}
	//	return move(temp);
	//}


	Matrix operator+(const Matrix& other) {
		Matrix temp(this->row_num, this->column_num);
		for (size_t i = 0; i < this->row_num; i++) {
			for (size_t j = 0; j < this->column_num; j++) {
				temp(i, j) = other(i, j) + (*(*(this->matrix_pointer + i) + j));
			}
		}
		return move(temp);
	}

	Matrix operator-(const Matrix& other) {
		Matrix temp(this->row_num, this->column_num);
		for (size_t i = 0; i < this->row_num; i++) {
			for (size_t j = 0; j < this->column_num; j++) {
				temp(i, j) = other(i, j) - (*(*(this->matrix_pointer + i) + j));
			}
		}
		return move(temp);
	}

private:
	size_t column_num = 0, row_num = 0;
	double** matrix_pointer = nullptr;
	string name;
};

class Operator : public Node {
public:
	Operator(char s) {
		if (s == '*') {
			level = 2;
		}
		else {
			level = 1;
		}
	}

	string calculate() override {

	}

	void SetLeft(Node* lft) {
		left = lft;
	}

	void SetRight(Node* rht) {
		right = rht;
	}
	int level = 0;
private:
	char sym;
	Node* left, * right;
};

Node* parse(vector<Variable*>& var_vector, stringstream& s_str) {
	stack<Node*> values;
	stack<Operator*> ops;
	string temp;
	auto PopOps = [&](int precedence) {
		while (!ops.empty() && ops.top()->level >= precedence) {
			auto value1 = values.top();
			values.pop();
			auto value2 = values.top();
			values.pop();
			auto op = ops.top();
			ops.pop();

			op->SetRight(value1);
			op->SetLeft(value2);

			values.push(new Matrix(op->calculate()));
		}
	};
	while (s_str >> temp) {
		auto temp_it = find_if(var_vector.begin(), var_vector.end(), [&temp](Variable* temp_var) {
			return temp == temp_var->get_name();
			});
		// Переменная не существует
		if (temp_it == var_vector.end()) {
			if (temp != "[") {
				string temp_m = "[";
				while (temp != "]") {
					s_str >> temp;
					temp_m =temp_m + ' ' + temp + ' ';
				}
				values.push(new Matrix(temp_m));
			}
			else if (temp == "/") {

			}
			else if (temp == "*") {

			}
			else if (temp == "+" || temp == "-") {
			}
		}
		// Переменная существует
		else {
			return *temp_it;
		}
	}
}