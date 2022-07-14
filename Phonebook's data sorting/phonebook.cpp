#include "phonebook.h"
#include <fstream>
using namespace std;

ostream &operator<<(ostream &Str, Phonebook const &arg)
{
	Str << arg.surname << " " << arg.name << " " << arg.patronymic << " " << arg.phoneNumber;
	return Str;
}
istream &operator>>(istream &Str, Phonebook &arg)
{
	Str >> arg.surname >> arg.name >> arg.patronymic >> arg.phoneNumber;
	return Str;
}
bool Phonebook::operator>(Phonebook &arg) const
{
	if (this->surname != arg.surname)
	{
		return this->surname > arg.surname;
	}
	else if (this->name != arg.name)
	{
		return this->name > arg.name;
	}
	else if (this->patronymic != arg.patronymic)
	{
		return this->patronymic > arg.patronymic;
	}
	else if (this->phoneNumber != arg.phoneNumber)
	{
		return this->phoneNumber > arg.phoneNumber;
	}
	return false;
}

bool Phonebook::operator>=(Phonebook &arg) const
{
	if (this->surname != arg.surname)
	{
		return this->surname >= arg.surname;
	}
	else if (this->name != arg.name)
	{
		return this->name >= arg.name;
	}
	else if (this->patronymic != arg.patronymic)
	{
		return this->patronymic >= arg.patronymic;
	}
	else if (this->phoneNumber != arg.phoneNumber)
	{
		return this->phoneNumber >= arg.phoneNumber;
	}
	return false;
}
bool Phonebook::operator<(Phonebook &arg) const
{
	if (this->surname != arg.surname)
	{
		return this->surname < arg.surname;
	}
	else if (this->name != arg.name)
	{
		return this->name < arg.name;
	}
	else if (this->patronymic != arg.patronymic)
	{
		return this->patronymic < arg.patronymic;
	}
	else if (this->phoneNumber != arg.phoneNumber)
	{
		return this->phoneNumber < arg.phoneNumber;
	}
	return false;
}
bool Phonebook::operator<=(Phonebook &arg) const
{
	if (this->surname != arg.surname)
	{
		return this->surname <= arg.surname;
	}
	else if (this->name != arg.name)
	{
		return this->name <= arg.name;
	}
	else if (this->patronymic != arg.patronymic)
	{
		return this->patronymic <= arg.patronymic;
	}
	else if (this->phoneNumber != arg.phoneNumber)
	{
		return this->phoneNumber <= arg.phoneNumber;
	}
	return false;
}
bool Phonebook::operator==(Phonebook &arg) const
{
	return (this->surname == arg.surname && this->name == arg.name && this->patronymic == arg.patronymic &&
			this->phoneNumber == arg.phoneNumber);
}