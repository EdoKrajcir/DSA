// zadanie1.c -- Eduard Krajèír, 27.9.2018 10:13

#include <stdio.h>
#include <string.h>
#define MIN_ZAPIS 8

typedef struct main_hlavicka
{
	int velkost;
	struct volna_hlavicka *zaciatok;
}MAIN_HLAVICKA;

typedef struct volna_hlavicka
{
	int velkost;
	struct volna_hlavicka *next;
}VOLNA_HLAVICKA;

typedef struct plna_hlavicka
{
	int velkost;
}PLNA_HLAVICKA;

char *start;

void *memory_alloc(unsigned int size)
{

	int a, volnych = 0, odstup = 0, posledny = 0;
	VOLNA_HLAVICKA *pomocny, *najlepsi,*pomodst;
	MAIN_HLAVICKA *hlavna_hlavicka;
	VOLNA_HLAVICKA *volna_hlavicka;
	PLNA_HLAVICKA *novy, *vrat;
	hlavna_hlavicka = (char*)start;
	volna_hlavicka = (char*)hlavna_hlavicka->zaciatok;
	pomocny = hlavna_hlavicka->zaciatok;
	najlepsi = NULL;
	while (pomocny != NULL)
	{
		pomocny = pomocny->next;
		volnych++;
	}
	pomocny = hlavna_hlavicka->zaciatok;
	for (a = 0; a < volnych; a++)
	{
		if (pomocny->velkost == size + sizeof(VOLNA_HLAVICKA))
		{
			najlepsi = pomocny;
			break;
			posledny++;
		}

		if ((char*)pomocny->velkost > size + sizeof(PLNA_HLAVICKA) * 2 + sizeof(VOLNA_HLAVICKA) + MIN_ZAPIS)
		{
			najlepsi = pomocny;
			break;
			//if ((odstup == volnych - 1) && ())pomocny=pomocny;
		}
		pomocny = pomocny->next;
		odstup++;
	}

	if (najlepsi == NULL) return NULL;
	else if ((char*)najlepsi + 8 + size <= (char*)hlavna_hlavicka + hlavna_hlavicka->velkost)
	{
		if ((posledny = 0) && (volnych != 1))
		{
			novy = najlepsi;
			//novy = pomocny;
			pomocny = (char*)novy + sizeof(PLNA_HLAVICKA) * 2 + size;
			pomocny->velkost = najlepsi->velkost - sizeof(PLNA_HLAVICKA) * 2 - size - sizeof(VOLNA_HLAVICKA);
			if (hlavna_hlavicka->zaciatok == najlepsi) hlavna_hlavicka->zaciatok = pomocny;
			pomocny->next = najlepsi->next;
			novy->velkost = (-1) * size;
			if (odstup != 0)
			{
				pomodst = hlavna_hlavicka->zaciatok;
				for (a = 0; a < odstup - 1; a++)
				{
					pomodst = pomodst->next;
				}
				pomodst->next = pomocny;
			}
			//pomocny->velkost = pomocny->velkost - size - plna_hlavicka.
			//pomocny->next = NULL;
			//if ((char*)hlavna_hlavicka->zaciatok == (char*)novy) hlavna_hlavicka->zaciatok = pomocny;
			return novy;
		}

		else
		{
			novy = najlepsi;
			novy->velkost = (-1) * size;
			pomocny = hlavna_hlavicka->zaciatok;
			for (a = 0; a < odstup - 1; a++)
			{
				pomocny = pomocny->next;
			}
			pomocny->next = NULL;
			return novy;
		}
	}
	else return NULL;





}

