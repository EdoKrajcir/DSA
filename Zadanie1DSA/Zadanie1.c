// zadanie1.c -- Eduard Krajèír, 27.9.2018 10:13

#include <stdio.h>
#include <string.h>
#define MIN_ZAPIS 8


//Hlavicka na zaciatku adresneho priestoru - udava celkovu velkost a smernik na prvy volny blok
typedef struct main_hlavicka
{
	int velkost;
	struct volna_hlavicka *zaciatok;
}MAIN_HLAVICKA;


//hlavicka volneho bloku - udava velkost volnej pamate v bloku a smernik na dalsi volny blok
typedef struct volna_hlavicka
{
	int velkost;
	struct volna_hlavicka *next;
}VOLNA_HLAVICKA;


//hlavicka plneho bloku - udava velkost zaplnenej pamate (v zapornom tvare)
typedef struct plna_hlavicka
{
	int velkost;
}PLNA_HLAVICKA;

char *Start;




void *memory_alloc(unsigned int size)
{
	MAIN_HLAVICKA *hlavna_hlavicka;
	VOLNA_HLAVICKA *pom, *prev, *next;
	PLNA_HLAVICKA *plny;
	
	hlavna_hlavicka = (char*)Start;
	pom = (char*)hlavna_hlavicka->zaciatok;
	prev = (char*)hlavna_hlavicka;
	while (pom != NULL)
	{
		next = pom->next;

		//1 - zapisujem rovnako velke;
		if (pom->velkost == sizeof(PLNA_HLAVICKA) * 2 + size)
		{
			plny = (char*)pom;
			prev->next = (char*)next;
			plny->velkost = -size;
			plny = plny + 2;
			return plny;
		}

		//2 - zapisujem mensie ale nie o dost mensie;
		if ((pom->velkost > sizeof(PLNA_HLAVICKA) * 2 + size) && (pom->velkost < sizeof(PLNA_HLAVICKA) * 2 + size + sizeof(VOLNA_HLAVICKA) + MIN_ZAPIS))
		{
			plny = (char*)pom;
			prev->next = (char*)next;
			plny->velkost = -(plny->velkost) -8;
			plny = plny + 2;
			return plny;
		}

		//3 - zapisujem mensie - sekam z volneho bloku;
		if (pom->velkost >= sizeof(PLNA_HLAVICKA) * 2 + size + sizeof(VOLNA_HLAVICKA) + MIN_ZAPIS)
		{
			plny = (char*)pom;
			pom = (char*)pom + sizeof(PLNA_HLAVICKA) * 2 + size;
			pom->velkost = plny->velkost - sizeof(PLNA_HLAVICKA) * 2 - size;
			prev->next = (char*)pom;
			pom->next = next;
			plny->velkost = -size;
			plny = plny + 2;
			return plny;
		}

		prev = (char*)pom;
		pom = (char*)next;
	}
	return NULL;
}

int memory_free(void *valid_ptr)
{
	MAIN_HLAVICKA *hlavna_hlavicka;
	VOLNA_HLAVICKA *prev, *next, *pom, *pomvol;
	PLNA_HLAVICKA *plny;
	plny = (char*)valid_ptr - 8;							//uvolnujem pamat
	hlavna_hlavicka = (char*)Start;
	prev = (char*)hlavna_hlavicka;							
	next = (char*)prev->next;
	if (valid_ptr == NULL) return 1;
	if (next != NULL)
		while (((char*)next < (char*)plny) && ((char*)next != NULL))			//najdem predchadzajuci a nasledujuci volny blok
		{
			prev = (char*)next;
			next = (char*)next->next;
		}
	pom = (char*)plny;
	pom->next = (char*)next;									//presmerujem smerniky volnych blokov
	prev->next = (char*)pom;
	pom->velkost = -(plny->velkost) - 8;

	//zlucovanie
	pomvol = (char*)hlavna_hlavicka->zaciatok;
	while (pomvol->next != NULL)
	{
		if ((char*)pomvol->next == (char*)pomvol + pomvol->velkost + sizeof(VOLNA_HLAVICKA))
		{
			pomvol->velkost = pomvol->velkost + sizeof(VOLNA_HLAVICKA) + pomvol->next->velkost;
			pomvol->next = (char*)pomvol->next->next;
		}
		else pomvol = (char*)pomvol->next;
	}
	return 0;
}


