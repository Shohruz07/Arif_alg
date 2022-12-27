#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <string>
#include <fstream>
#define sym_endl '\0'
#define fname_input "input.txt "
#define fname_output "output.txt "
#define low_ 0
#define high_ 65535
using namespace std;
class Arif_alg {
  public:
  string text;
  string qwerty;
  string encode;
  static bool problem_mode ;
  int *freq;
  void read_keyboard();
  void enter_text(string enter);
  void get_file(string fname);
  void output_file(string fname);
  string get_encode();
  void encode_text();
  void bin_freq();
  void table();
  void next_symbol(int i, int *such);
  string bits(bool bit, int bits_steck);
  const int f_h = high_ / 4 + 1;
    const int half = 2 * f_h;
       const int th_h = 3 * f_h;

};
bool Arif_alg::problem_mode = false;
void Arif_alg::read_keyboard() {
  cin >> text;
  text.push_back(sym_endl);
}

void Arif_alg::enter_text(string input) {
  text = input;
  text.push_back(sym_endl);
}
void Arif_alg::table()
{
	cout << endl << text << endl;
	cout << qwerty << endl;
	for (int i = 0; i < qwerty.size(); i++)
	{
		cout << qwerty[i] << " - " << freq[i] << endl;
	}
	cout << endl;
}
void Arif_alg::get_file(string filename) {
  fstream input;
  input.open(filename, ios::in);
  while (!input.eof()) {
    string temp;
    getline(input, temp);
    if (!input.eof()) {
      temp += '\n';
    }
    text += temp;
  }
  text.push_back(sym_endl);
  input.close();
}

void Arif_alg::output_file(string filename) {
  fstream g;
  g.open(filename, ios::out);
  for (int i = 0; i < text.size() - 1; i++)
  {
    g << text[i];
  }
  g.close();
}
void Arif_alg::bin_freq()
{
	freq = new int[qwerty.size()];
	for (int i = 0; i < qwerty.size(); i++)
	{
		freq[i] = 0;
	}
	for (int j = 0; j < qwerty.size(); j++)
	{
		for (int i = 0; i < text.size(); i++)
		{
			if (qwerty[j] == text[i])
			{
				freq[j]++;
			}
		}
		if (j > 0)
		{
			freq[j] += freq[j - 1];
		}
	}
}
string Arif_alg::get_encode()
{
	return encode;
}
/* Запись битов */
string Arif_alg::bits(bool bit, int bits_steck)
{
	string temp;
	temp += to_string(bit);
	while (bits_steck > 0)
	{
		temp += to_string(!bit);
		bits_steck -= 1;
	}
	return temp;
}
/* Возвращаем номер символа в алфавите */
void Arif_alg::next_symbol(int i, int *such)
{
	/* Индекс символа из текста в алфавите */
	bool exit = false;
	for (i; i < text.size() && !exit; i++)
	{
		char temp_text = text[i];
		for (int j = 0; j < qwerty.size(); j++)
		{
			char temp_abc = qwerty[j];
			if (temp_text == temp_abc)
			{
				*such = j;
				exit = true;
				break;
			}
		}
	}
}
void Arif_alg::encode_text()
{
	int mass_size = text.length();
	unsigned short int *_low = new unsigned short int[mass_size];
	unsigned short int *_high = new unsigned short int[mass_size];
	_low[0] = low_;
	_high[0] = high_;
	int such = 1;	/* Какой элемент берём */
	int i = 0;			/* Где находимся */
	unsigned int range = 0;
	/* Del - последняя накопленная частота */
	int del = freq[qwerty.size() - 1];
	int bits_steck = 0;
	string code = "";

	while (i < text.length())
	{
		next_symbol(i, &such);
		i += 1;

		range = _high[i - 1] - _low[i - 1] + 1;
		_low[i] = _low[i - 1] + (range*freq[such - 1]) / del;
		_high[i] = _low[i - 1] + (range*freq[such]) / del - 1;
		if (problem_mode)
		{
			cout << "[" << _low[i] << "; " << _high[i] << ")" << endl;
		}

		for (;;)
		{
			if (_high[i] < half)
			{
				code += bits(0, bits_steck);
				bits_steck = 0;
			}
			else if (_low[i] >= half)
			{
				code += bits(1, bits_steck);
				bits_steck = 0;
				_low[i] -= half;
				_high[i] -= half;
			}
			else if (_low[i] >= f_h && _high[i] < th_h)
			{
				bits_steck += 1;
				_low[i] -= f_h;
				_high[i] -= f_h;
			}
			else break;
			_low[i] = 2 * _low[i];
			_high[i] = 2 * _high[i] + 1;
		}
	}
	if (problem_mode)
	{
		cout << endl << code << endl;
	}
	encode = code;
}


