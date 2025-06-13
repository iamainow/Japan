/*
Выполнил XXX XXX из МОТС-11
на Microsoft Visual Studio 2003
Программа решения японских кроссвордов.
В своей программе я принимал,что
крестик - 1,пробел - 0,неизвестное значение - 2.

Замечания:путь до файла должен быть без пробелов,т.к. при использовании gets()
почему-то возникали ошибки,и я заменил ее на cin,(строка 800).
Вы не подскажете причину этого?

Ввод с клавивтуры я не реализовал по той-же причине,
но гораздо удобнее использовать загрузку кроссворда из файла

В папке samples 7 кроссвордов
ваши маленькие кроссворды - 1.txt , 2.txt , большой - 5.txt
время решения каждого из этих кроссвордов не превышает 15 сек.
*/
#include "stdafx.h"
#include <math.h>
#include <conio.h>
#include <iostream>
using namespace std;
using namespace System;
//максимальный размер кроссворда
#define size 100
//граница сложности для METHOD_MAIN и METHOD_2
double diff;
//количество строк или высота
int quantity_line;
//количество столбцов или ширина
int quantity_column;
//сам кроссворд
int a[size][size];
//решён ли кроссворд?
bool win;
//цифры наверху (для строк)
int number_column[size][size];
//цифры слева (для столбцов)
int number_line[size][size];
//были ли изменения в текущей строке(или столбце)?
bool change;
//были ли изменения в кроссворде?
bool change_all;
bool print;
bool auto_diff;
double progress=0;
int b_c[size];
int b_l[size];
char path[100];
//сложность строк и столбцов для METHOD_MAIN
struct RATE_MAIN
{
	int i;
	double d;
	char c;
}rate_main[2*size];
//позволяет выводить русские буквы
class RUS
{
protected:
	//позволяет вывести русскую букву
	char rus_rus(char c1)
	{
		int i1=(int)c1;
		if((i1>=-64)&&(i1<=-17))
		{
			return (char)(i1+192);
		}
		if((i1>=-16)&&(i1<=-1))
		{
			return (char)(i1+240);
		}
		return c1;
	}
public:
	//выводит строчку из русских букв,в конце ENTER
	void puts_rus(char* c1)
	{
		int i1=strlen(c1);
		for(int i2=0;i2<i1;i2++)
		{
			cout << rus_rus(c1[i2]);
		}
		cout << endl;
	}
	//выводит строчку из русских букв
	void put_rus(char* c1)
	{
		int i1=strlen(c1);
		for(int i2=0;i2<i1;i2++)
		{
			cout << rus_rus(c1[i2]);
		}
	}
	void char_rus(char c1)
	{
		cout << rus_rus(c1);
	}
}rus;
//первый способ решения
class METHOD_MAIN
{
protected:
	//текущий номер строки или столбца
	int current;
	//максимальное кол-во возможных пробелов 
	int quantity_space;
	//кол-во пробелов впри данной генерации
	int space[size]; 
	//генерируемая строка
	int string[size];
	//строка с точно известными элементами
	int string_2[size];
	//является ли генерируемый интервал первым?
	bool first;
	//генерация строки,ее проверка
	void line_3()
	{
		int i1,i2,i3;
		i1=0;
		//очистка массива
		for(i2=0;i2<quantity_column;i2++)
		{
			string[i2]=0;
		}
		//построение строки на основе выбранных интервалов
		for(i2=0;i2<size;i2++)
		{
			if(number_line[current][i2]==0)
			{
				break;
			}
			//занести пробелы в массив
			for(i3=0;i3<space[i2];i3++)
			{
				string[i1]=0;
				i1++;
			}
			//занести крестики в массив
			for(i3=0;i3<number_line[current][i2];i3++)
			{
				string[i1]=1;
				i1++;
			}
		}
		//если хоть один элемент string не совпадает со значением в кроссворде,то...
		//...прекратить выполнение
		for(i2=0;i2<quantity_column;i2++)
		{
			//если значение в кроссворде известно и...
			if(a[current][i2]!=2)
			{
				//...если значение в кроссворде не совпадает со значением в массиве,то...
				if(string[i2]!=a[current][i2])
				{
					//...прекратить выполнение
					return;
				}
			}
		}
		//если генерируемая строка не первая,то... 
		if(first==false)
		{
			for(i2=0;i2<quantity_column;i2++)
			{
				//...если значения string и string_2 не совпадают,то...
				if(string[i2]!=string_2[i2])
				{
					//...значение не определено
					string_2[i2]=2;
				}
			}
		}
		//если генерируемая строка первая,то скопировать его значение в string_2
		if(first==true)
		{
			for(i2=0;i2<quantity_column;i2++)
			{
				string_2[i2]=string[i2];
			}
			//последующие строки уже не первые
			first=false;
		}
		return;
	}
	//генерирование интервала(position - по счету)для строки
	void line_2(int position)
	{
		int i1,i2,i3;
		//пока не встречен 0
		if(number_line[current][position]!=0)
		{
			//для текущего интервала генерировать все возможные его размеры
			//если этот интервал первый,то...
			if(position==0)
			{
				//...длина первого пробела может быть от 0 до quantity_space
				for(i1=0;i1<=quantity_space;i1++)
				{
					//записать выбранный размер в массив
					space[position]=i1;
					//генерировать все возможные размеры следующего интервала (position+1 -ого)
					line_2(position+1);
				}
			}
			//если этот интервал не первый,то...
			else
			{
				//...длина не первого интервала может быть от 1 до quantity_space
				for(i1=1;i1<=quantity_space;i1++)
				{
					//записать выбранный размер в массив
					space[position]=i1;
					//генерировать все возможные размеры следующего интервала (position+1 -ого)
					line_2(position+1);
				}
			}
		}
		//если дошли до 0(генерация закончена),то...
		else
		{
			//...подсчёт количества пробелов
			i2=0;
			for(i3=0;i3<(int)(quantity_column/2)+1;i3++)
			{
				i2=i2+space[i3];
			}
			//если количество пробелов меньше,
			//чем максимальное количество пробелов для этой строки,то...
			if(i2<=quantity_space)
			{
				//...запустить проверку
				line_3();
			}
		}
	}
	//генерация столбца,его проверка
	void column_3()
	{
		int i1,i2,i3;
		i1=0;
		//очистка массива
		for(i2=0;i2<quantity_line;i2++)
		{
			string[i2]=0;
		}
		//построение столбца на основе выбранных интервалов
		for(i2=0;i2<size;i2++)
		{
			if(number_column[current][i2]==0)
			{
				break;
			}
			//занести пробелы в массив
			for(i3=0;i3<space[i2];i3++)
			{
				string[i1]=0;
				i1++;
			}
			//занести крестики в массив
			for(i3=0;i3<number_column[current][i2];i3++)
			{
				string[i1]=1;
				i1++;
			}
		}
		//если хоть один элемент string не совпадает со значением в кроссворде,то...
		//...прекратить выполнение
		for(i2=0;i2<quantity_line;i2++)
		{
			//если значение в кроссворде известно и...
			if(a[i2][current]!=2)
			{
				//...если значение в кроссворде не совпадает со значением в массиве,то...
				if(string[i2]!=a[i2][current])
				{
					//...прекратить выполнение
					return;
				}
			}
		}
		//если генерируемый столбец не первый,то... 
		if(first==false)
		{
			for(i2=0;i2<quantity_line;i2++)
			{
				//...если значения string и string_2 не совпадают,то...
				if(string[i2]!=string_2[i2])
				{
					//...значение не определено
					string_2[i2]=2;
				}
			}
		}
		//если генерируемый столбец первый,то скопировать его значение в string_2
		if(first==true)
		{
			for(i2=0;i2<quantity_line;i2++)
			{
				string_2[i2]=string[i2];
			}
			//последующие столбцы уже не первые
			first=false;
		}
		return;
	}
	//генерирование интервала(position - по счету)для столбца
	void column_2(int position)
	{
		int i1,i2,i3;
		//пока не встречен 0
		if(number_column[current][position]!=0)
		{
			//для текущего интервала генерировать все возможные его размеры
			//если этот интервал первый,то...
			if(position==0)
			{
				//...длина первого пробела может быть от 0 до quantity_space
				for(i1=0;i1<=quantity_space;i1++)
				{
					//записать выбранный размер в массив
					space[position]=i1;
					//генерировать все возможные размеры следующего интервала (position+1 -ого)
					column_2(position+1);
				}
			}
			//если этот интервал не первый,то...
			else
			{
				//...длина не первого интервала может быть от 1 до quantity_space
				for(i1=1;i1<=quantity_space;i1++)
				{
					//записать выбранный размер в массив
					space[position]=i1;
					//генерировать все возможные размеры следующего интервала (position+1 -ого)
					column_2(position+1);
				}
			}
		}
		//если дошли до 0(генерация закончена),то...
		else
		{
			//...подсчёт количества пробелов
			i2=0;
			for(i3=0;i3<(int)(quantity_line/2)+1;i3++)
			{
				i2+=space[i3];
			}
			//если количество пробелов меньше,
			//чем максимальное количество пробелов для этого столбца,то...
			if(i2<=quantity_space)
			{
				//...запустить проверку
				column_3();
			}
		}
	}
public:
	//вызывается для решения какой-нибудь строчки
	void line_1(int i2)
	{
		int i1;
		//current - какая по счету строчка решается
		current=i2;
		//строчка будет первой при проверке
		first=true;
		//в массивах ничего не известно
		for(i1=0;i1<size;i1++)
		{
			string[i1]=2;
			string_2[i1]=2;
			space[i1]=0;
		}
		//вычисляется максимальное количество пробелов в строке
		quantity_space=quantity_column;
		for(i1=0;i1<(int)(quantity_column/2)+1;i1++)
		{
			quantity_space-=number_line[current][i1];
		}
		//обработать все возможные варианты строки:
		//генерировать все возможные интервалы(функция line_2),построить по ним строку и если
		//во всех вариантах этой строки какой-то элемент остаётся неизменным,то...
		line_2(0);
		for(i1=0;i1<quantity_column;i1++)
		{
			//...если элемент известен,а...
			if(string_2[i1]!=2)
			{
				//...в кроссворде это значение неизвестно,то...
				if(a[current][i1]==2)
				{
					//...занести это значение в кроссворд
					a[current][i1]=string_2[i1];
					//одно(или более)значений изменилось 
					change=true;
				}
			}
		}
	}
	//вызывается для решения какого-нибудь столбца
	void column_1(int i2)
	{
		int i1;
		//current - какой по счету столбец решается
		current=i2;
		//столбец будет первым при проверке
		first=true;
		//в массивах ничего не известно
		for(i1=0;i1<size;i1++)
		{
			string[i1]=2;
			string_2[i1]=2;
			space[i1]=0;
		}
		//вычисляется максимальное количество пробелов в столбце
		quantity_space=quantity_line;
		for(i1=0;i1<(int)(quantity_line/2)+1;i1++)
		{
			quantity_space-=number_column[current][i1];
		}
		//обработать все возможные варианты столбца
		//найти все возможные интервалы(функция column_2),построить по ним столбец и
		//если во всех вариантах этого столбеца какой-то элемент остаётся неизменным,то...
		column_2(0);
		for(i1=0;i1<quantity_line;i1++)
		{
			//...если элемент известен,а...
			if(string_2[i1]!=2)
			{
				//...в кроссворде это значение неизвестно,то...
				if(a[i1][current]==2)
				{
					//...занести это значение в кроссворд
					a[i1][current]=string_2[i1];
					//одно (или более)значение изменилось 
					change=true;
				}
			}
		}
	}
}method_main;
//второй способ решения
class METHOD_2
{
protected:
	//является ли генерируемая строка\столбец первым?
	bool first;
	//текущий номер строки или столбца
	int current;
	//генерируемая строка
	int string[size];
	//строка с точно известными элементами
	int string_2[size];
	//нужен для определения правильности кроссворда
	int number[size];
	//генерация строки с точно известными элементами
	void line_3()
	{
		int i1;
		//если генерируемая строка не первая,то... 
		if(first==false)
		{
			for(i1=0;i1<quantity_column;i1++)
			{
				//...если значения string и string_2 не совпадают,то...
				if(string[i1]!=string_2[i1])
				{
					//...значение не определено
					string_2[i1]=2;
				}
			}
		}
		//если генерируемая строка первая,то... 
		if(first==true)
		{
			for(i1=0;i1<quantity_column;i1++)
			{
				string_2[i1]=string[i1];
			}
			//последующие строки уже не первые
			first=false;
		}
	}
	//генерация элемента(i1 - по счету)для строки
	void line_2(int position)
	{
		int i2,i3;
		//если не дошли до последнего элемента
		if(position<quantity_column)
		{
			//если элемент в кроссворде известен,то...
			if(a[current][position]!=2)
			{
				//записать его в string
				string[position]=a[current][position];
				//перейти к следующему элементу
				line_2(position+1);
			}
			//если элемент неизвестен
			if(a[current][position]==2)
			{
				//записать в string крестик
				string[position]=1;
				//перейти к следующему элементу
				line_2(position+1);
				//потом записать в string пробел
				string[position]=0;
				//перейти к следующему элементу
				line_2(position+1);
			}
		}
		//если дошли до последнего элемента
		if(position==quantity_column)
		{
			//очистка массива
			for(i2=0;i2<(int)(quantity_column/2)+1;i2++)
			{
				number[i2]=0;
			}
			//подсчет групп крестиков,например если
			//string[] = "-XXX---XXXXX-X" ,то
			//number[] = { 3,5,1 };
			i3=0;
			for(i2=0;i2<quantity_column;i2++)
			{
				if(string[i2]==1)
				{
					number[i3]++;
				}
				if(i2!=0)
				{
					if(string[i2]==0 && string[i2-1]==1)
					{
						i3++;
					}
				}
			}
			//если массив number не равен массиву number_line,то выйти
			for(i2=0;i2<size;i2++)
			{
				if(number_line[current][i2]==0)
				{
					break;
				}
				if(number[i2]!=number_line[current][i2])
				{
					return;
				}
			}
			//если массив number равен массиву number_line,то запустить проверку
			line_3();
		}
	}
	//генерация столбца с точно известными элементами
	void column_3()
	{
		int i1;
		//если генерируемый столбец не первый,то...
		if(first==false)
		{
			for(i1=0;i1<quantity_line;i1++)
			{
				//...если значения string и string_2 не совпадают,то...
				if(string[i1]!=string_2[i1])
				{
					//...значение не определено
					string_2[i1]=2;
				}
			}
		}
		//если генерируемый столбец первый,то... 
		if(first==true)
		{
			for(i1=0;i1<quantity_line;i1++)
			{
				string_2[i1]=string[i1];
			}
			//последующие столбцы уже не первые
			first=false;
		}
	}
	//генерация элемента(i1 - по счету)для столбца
	void column_2(int position)
	{
		int i2,i3;
		//если не дошли до последнего элемента
		if(position<quantity_line)
		{
			//если элемент в кроссворде известен,то...
			if(a[position][current]!=2)
			{
				//записать его в string
				string[position]=a[position][current];
				//перейти к следующему элементу
				column_2(position+1);
			}
			//если элемент в кроссворде неизвестен,то...
			if(a[position][current]==2)
			{
				//записать в string крестик
				string[position]=1;
				//перейти к следующему элементу
				column_2(position+1);
				//потом записать в string пробел
				string[position]=0;
				//перейти к следующему элементу
				column_2(position+1);
			}
		}
		//если дошли до последнего элемента
		if(position==quantity_line)
		{
			//очистка массива
			for(i2=0;i2<(int)(quantity_line/2)+1;i2++)
			{
				number[i2]=0;
			}
			//подсчет групп крестиков,например если
			//string[] = "-XXX---XXXXX-X" ,то
			//number[] = { 3,5,1 };
			i3=0;
			for(i2=0;i2<quantity_line;i2++)
			{
				if(string[i2]==1)
				{
					number[i3]++;
				}
				if(i2!=0)
				{
					if(string[i2]==0 && string[i2-1]==1)
					{
						i3++;
					}
				}
			}
			//если массив number не равен массиву number_column,то выйти
			for(i2=0;i2<size;i2++)
			{
				if(number_column[current][i2]==0)
				{
					break;
				}
				if(number[i2]!=number_column[current][i2])
				{
					return;
				}
			}
			//если массив number равен массиву number_column,то запустить проверку
			column_3();
		}
	}
public:
	//сложность решения строки этим способом
	//сложность решения столбца этим способом
	//вызывается для решения какой-нибудь строчки
	void line_1(int i1)
	{
		int i2;
		//current - какая по счету строчка решается
		current=i1;
		//очистка массивов
		for(i2=0;i2<size;i2++)
		{
			string[i2]=2;
			string_2[i2]=2;
			number[i2]=0;
		}
		//string - текущая строка
		for(i2=0;i2<quantity_column;i2++)
		{
			string[i2]=a[current][i2];
		}
		//строка будет первой при проверке
		first=true;
		//обработать все возможные варианты строки:
		//каждый неизвестный элемент заменять сначала на крестик,а потом на пробел,
		//построить по ним строку,проверить ее на правильность и если
		//во всех вариантах этой строки какой-то элемент остаётся неизменным,то...
		line_2(0);
		for(i2=0;i2<quantity_column;i2++)
		{
			//...если элемент известен,а...
			if(string_2[i2]!=2)
			{
				//...в кроссворде это значение неизвестно,то...
				if(a[current][i2]==2)
				{
					a[current][i2]=string_2[i2];
					//одно(или более)значений изменилось 
					change=true;
				}
			}
		}
	}
	//вызывается для решения какого-нибудь столбца
	void column_1(int i1)
	{
		int i2;
		//current - какой по счету столбец решается
		current=i1;
		//очистка массивов
		for(i2=0;i2<size;i2++)
		{
			string[i2]=2;
			string_2[i2]=2;
			number[i2]=0;
		}
		//string - текущий столбец
		for(i2=0;i2<quantity_line;i2++)
		{
			string[i2]=a[i2][current];
		}
		//столбец будет первым при проверке
		first=true;
		//обработать все возможные варианты столбца:
		//каждый неизвестный элемент заменять сначала на крестик,а потом на пробел,
		//построить по ним столбец,проверить его на правильность и если
		//во всех вариантах этого столбца какой-то элемент остаётся неизменным,то...
		column_2(0);
		for(i2=0;i2<quantity_line;i2++)
		{
			//...если элемент известен,а...
			if(string_2[i2]!=2)
			{
				//...в кроссворде это значение неизвестно,то...
				if(a[i2][current]==2)
				{
					a[i2][current]=string_2[i2];
					//одно(или более)значений изменилось 
					change=true;
				}
			}
		}
	}
}method_2;
class METHOD_3
{
protected:
	//текущий номер строки или столбца
	int current;
	//максимальное кол-во возможных пробелов 
	int quantity_space;
	//кол-во пробелов впри данной генерации
	int space[size]; 
	//генерируемая строка
	int string[size];
	//строка с точно известными элементами
	int string_2[size];
	//является ли генерируемый интервал первым?
	bool first;
	//генерация строки,ее проверка
	void line_3()
	{
		int i1,i2,i3;
		i1=0;
		//очистка массива
		for(i2=0;i2<quantity_column;i2++)
		{
			string[i2]=0;
		}
		//построение строки на основе выбранных интервалов
		for(i2=0;i2<size;i2++)
		{
			if(number_line[current][i2]==0)
			{
				break;
			}
			//занести пробелы в массив
			for(i3=0;i3<space[i2];i3++)
			{
				string[i1]=0;
				i1++;
			}
			//занести крестики в массив
			for(i3=0;i3<number_line[current][i2];i3++)
			{
				string[i1]=1;
				i1++;
			}
		}
		//если генерируемая строка не первая,то... 
		if(first==false)
		{
			for(i2=0;i2<quantity_column;i2++)
			{
				//...если значения string и string_2 не совпадают,то...
				if(string[i2]!=string_2[i2])
				{
					//...значение не определено
					string_2[i2]=2;
				}
			}
		}
		//если генерируемая строка первая,то скопировать его значение в string_2
		if(first==true)
		{
			for(i2=0;i2<quantity_column;i2++)
			{
				string_2[i2]=string[i2];
			}
			//последующие строки уже не первые
			first=false;
		}
		return;
	}
	//генерирование интервала(position - по счету)для строки
	void line_2(int position)
	{
		int i1,i2,i3;
		//пока не встречен 0
		if(number_line[current][position]!=0)
		{
			//для текущего интервала генерировать все возможные его размеры
			//если этот интервал первый,то...
			if(position==0)
			{
				//...длина первого пробела может быть от 0 до quantity_space
				for(i1=0;i1<=quantity_space;i1++)
				{
					//записать выбранный размер в массив
					space[position]=i1;
					//генерировать все возможные размеры следующего интервала (position+1 -ого)
					line_2(position+1);
				}
			}
			//если этот интервал не первый,то...
			else
			{
				//...длина не первого интервала может быть от 1 до quantity_space
				//записать выбранный размер в массив
				space[position]=1;
				//генерировать все возможные размеры следующего интервала (position+1 -ого)
				line_2(position+1);
			}
		}
		//если дошли до 0(генерация закончена),то...
		else
		{
			//...подсчёт количества пробелов
			i2=0;
			for(i3=0;i3<(int)(quantity_column/2)+1;i3++)
			{
				i2=i2+space[i3];
			}
			//если количество пробелов меньше,
			//чем максимальное количество пробелов для этой строки,то...
			if(i2<=quantity_space)
			{
				//...запустить проверку
				line_3();
			}
		}
	}
	//генерация столбца,его проверка
	void column_3()
	{
		int i1,i2,i3;
		i1=0;
		//очистка массива
		for(i2=0;i2<quantity_line;i2++)
		{
			string[i2]=0;
		}
		//построение столбца на основе выбранных интервалов
		for(i2=0;i2<size;i2++)
		{
			if(number_column[current][i2]==0)
			{
				break;
			}
			//занести пробелы в массив
			for(i3=0;i3<space[i2];i3++)
			{
				string[i1]=0;
				i1++;
			}
			//занести крестики в массив
			for(i3=0;i3<number_column[current][i2];i3++)
			{
				string[i1]=1;
				i1++;
			}
		}
		//если генерируемый столбец не первый,то... 
		if(first==false)
		{
			for(i2=0;i2<quantity_line;i2++)
			{
				//...если значения string и string_2 не совпадают,то...
				if(string[i2]!=string_2[i2])
				{
					//...значение не определено
					string_2[i2]=2;
				}
			}
		}
		//если генерируемый столбец первый,то скопировать его значение в string_2
		if(first==true)
		{
			for(i2=0;i2<quantity_line;i2++)
			{
				string_2[i2]=string[i2];
			}
			//последующие столбцы уже не первые
			first=false;
		}
		return;
	}
	//генерирование интервала(position - по счету)для столбца
	void column_2(int position)
	{
		int i1,i2,i3;
		//пока не встречен 0
		if(number_column[current][position]!=0)
		{
			//для текущего интервала генерировать все возможные его размеры
			//если этот интервал первый,то...
			if(position==0)
			{
				//...длина первого пробела может быть от 0 до quantity_space
				for(i1=0;i1<=quantity_space;i1++)
				{
					//записать выбранный размер в массив
					space[position]=i1;
					//генерировать все возможные размеры следующего интервала (position+1 -ого)
					column_2(position+1);
				}
			}
			//если этот интервал не первый,то...
			else
			{
				//...длина не первого интервала может быть от 1 до quantity_space
				//записать выбранный размер в массив
				space[position]=1;
				//генерировать все возможные размеры следующего интервала (position+1 -ого)
				column_2(position+1);
			}
		}
		//если дошли до 0(генерация закончена),то...
		else
		{
			//...подсчёт количества пробелов
			i2=0;
			for(i3=0;i3<(int)(quantity_line/2)+1;i3++)
			{
				i2+=space[i3];
			}
			//если количество пробелов меньше,
			//чем максимальное количество пробелов для этого столбца,то...
			if(i2<=quantity_space)
			{
				//...запустить проверку
				column_3();
			}
		}
	}
public:
	//вызывается для решения какой-нибудь строчки
	void line_1(int i2)
	{
		int i1;
		//current - какая по счету строчка решается
		current=i2;
		//строчка будет первой при проверке
		first=true;
		//в массивах ничего не известно
		for(i1=0;i1<size;i1++)
		{
			string[i1]=2;
			string_2[i1]=2;
			space[i1]=0;
		}
		//вычисляется максимальное количество пробелов в строке
		quantity_space=quantity_column;
		for(i1=0;i1<(int)(quantity_column/2)+1;i1++)
		{
			quantity_space-=number_line[current][i1];
		}
		//обработать все возможные варианты строки:
		//генерировать все возможные интервалы(функция line_2),построить по ним строку и если
		//во всех вариантах этой строки какой-то элемент остаётся неизменным,то...
		line_2(0);
		for(i1=0;i1<quantity_column;i1++)
		{
			//...если элемент известен,а...
			if(string_2[i1]!=2)
			{
				//...в кроссворде это значение неизвестно,то...
				if(a[current][i1]==2)
				{
					//...занести это значение в кроссворд
					a[current][i1]=string_2[i1];
					//одно(или более)значений изменилось 
					change=true;
				}
			}
		}
	}
	//вызывается для решения какого-нибудь столбца
	void column_1(int i2)
	{
		int i1;
		//current - какой по счету столбец решается
		current=i2;
		//столбец будет первым при проверке
		first=true;
		//в массивах ничего не известно
		for(i1=0;i1<size;i1++)
		{
			string[i1]=2;
			string_2[i1]=2;
			space[i1]=0;
		}
		//вычисляется максимальное количество пробелов в столбце
		quantity_space=quantity_line;
		for(i1=0;i1<(int)(quantity_line/2)+1;i1++)
		{
			quantity_space-=number_column[current][i1];
		}
		//обработать все возможные варианты столбца
		//найти все возможные интервалы(функция column_2),построить по ним столбец и
		//если во всех вариантах этого столбеца какой-то элемент остаётся неизменным,то...
		column_2(0);
		for(i1=0;i1<quantity_line;i1++)
		{
			//...если элемент известен,а...
			if(string_2[i1]!=2)
			{
				//...в кроссворде это значение неизвестно,то...
				if(a[i1][current]==2)
				{
					//...занести это значение в кроссворд
					a[i1][current]=string_2[i1];
					//одно (или более)значение изменилось 
					change=true;
				}
			}
		}
	}
}method_3;
//ввод
class INPUT
{
protected:
	int i1,i2,i3;
	char c1;
	bool b1;
	//преобразует из char в int
	int digit(char c1)
	{
		switch(c1)
		{
		case '0':return 0;
		case '1':return 1;
		case '2':return 2;
		case '3':return 3;
		case '4':return 4;
		case '5':return 5;
		case '6':return 6;
		case '7':return 7;
		case '8':return 8;
		case '9':return 9;
		}
		return -1;
	}
public:
	//загрузка из файла
	int program()
	{
		int i1;
		FILE *f1;
		i1=strlen(path);
		if(path[i1-4]!='.'||path[i1-3]!='t'||path[i1-2]!='x'||path[i1-1]!='t')
		{
			strcat(path,".txt");
		}
		if((f1=fopen(path,"rt"))==NULL)
		{
			return 0;
		}
		//прочитать ширину кроссворда
		fscanf(f1,"%d",&quantity_column);
		//прочитать высоту кроссворда
		fscanf(f1,"%d",&quantity_line);
		//прочитать цифры слева
		for(i1=0;i1<quantity_line;i1++)
		{
			for(i2=0;i2<size;i2++)
			{
				fscanf(f1,"%d",&number_line[i1][i2]);
				fscanf(f1,"%c",&c1);
				if((int)c1==10)
				{
					break;
				}
			}
		}
		//прочитать цифры сверху
		for(i1=0;i1<quantity_column;i1++)
		{
			for(i2=0;i2<size;i2++)
			{
				fscanf(f1,"%d",&number_column[i1][i2]);
				fscanf(f1,"%c",&c1);
				if((int)c1==10)
				{
					break;
				}
			}
		}
		//все остальное,от находящееся между кавычками,вывести на экран
		fscanf(f1,"%c",&c1);
		if(c1=='"')
		{
			while(1)
			{
				fscanf(f1,"%c",&c1);
				if(c1=='"')
				{
					break;
				}
				rus.char_rus(c1);
			}
			cout << endl;
		}
		fclose(f1);
		return 1;
	}
}input;
class FUNCTION
{
public:
	double precision(double number,double preposition)
	{
		return (double)((int)(number/preposition))*preposition;
	}
	void sound()
	{
		cout << "\a";
		getch();
	}
	//установка начальных значений
	void set_default()
	{
		int i1,i2;
		//сначала в кроссворде ничего не известно
		for(i1=0;i1<size;i1++)
		{
			for(i2=0;i2<size;i2++)
			{
				//каждый элемент массива неизвестен
				a[i1][i2]=2;
			}
		}
		for(i1=0;i1<size;i1++)
		{
			//надо решать все строки и столбцы
			b_l[i1]=size;
			b_c[i1]=size;
		}
		//массивы number_line и number_column заполнены нулями
		//они пока не введены
		for(i1=0;i1<size;i1++)
		{
			for(i2=0;i2<size;i2++)
			{
				number_line[i1][i2]=0;
				number_column[i1][i2]=0;
			}
		}
	}
	//вывести весь кроссворд
	void print_all()
	{
		int i1,i2;
		for(i1=0;i1<quantity_line;i1++)
		{
			for(i2=0;i2<quantity_column;i2++)
			{
				if(a[i1][i2]==1)
				{
					cout << "X";
				}
				if(a[i1][i2]==0)
				{
					cout << " ";
				}
				if(a[i1][i2]==2)
				{
					cout << "?";
				}
			}
			cout << endl;
		}
	}
	//записывает сложность каждой строки и каждого столбца для METHOD_MAIN,
	//записывает в rate_main[] и сортирует их по возрастанию
	void build_rate_main()
	{
		int i1,i2,i3,i4;
		double d1;
		char c1;
		//записывает сложность строк
		for(i2=0;i2<quantity_line;i2++)
		{
			i3=0;
			i4=0;
			for(i1=0;i1<size;i1++)
			{
				if(number_line[i2][i1]==0)
				{
					break;
				}
				i4+=number_line[i2][i1];
				i3++;
			}
			rate_main[i2].d=i3*log10((double)(quantity_column-i3-i4+2));
			rate_main[i2].i=i2;
			rate_main[i2].c='l';
		}
		//записывает сложность столбцов
		for(i2=0;i2<quantity_column;i2++)
		{
			i3=0;
			i4=0;
			for(i1=0;i1<size;i1++)
			{
				if(number_column[i2][i1]==0)
				{
					break;
				}
				i4+=number_column[i2][i1];
				i3++;
			}
			rate_main[i2+quantity_line].d=i3*log10((double)(quantity_line-i3-i4+2));
			rate_main[i2+quantity_line].i=i2;
			rate_main[i2+quantity_line].c='c';
		}
		//массивы rate_line и rate_column записать в структуру rate_main[]
		//rate_main[].d - сложность
		//rate_main[].i - номер строки\столбца
		//rate_main[].c - строка(l) или столбец(c)
		//сортировка rate_main[]
		for(i1=0;i1<quantity_line+quantity_column;i1++)
		{
			for(i2=0;i2<quantity_line+quantity_column-1;i2++)
			{
				if(rate_main[i2].d>rate_main[i2+1].d)
				{
					d1=rate_main[i2].d;
					c1=rate_main[i2].c;
					i3=rate_main[i2].i;
					rate_main[i2].d=rate_main[i2+1].d;
					rate_main[i2].c=rate_main[i2+1].c;
					rate_main[i2].i=rate_main[i2+1].i;
					rate_main[i2+1].d=d1;
					rate_main[i2+1].c=c1;
					rate_main[i2+1].i=i3;
				}
			}
		}
	}
	//проверка кроссворда на правильность
	int begin_verification()
	{
		int i1,i2,i3,i4;
		i1=0;
		i2=0;
		//подсчёт количества введённых крестиков слева
		for(i3=0;i3<quantity_line;i3++)
		{
			for(i4=0;i4<(int)(quantity_column/2)+1;i4++)
			{
				i1+=number_line[i3][i4];
			}
		}
		//подсчёт количества введённых крестиков сверху
		for(i3=0;i3<quantity_column;i3++)
		{
			for(i4=0;i4<(int)(quantity_line/2)+1;i4++)
			{
				i2+=number_column[i3][i4];
			}
		}
		//если количество крестиков слева не совпадает с количеством крестиков сверху,то
		//допущена ошибка при вводе
		if(i1!=i2)
		{
			return 0;
		}
		return 1;
	}
	void verification()
	{
		int i1,i2,i3;
		bool b1;
		//предполагаем,что кроссворд решён
		win=true;
		//предполагаем,что строку проверять не надо
		for(i1=0;i1<quantity_line;i1++)
		{
			b_l[i1]=0;
		}
		//предполагаем,что столбец проверять не надо
		for(i2=0;i2<quantity_column;i2++)
		{
			b_c[i2]=0;
		}
		i3=0;
		for(i1=0;i1<quantity_line;i1++)
		{
			for(i2=0;i2<quantity_column;i2++)
			{
				//если значение кроссворда пока неизвестно,то эту строку и этот столбец нужно проверять
				if(a[i1][i2]==2)
				{
					//если есть хотя бы один неизвестный элемент,то строку проверять надо
					b_l[i1]++;
					//если есть хотя бы один неизвестный элемент,то столбец проверять надо
					b_c[i2]++;
					//кроссворд не решён
					win=false;
					i3++;
				}
			}
		}
		progress=1-i3/(double)(quantity_line*quantity_column);
		if(print==true)
		{
			system("CLS");
			rus.put_rus("Завершено:");
			printf("%0.0f\n",100*progress);
			cout << "diff=";
			printf("%0.2f\n",diff);
			function.print_all();
		}
		//если не было ни одного изменения и кроссворд не решен,то...
		if(change_all==false && win==false)
		{
			//...его однозначно решить нельзя
			system("CLS");
			rus.puts_rus("Кроссворд невозможно решить одним способом");
			function.print_all();
			function.sound();
			//Принять первый неизвестный элемент за крестик
			b1=false;
			for(i1=0;i1<quantity_line;i1++)
			{
				for(i2=0;i2<quantity_column;i2++)
				{
					if(a[i1][i2]==2)
					{
						a[i1][i2]=1;
						b1=true;
						break;
					}
				}
				if(b1==true)
				{
					break;
				}
			}
		}
	}
}function;
class SOLVE
{
public:
	void solve_1()
	{
		DateTime time_1,time_2;
		double a_t;
		double best_time=0.8;
		if(quantity_line*quantity_column>600)
		{
			best_time+=log10((double)(quantity_line*quantity_column)/375);
		}
		const double step=0.3;
		const double add_time=best_time;
		int i1;
		bool line;
		double all_time;
		int current;
		//запоминает текущее время
		DateTime begin_time=DateTime::Now;
		for(i1=0;i1<quantity_line;i1++)
		{
			method_3.line_1(i1);
		}
		for(i1=0;i1<quantity_column;i1++)
		{
			method_3.column_1(i1);
		}
		diff=3*best_time*best_time;
		progress=0;
		do
		{
			time_1=DateTime::Now;
			//пока что,изменений нет
			change_all=false;
			for(i1=0;i1<quantity_line+quantity_column;i1++)
			{
				//пока что,изменений в текущей строке\столбце нет
				change=false;
				//текущая строка\столбец
				current=rate_main[i1].i;
				if(rate_main[i1].c=='l')
				{
					line=true;
				}
				if(rate_main[i1].c=='c')
				{
					line=false;
				}
				//если это строка и ее проверять не надо,то перейти к следующей
				if(line==true && b_l[current]==0)
				{
					continue;
				}
				//если это столбец и его проверять не надо,то перейти к следующему
				if(line==false && b_c[current]==0)
				{
					continue;
				}
				//если это строка
				if(line==true)
				{
					//если сложность этой строки меньше diff
					if(b_l[current]<diff*3.4)
					{
						//решить эту строку вторым способом
						method_2.line_1(current);
						//если изменение в этой строке были,то и во всем кроссворде есть изменения
						if(change==true)
						{
							change_all=true;
							continue;
						}

					}
				}
				//если это столбец
				if(line==false)
				{
					//если сложность этого столбца меньше diff
					if(b_c[current]<diff*3.4)
					{
						//решить этот столбец вторым способом
						method_2.column_1(current);
						//если изменение в этом столбце были,то и во всем кроссворде есть изменения
						if(change==true)
						{
							change_all=true;
							continue;
						}
					}
				}
				//если изменения в кроссворде есть и сложность строки\столбца больше diff,
				//то перейти к след. строке\столбцу
				if(rate_main[i1].d>=diff && change_all==true)
				{
					continue;
				}
				//решать строку первым способом
				if(line==true)
				{
					method_main.line_1(current);
					if(change==true)
					{
						change_all=true;
						continue;
					}
				}
				//решать столбец первым способом
				if(line==false)
				{
					method_main.column_1(current);
					if(change==true)
					{
						change_all=true;
						continue;
					}
				}
			}
			time_2=DateTime::Now;
			if(progress<1)
			{
				a_t=3600*(time_2.Hour-time_1.Hour)+60*(time_2.Minute-time_1.Minute)+(time_2.Second-time_1.Second)+0.001*(time_2.Millisecond-time_1.Millisecond);
				a_t=function.precision(a_t,0.05);
				if(a_t>=best_time+add_time)
				{
					diff-=step;
				}
				if(a_t<=best_time-add_time)
				{
					diff+=step;
				}
				if(a_t>best_time && a_t<best_time+add_time)
				{
					diff-=step*a_t/best_time;
				}
				if(a_t<best_time && a_t>best_time-add_time)
				{
					diff+=step*a_t/best_time;
				}
				if(progress>0.75)
				{
					if(best_time<=1)
					{
						diff=4;
					}
					else
					{
						diff=4.5;
					}
					diff=4;
				}
			}
			function.verification();
		}
		//повторять пока есть хотя бы один неизвестный элемент
		while(win==false);
		//запоминает текущее время
		system("CLS");
		function.print_all();
		cout << endl;
		DateTime end_time=DateTime::Now;
		//вычисляет прошедшее время в секундах
		all_time=(end_time.Hour-begin_time.Hour)*3600+(end_time.Minute-begin_time.Minute)*60+(end_time.Second-begin_time.Second)+(end_time.Millisecond-begin_time.Millisecond)*0.001;
		rus.put_rus("Затраченное время: ");
		printf("%0.2f\n",all_time);
		function.sound();
	}
	void solve_2()
	{
		int i1;
		bool line;
		double all_time;
		int current;
		//запоминает текущее время
		DateTime begin_time=DateTime::Now;
		for(i1=0;i1<quantity_line;i1++)
		{
			method_3.line_1(i1);
		}
		for(i1=0;i1<quantity_column;i1++)
		{
			method_3.column_1(i1);
		}
		do
		{
			//пока что,изменений нет
			change_all=false;
			for(i1=0;i1<quantity_line+quantity_column;i1++)
			{
				//пока что,изменений в текущей строке\столбце нет
				change=false;
				//текущая строка\столбец
				current=rate_main[i1].i;
				if(rate_main[i1].c=='l')
				{
					line=true;
				}
				if(rate_main[i1].c=='c')
				{
					line=false;
				}
				//если это строка и ее проверять не надо,то перейти к следующей
				if(line==true && b_l[current]==0)
				{
					continue;
				}
				//если это столбец и его проверять не надо,то перейти к следующему
				if(line==false && b_c[current]==0)
				{
					continue;
				}
				//если это строка
				if(line==true)
				{
					//если сложность этой строки меньше diff
					if(b_l[current]<diff*3.4)
					{
						//решить эту строку вторым способом
						method_2.line_1(current);
						//если изменение в этой строке были,то и во всем кроссворде есть изменения
						if(change==true)
						{
							change_all=true;
							continue;
						}

					}
				}
				//если это столбец
				if(line==false)
				{
					//если сложность этого столбца меньше diff
					if(b_c[current]<diff*3.4)
					{
						//решить этот столбец вторым способом
						method_2.column_1(current);
						//если изменение в этом столбце были,то и во всем кроссворде есть изменения
						if(change==true)
						{
							change_all=true;
							continue;
						}
					}
				}
				//если изменения в кроссворде есть и сложность строки\столбца больше diff,
				//то перейти к след. строке\столбцу
				if(rate_main[i1].d>=diff && change_all==true)
				{
					continue;
				}
				//решать строку первым способом
				if(line==true)
				{
					method_main.line_1(current);
					if(change==true)
					{
						change_all=true;
						continue;
					}
				}
				//решать столбец первым способом
				if(line==false)
				{
					method_main.column_1(current);
					if(change==true)
					{
						change_all=true;
						continue;
					}
				}
			}
			function.verification();
		}
		//повторять пока есть хотя бы один неизвестный элемент
		while(win==false);
		//запоминает текущее время
		system("CLS");
		function.print_all();
		cout << endl;
		DateTime end_time=DateTime::Now;
		//вычисляет прошедшее время в секундах
		all_time=(end_time.Hour-begin_time.Hour)*3600+(end_time.Minute-begin_time.Minute)*60+(end_time.Second-begin_time.Second)+(end_time.Millisecond-begin_time.Millisecond)*0.001;
		rus.put_rus("Затраченное время: ");
		printf("%0.2f\n",all_time);
		function.sound();
	}
}solve;
void main(int argc, char *argv[])
{
	char c1;
	if(argc!=2 && argc!=3)
	{
		while(1)
		{
			//очистка экрана
			system("CLS");
			//меню
			rus.puts_rus("(1)Загрузка из файла");
			rus.puts_rus("(2)Выход");
			while(1)
			{
				c1=getch();
				if(c1=='1'|| c1=='2')
				{
					break;
				}
			}
			if(c1=='2')
			{
				return;
			}
			function.set_default();
			if(c1=='1')
			{
				//загрузка из файла
				rus.put_rus("Путь=");
				cin >> path;
				if(input.program()==0)
				{
					rus.puts_rus("Файл не найден");
					function.sound();
					continue;
				}
			}
			//проверка кроссворда
			if(function.begin_verification()==0)
			{
				rus.puts_rus("Кроссворд введeн с ошибкой");
				function.sound();
				continue;
			}
			rus.puts_rus("Использовать автоматическую настройку diff?(Y/N)");
			while(1)
			{
				c1=getch();
				if(c1=='y'||c1=='Y')
				{
					auto_diff=true;
					break;
				}
				if(c1=='n'||c1=='N')
				{
					cout << "diff=";
					cin >> diff;
					auto_diff=false;
					break;
				}
			}
			rus.puts_rus("Выводить кроссворд каждый раз?(Y/N)");
			while(1)
			{
				c1=getch();
				if(c1=='y'||c1=='Y')
				{
					print=true;
					break;
				}
				if(c1=='n'||c1=='N')
				{
					print=false;
					break;
				}
			}
			rus.puts_rus("Пожалуйста,подождите...");
			function.build_rate_main();
			if(auto_diff==true)
			{
				solve.solve_1();
			}
			if(auto_diff==false)
			{
				solve.solve_2();
			}
		}
	}
	else
	{
		function.set_default();
		strcpy(path,argv[1]);
		if(input.program()==0)
		{
			rus.puts_rus("Файл не найден");
			function.sound();
			return;
		}
		if(function.begin_verification()==0)
		{
			rus.puts_rus("Кроссворд введeн с ошибкой");
			function.sound();
			return;
		}
		function.build_rate_main();
		print=false;
		if(argc==3)
		{
			if(strcmp(argv[2],"y")==0)
			{
				print=true;
			}
		}
		solve.solve_1();
		return;
	}
}