int memory_check(void *ptr)
{
	PLNA_HLAVICKA *pompln;
	pompln = ptr;
	if (pompln->velkost < 0) return 1;
	else return 0;
}



void memory_init(void *ptr, unsigned int size)
{
	MAIN_HLAVICKA *hlavna_hlavicka;
	VOLNA_HLAVICKA *volna_hlavicka;
	Start = ptr;
	hlavna_hlavicka = (char*)Start;
	hlavna_hlavicka->velkost = size;
	hlavna_hlavicka->zaciatok = NULL;
	volna_hlavicka = (char*)hlavna_hlavicka + sizeof(MAIN_HLAVICKA);
	volna_hlavicka->velkost = size - sizeof(MAIN_HLAVICKA) - sizeof(VOLNA_HLAVICKA);
	volna_hlavicka->next = NULL;
	hlavna_hlavicka->zaciatok = (char*)volna_hlavicka;
}


void test1()
{
	char region[100];
	memory_init(region, 100);
	char* pointer1 = (char*)memory_alloc(8);
	char* pointer2 = (char*)memory_alloc(8);
	char* pointer3 = (char*)memory_alloc(8);
	char* pointer4 = (char*)memory_alloc(8);
	char* pointer5 = (char*)memory_alloc(8);
	char* pointer6 = (char*)memory_alloc(8);
	if (pointer3)
		if (memory_free(pointer3) == 0)
			pointer3 = NULL;
	char* pointer7 = (char*)memory_alloc(8);
	char* pointer8 = (char*)memory_alloc(8);
	char* pointer9 = (char*)memory_alloc(8);
	char* pointer10 = (char*)memory_alloc(8);
	char* pointer11 = (char*)memory_alloc(8);
	return 0;
}

void test2()
{
	char region[350];
	memory_init(region, 350);
	char* pointer1 = (char*)memory_alloc(59);
	char* pointer2 = (char*)memory_alloc(65);
	char* pointer3 = (char*)memory_alloc(17);
	char* pointer4 = (char*)memory_alloc(19);
	char* pointer5 = (char*)memory_alloc(85);
	char* pointer6 = (char*)memory_alloc(8);
	if (pointer1)
		if (memory_free(pointer1) == 0)
			pointer1 = NULL;
	if (pointer4)
		if (memory_free(pointer4) == 0)
			pointer4 = NULL;
	if (pointer6)
		if (memory_free(pointer6) == 0)
			pointer6 = NULL;
	char* pointer7 = (char*)memory_alloc(18);
	char* pointer8 = (char*)memory_alloc(18);
	char* pointer9 = (char*)memory_alloc(18);
	char* pointer10 = (char*)memory_alloc(18);
	char* pointer11 = (char*)memory_alloc(18);
	if (pointer8)
		if (memory_free(pointer8) == 0)
			pointer8 = NULL; 
	if (pointer11)
		if (memory_free(pointer11) == 0)
			pointer11 = NULL;
	if (pointer10)
		if (memory_free(pointer10) == 0)
			pointer10 = NULL;
	char* pointer12 = (char*)memory_alloc(56);
	char* pointer13 = (char*)memory_alloc(32);
	char* pointer14 = (char*)memory_alloc(58);
	char* pointer15 = (char*)memory_alloc(85);
	char* pointer16 = (char*)memory_alloc(58);

	return 0;
}

void test3()
{
	char region[100000];
	memory_init(region, 100000);
	char* pointer1 = (char*)memory_alloc(65595);
	char* pointer2 = (char*)memory_alloc(25444);
	char* pointer3 = (char*)memory_alloc(4823);
	char* pointer4 = (char*)memory_alloc(25638);
	if (pointer1)
		if (memory_free(pointer1) == 0)
			pointer1 = NULL;
	if (pointer2)
		if (memory_free(pointer2) == 0)
			pointer2 = NULL;
	if (pointer3)
		if (memory_free(pointer3) == 0)
			pointer3 = NULL;
	if (pointer4)
		if (memory_free(pointer4) == 0)
			pointer4 = NULL;
}



int main()
{
	test1();
	return 0;
}