map < char, pair < double, double > > m;
map < char, double > sym_freq;

int main(int argc, char * argv[]) {

  Arif_alg coding;

  cout << "Open from file or keyboard: " << endl;
  cout << "1. From file__" << endl;
  cout << "2. From keyboard__" << endl;
  cout << "Choose (1 / 2 ): ";
  int choose;
  cin >> choose;
  cout << endl;

  if (choose == 2) {
    cout << "Your text to encode: ";
    coding.read_keyboard();
  } else if (choose == 1) {
    cout << "Name of file to open (0 - default): ";
    string fname_in;
    cin >> fname_in;
    if (fname_in.at(0) == '0') {
      fname_in = fname_input;
    }
    coding.get_file(fname_in);
  } else {
    cout << "Your input text is incorrect" << endl;
    exit(1);
  }

  ifstream f("input.txt", ios::out | ios::binary);
  ofstream g("encode.txt", ios::out | ios::binary);

  /*
   * считывая символы, составим алфавит
   * Каждому символу сопоставляется количество встреч в строке
   */
  char c;
  c = f.get();
  int count = 0;

  while (!f.eof()) {
    m[c].first++;
    c = f.get();
    count++;
  }

  //определим верхнюю и нижнюю границы
  double high = 1.0;
  double low = 0.0;


  for (map < char, pair < double, double > > ::iterator itr = m.begin(); itr != m.end(); ++itr) {

    itr -> second.first /= count; //вероятность
    sym_freq[itr -> first] = itr -> second.first; //связываю символ и его вероятность

    itr -> second.first = low;
    itr -> second.second = itr -> second.first + sym_freq[itr -> first];
    low = itr -> second.second;

  }

  //в файл g получим таблицу вероятнотей
  for (map < char, double > ::iterator itr = sym_freq.begin(); itr != sym_freq.end(); ++itr)
    g << itr -> first << " " << itr -> second << " ";

  g << "|" << count << " "; //отметим конец записи символов

  //encoding

  low = 0.0;
  f.clear();
  f.seekg(0);

  c = f.get();
  while (!f.eof()) {
    //считаю новые отрезки для символа
    double range = high - low;
    high = low + range * m[c].second;
    low = low + range * m[c].first;


    //новый символ
    c = f.get();

  }


  f.close();

  double d = 0; //искомое число
  int deg = 1; //степень
  double temp = 1;

  vector < bool > v;
  cout << low << " " << high << endl;
  while (d < low || d > high) {
    for (int i = 0; i < deg; i++)
      temp *= 2;
    d += (1 / temp);
    if (d > high) {
      d -= (1 / temp);
      v.push_back(0);

    } else v.push_back(1);
    deg++;
    temp = 1;

  }

  //вывод
  for (int i = 0; i < v.size(); i++)
    cout << v[i];
  char buf;
  count = 0;

  for (int n = 0; n < v.size(); n++) {
    buf = buf | v[n] << (7 - count);
    count++;
    if (count == 8) {
      count = 0;
      g << buf;
      buf = 0;
    }
  }
  if (buf) g << buf;


  g.close();

		coding.bin_freq();
		coding.encode_text();
		coding.table();

		string encode = coding.get_encode();
		coding.table();



		cout << "encode: " << endl << encode << endl << endl;
        coding.output_file(fname_output);
  return 0;
}
