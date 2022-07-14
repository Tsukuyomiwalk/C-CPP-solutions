#include <string>
using namespace std;
struct Phonebook
{
	string surname, name, patronymic;
	unsigned long phoneNumber;
	bool operator<(Phonebook &arg) const;
	bool operator>(Phonebook &arg) const;
	bool operator>=(Phonebook &arg) const;
	bool operator<=(Phonebook &arg) const;
	bool operator==(Phonebook &arg) const;
	//реализовал все операции на всякий случай, хоть и использоваться будут лишь 2)
};
ostream &operator<<(ostream &Str, const Phonebook &arg);
istream &operator>>(istream &Str, Phonebook &arg);
