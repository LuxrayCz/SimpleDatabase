#include <conio.h>
#include <iostream>
#include <algorithm>
#include <fstream>
namespace luxray {
	void print(const char * s) {
		for (; *s != 0; ++s) {
			_putch(*s);
		}
	}
	void printFixed(const char *s, const int length){
		int i = 0;
		for (; *s != 0; ++s, ++i) {
			_putch(*s);
		}
		for (; i < length; ++i) {
			_putch(' ');
		}
	}
	void printValue(const int value) {
		for (int i = 0; i < value; ++i) {
			_putch('=');
		}
		_putch('\n');
	}
	void copyStr(const char *org, char *newStr, int maxBuffSize) {
		for (int i = 0; *org != 0 && ( i + 1 < maxBuffSize); ++org, ++newStr, ++i) {
			*newStr = *org;
		}
		*newStr = 0;
	}
	void read(char* buffer, int maxSize) {
		const char* const maxSizeP = buffer + maxSize;
		for (char c = _getch(); c != 13 && (buffer + 1 < maxSizeP); c = _getch(), buffer++) {
			*buffer = c;
			_putch(c);
		}
		*buffer = 0;
	}
	int str2int(const char* c) {
		const char* p = c;

		for (; *p >= '0' && *p < '9'; ++p);
		--p;

		int val = 0;
		int mul = 1;

		for (; c <= p; --p) {
			val += (*p - '0') *mul;
			mul *= 10;
		}
		return val;
	}
	void WriteInt(int n) {
		int nArr[10];
		for (int i =0; n > 0; i++) {
			nArr[i] = n % 10;
			n = n / 10;
		}
		for (int i = 10; i >= 0; --i) {
			if (nArr[i] > 0) {
				_putch(nArr[i] + '0');
			}
		}
	}
	void reverse(char* leftP) {
		char* rightP = leftP;
		for (; *rightP != 0; rightP++);
		rightP--;
		for (; leftP < rightP; leftP++, rightP--) {
			const char temp = *leftP;
			*leftP = *rightP;
			*rightP = temp;
		
		}
	}
	void int2str(int val, char* buff, int length) {
		char* localBuff = buff;
		for (; val > 0; val /=  10, localBuff++) {
			*localBuff = '0' + val % 10;
		
		}
		*localBuff = 0;
		reverse(buff);
	}



	int nFibonaci(int n) {

		if (n <= 1) {
			return n;
		}
		else {
			return nFibonaci(n-1) + nFibonaci(n-2) ;
		}
	}
}

class Database {
private:
	class Entry {
	public:
		Entry() = default;
		Entry(const char* name, int value) :
			value(value)
		{
			luxray::copyStr(name, this->name, sizeof(this->name));
		};
		void Print() const {
			luxray::printFixed(name, nameSize-1);
			luxray::print(" | ");
			luxray::printValue(value);
		}
		void Serialize(std::ofstream& out)const {
			out.write(name, sizeof(name));
			out.write(reinterpret_cast<const char*>(&value), sizeof(value));
		}
		void Deserialize(std::ifstream& in) {
			in.read(name, sizeof(name));
			in.read(reinterpret_cast<char*>(&value), sizeof(value));
		}
	private:
		static constexpr int nameSize = 16;
		char name[nameSize];
		int value;
	};

public:
	void Add(const char* name, int value) {
		entries[currEntry++] = { name, value };
	};
	void Print() const {
		for (int i = 0; i < currEntry; ++i) {
			entries[i].Print();
		}
	};
	void Save(const char* filename)const {
		std::ofstream out(filename);
		out.write(reinterpret_cast<const char*>(&currEntry), sizeof(currEntry));
		for (int i = 0; i < currEntry; ++i) {
			entries[i].Serialize(out);
		}
	};
	void Load(const char* filename) {
		std::ifstream in(filename);
		in.read(reinterpret_cast<char*>(&currEntry), sizeof(currEntry));
		for (int i = 0; i < currEntry; ++i) {
			entries[i].Deserialize(in);
		}
	};

private:
	static constexpr int maxEntries = 10;
	Entry entries[maxEntries];
	int currEntry = 0;
};

int main() {
	Database db;

	char nameBuffer[255];
	char valueBuffer[255];
	bool quiting = false;
	while (!quiting){
		luxray::print("(l)oad (s)ave (a)dd (q)uit or (p)print?\n");
		switch (_getch())
		{
		case 'l':
			luxray::print("Enter filename: ");
			luxray::read(nameBuffer, sizeof(nameBuffer));
			db.Load(nameBuffer);
			luxray::print("\nLoaded\n");
			break;
		case 's':
			char name[20];
			luxray::print("Enter filename: ");
			luxray::read(nameBuffer, sizeof(nameBuffer));
			db.Save(nameBuffer);
			luxray::print("\nSaved\n");
			break;
		case 'a':
			luxray::print("Enter name: ");
			luxray::read(nameBuffer, sizeof(nameBuffer));
			luxray::print("\nEnter value: ");
			luxray::read(valueBuffer, sizeof(valueBuffer));
			db.Add(nameBuffer, luxray::str2int(valueBuffer));
			luxray::print("\n ");

			break;
		case 'q':
			quiting = true;
			break;
		case 'p':
			db.Print();
			break;
		}
	}


	while (!_kbhit());
	return 0;
}