int memory_free(void *valid_ptr)
{
	VOLNA_HLAVICKA *pomocny1, *pomocny2, *pomvol, *savepom;
	MAIN_HLAVICKA *hlavna_hlavicka;
	PLNA_HLAVICKA *plny;
	hlavna_hlavicka = (char*)start;
	int velkost;
	savepom = NULL;
	plny = (char*)valid_ptr;
	pomocny1 = (char*)valid_ptr;
	velkost = (char*)plny->velkost;
	pomvol = (char*)hlavna_hlavicka->zaciatok;
	if (hlavna_hlavicka->zaciatok == NULL)
	{
		hlavna_hlavicka->zaciatok = pomocny1;
		pomocny1->velkost = -velkost - 8;
		return 0;
	}
	while ((char*)pomvol < (char*)pomocny1)
	{
		savepom = pomvol;
		pomvol = pomvol->next;
	}
	pomocny1->next = pomvol;
	if (savepom == NULL) hlavna_hlavicka->zaciatok = pomocny1;
	else savepom->next = pomocny1;
	pomocny1->velkost = -velkost - 8;

		pomvol = hlavna_hlavicka->zaciatok;
		while (pomvol->next != NULL)
		{
			if ((char*)pomvol->next == (char*)pomvol + pomvol->velkost + sizeof(VOLNA_HLAVICKA))
			{
				pomvol->velkost = pomvol->velkost + sizeof(VOLNA_HLAVICKA) + pomvol->next->velkost;
				pomvol->next = pomvol->next->next;
				printf("%d", pomvol->velkost);
			}
			else pomvol = pomvol->next;
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
	start = ptr;
	hlavna_hlavicka = (char*)start;
	hlavna_hlavicka->velkost = size;
	hlavna_hlavicka->zaciatok = NULL;
	volna_hlavicka = (char*)hlavna_hlavicka + sizeof(MAIN_HLAVICKA);
	volna_hlavicka->velkost = size - sizeof(MAIN_HLAVICKA) - sizeof(VOLNA_HLAVICKA);
	volna_hlavicka->next = NULL;
	hlavna_hlavicka->zaciatok = (char*)volna_hlavicka;
}

// Vlastna funkcia main() je pre vase osobne testovanie. Dolezite: pri testovacich scenaroch sa nebude spustat!
int main()
{
	
	
	//printf("%d\n", sizeof(PLNA_HLAVICKA));
	char region[500];
	memory_init(region, 500);
	char* pointer1 = (char*)memory_alloc(23);
	char* pointer2 = (char*)memory_alloc(23);
	char* pointer3 = (char*)memory_alloc(10);
	char* pointer4 = (char*)memory_alloc(21);
	char* pointer5 = (char*)memory_alloc(19);
	char* pointer6 = (char*)memory_alloc(11);
	char* pointer7 = (char*)memory_alloc(13);
	char* pointer8 = (char*)memory_alloc(17);
	char* pointer9 = (char*)memory_alloc(13);
	char* pointer10 = (char*)memory_alloc(18);
	char* pointer11 = (char*)memory_alloc(12);
	char* pointer12 = (char*)memory_alloc(19);
	char* pointer13 = (char*)memory_alloc(23);
	/*char* pointer3 = (char*)memory_alloc(10);
	char* pointer4 = (char*)memory_alloc(10);
	char* pointer5 = (char*)memory_alloc(10);
	char* pointer6 = (char*)memory_alloc(10);
	char* pointer7 = (char*)memory_alloc(10);
	char* pointer8 = (char*)memory_alloc(10);
	char* pointer9 = (char*)memory_alloc(10);
	char* pointer10 = (char*)memory_alloc(10);
	printf("%s", pointer1);
	printf("%s", pointer2);
	printf("%s", pointer3);
	printf("%s", pointer4);
	printf("%s", pointer5);
	printf("%s", pointer6);
	printf("%s", pointer7);
	printf("%s", pointer8);
	printf("%s", pointer9);
	printf("%s", pointer10);*/

	//if (pointer)
		//memset(pointer, 0, 10);

	//printf("%d\n", sizeof(PLNA_HLAVICKA));
	
	if (pointer8)
		memory_free(pointer8);
	if (pointer9)
		memory_free(pointer9);
	if (pointer1)
		memory_free(pointer1);
	if (pointer2)
		memory_free(pointer2);
	if (pointer3)
		memory_free(pointer3);
	if (pointer5)
		memory_free(pointer5);
	if (pointer10)
		memory_free(pointer10);
	if (pointer11)
		memory_free(pointer11);
	if (pointer12)
		memory_free(pointer12);
	if (pointer6)
		memory_free(pointer6);
	/*if (pointer7)
		memory_free(pointer7);
	if (pointer4)
		memory_free(pointer4);
	if (pointer13)
		memory_free(pointer13);*/

	char* pointer21 = (char*)memory_alloc(23);
	char* pointer22 = (char*)memory_alloc(23);
	char* pointer23 = (char*)memory_alloc(10);
	char* pointer24 = (char*)memory_alloc(21);
	char* pointer25 = (char*)memory_alloc(19);
	char* pointer26 = (char*)memory_alloc(11);
	char* pointer27 = (char*)memory_alloc(13);
	char* pointer28 = (char*)memory_alloc(17);
	char* pointer29 = (char*)memory_alloc(13);
	char* pointer30 = (char*)memory_alloc(18);
	char* pointer31 = (char*)memory_alloc(12);
	char* pointer32 = (char*)memory_alloc(19);
	char* pointer33 = (char*)memory_alloc(23);



	//char* pointer3 = (char*)memory_alloc(10);
	//char* pointer4 = (char*)memory_alloc(10);


	return 0;
}