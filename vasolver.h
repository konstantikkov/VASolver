#pragma once
#include <string>
#include <sstream>

namespace math_int{
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


	class Double : public Variable {
	public:


		Double(string new_name, string s) {
			name = new_name;
			value = atof(s.c_str());
		}

		Double(double new_value) {
			value = new_value;
		}

		string calculate() override {
			return to_string(value);
		}

		string get_name() override {
			return name;
		}

		Double operator + (Double& rhs) {
			return { this->value + rhs.value };
		}

		Double operator - (Double& rhs) {
			return { this->value - rhs.value };
		}

		Double operator * (Double& rhs) {
			return { this->value * rhs.value };
		}

		Double operator / (Double& rhs) {
			return { this->value / rhs.value };
		}
		double value = 0;
	private:
		string name;
	};


	class Matrix : public Variable {
	public:

		Matrix(string new_name, string s) {

		}



		string calculate() override {

		}

		string get_name() override {
			return name;
		}
	private:
		string name;
	};



	class Rational : public Variable {
	public:

		Rational(string new_name, string s) {
			stringstream str_s;
			str_s << s;

			name = new_name;
		}

		Rational(int new_numerator, int new_denominator) {
			const int gcd = GreatestCommonDivisor(new_numerator, new_denominator);
			numerator = new_numerator / gcd;
			denominator = new_denominator / gcd;
			if (denominator < 0) {
				denominator = -denominator;
				numerator = -numerator;
			}
		}


		string calculate() override {
			return to_string(numerator) + " / " + to_string(denominator);
		}
		string get_name() override {
			return name;
		}

		int Numerator() const {
			return numerator;
		}

		int Denominator() const {
			return denominator;
		}

		Rational operator + (const Rational& rhs) {
			return {
				 this->Numerator() * rhs.Denominator() + rhs.Numerator() * this->Denominator(),
				 this->Denominator() * rhs.Denominator()
			};
		}

		Rational operator - (const Rational& rhs) {
			return {
				 this->Numerator() * rhs.Denominator() - rhs.Numerator() * this->Denominator(),
				 this->Denominator() * rhs.Denominator()
			};
		}

		Rational operator * (const Rational& rhs) {
			return {
				 this->Numerator() * rhs.Numerator(),
				 this->Denominator() * rhs.Denominator()
			};
		}

		Rational operator / (const Rational& rhs) {
			return  *this * Rational(rhs.Denominator(), rhs.Numerator());
		}


		Double operator + (Double& rhs) {
			return this->numerator / this->denominator + rhs.value;
		}

		Double operator - (Double& rhs) {
			return this->numerator / this->denominator - rhs.value;
		}

		Double operator * (Double& rhs) {
			return this->numerator / this->denominator * rhs.value;
		}

		Double operator / (Double& rhs) {
			return this->numerator / this->denominator / rhs.value;
		}

	private:
		int GreatestCommonDivisor(int a, int b) {
			if (b == 0) {
				return a;
			}
			else {
				return GreatestCommonDivisor(b, a % b);
			}
		}
		string name;
		int numerator;
		int denominator;
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

		void set_left(Node* lft, int mode) {
			left = lft;
			left_id = mode;
		}

		void set_right(Node* rht, int mode) {
			right = rht;
			right_id = mode;
		}
		int level = 0;
	private:
		char sym;
		Node* left, *right;
		int left_id, right_id;
	};

	int parser_var(string temp_s, Node* temp_ptr) {

	}

	Variable* parser_expression(string expression) {

	}
}