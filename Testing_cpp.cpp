#include <iostream>
#include <string>
#include <optional>
#include <tuple>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include "gtest.h"

using namespace std;

struct Person {
	string lastName;
	string name;
	optional<string> patronymic;

	Person() {}

	Person(string inLastName, string inName, optional<string> inPatronymic) :
		lastName(inLastName), name(inName), patronymic(inPatronymic) {}

	bool operator<(const Person& person2) {
		return (tie(lastName, name, patronymic) < tie(person2.lastName, person2.name, person2.patronymic));
	}
	bool operator==(const Person& person2) {
		return (tie(lastName, name, patronymic) == tie(person2.lastName, person2.name, person2.patronymic));
	}
};

ostream& operator<<(ostream& out, const Person& person) {
	out << left << setw(15) << person.lastName << setw(10) << person.name;
	out << setw(15);
	if (person.patronymic.has_value())
		out << person.patronymic.value();
	else
		out << " ";

	return out;
}

struct PhoneNumber {
	int codeCountry;
	int codeCity;
	string number;
	optional<int> addNumber;

	PhoneNumber() {}

	PhoneNumber(int inCodeCountry, int inCodeCity, string inNumber, optional<int> inAddNumber) :
		codeCountry(inCodeCountry), codeCity(inCodeCity), number(inNumber), addNumber(inAddNumber) {}

	bool operator<(const PhoneNumber& phoneNumber2) {
		return (tie(codeCountry, codeCity, number, addNumber) < tie(phoneNumber2.codeCountry, phoneNumber2.codeCity, phoneNumber2.number, phoneNumber2.addNumber));
	}
};

ostream& operator<<(ostream& out, const PhoneNumber& phoneNumber) {
	out << "+" << phoneNumber.codeCountry << "(" << phoneNumber.codeCity << ")" << phoneNumber.number;
	if (phoneNumber.addNumber.has_value())
	{
		out << " " << phoneNumber.addNumber.value();
	}
	return out;
}

class PhoneBook {
private:
	vector<pair<Person, PhoneNumber>> book;
public:
	PhoneBook(ifstream& inf) {
		string str;
		string temp;

		Person person;
		PhoneNumber phoneNumber;

		while (inf.is_open() && !inf.eof()) {
			getline(inf, str);

			person.lastName = str.substr(0, str.find(' '));
			str.erase(0, person.lastName.length() + 1);

			person.name = str.substr(0, str.find(' '));
			str.erase(0, person.name.length() + 1);

			temp = str.substr(0, str.find(' '));
			str.erase(0, temp.length() + 1);

			person.patronymic = nullopt;

			if (temp.find("+") == string::npos)
			{
				person.patronymic = temp;

				temp = str.substr(0, str.find(' '));
				str.erase(0, temp.length() + 1);
			}

			phoneNumber.codeCountry = stoi(temp.substr(1, 1));
			phoneNumber.codeCity = stoi(temp.substr(3, 5));
			phoneNumber.number = temp.substr(7, 13);
			phoneNumber.addNumber = nullopt;

			if (!str.empty())
			{
				phoneNumber.addNumber = stoi(str);
			}

			book.push_back(make_pair(person, phoneNumber));

		}

	}

	void SortByName() {
		sort(book.begin(), book.end(), [](auto& x, auto& y) {return x.first < y.first; });
	}

	void SortByPhone() {
		sort(book.begin(), book.end(), [](auto& x, auto& y) {return x.second < y.second; });
	}

	tuple<std::string, PhoneNumber> GetPhoneNumber(std::string lastName) {
		vector<pair<Person, PhoneNumber>>::iterator it;
		tuple<string, PhoneNumber> temp_tuple;

		int sum = 0;
		PhoneNumber phoneNumber;

		for (it = book.begin(); it != book.end(); it++)
		{
			if (it->first.lastName == lastName)
			{
				sum++;
				phoneNumber = it->second;
			}
		}

		switch (sum) {
		case 0:
			temp_tuple = make_tuple("not found", phoneNumber);
			break;
		case 1:
			temp_tuple = make_tuple("", phoneNumber);
			break;
		default:
			temp_tuple = make_tuple("found more than 1", phoneNumber);
			break;
		}

		return temp_tuple;
	}

	void ChangePhoneNumber(Person person, PhoneNumber newPhoneNumber) {
		vector<pair<Person, PhoneNumber>>::iterator it;
		for (it = book.begin(); it != book.end(); it++)
		{
			if (it->first == person)
			{
				it->second = newPhoneNumber;
			}
		}
	}

	friend ostream& operator<<(ostream& out, PhoneBook& phoneBook);
};


ostream& operator<<(ostream& out, PhoneBook& phoneBook) {
	vector<pair<Person, PhoneNumber>>::iterator it;

	for (it = phoneBook.book.begin(); it != phoneBook.book.end(); it++)
	{
		out << it->first << " " << it->second << std::endl;
	}

	return out;
}



int main()
{
	ifstream file("E:\phone\PhoneBook.txt"); // путь к файлу PhoneBook.txt
	PhoneBook book(file);
	cout << book;
	cout << "------SortByPhone-------" << endl;
	book.SortByPhone();
	cout << book;
	cout << "------SortByName--------" << endl;
	book.SortByName();
	cout << book;
	cout << "-----GetPhoneNumber-----" << endl;
	// лямбда функция, которая принимает фамилию и выводит номер телефона этого    	человека, либо строку с ошибкой
	auto print_phone_number = [&book](const string& surname) {
		cout << surname << "\t";
		auto answer = book.GetPhoneNumber(surname);
		if (get<0>(answer).empty())
			cout << get<1>(answer);
		else
			cout << get<0>(answer);
		cout << endl;
	};
	// вызовы лямбды
	print_phone_number("Ivanov");
	print_phone_number("Petrov");

	cout << "----ChangePhoneNumber----" << endl;
	book.ChangePhoneNumber(Person{ "Kotov", "Vasilii", "Eliseevich" }, PhoneNumber{ 7, 123, "15344458", nullopt });
	book.ChangePhoneNumber(Person{ "Mironova", "Margarita", "Vladimirovna" }, PhoneNumber{ 16, 465, "9155448", 13 });
	cout << book;
}