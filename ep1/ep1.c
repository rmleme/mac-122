// Autores: Paulo Marcel Caldeira Yokosawa         Numero USP: 3095430
//          Rodrigo Mendes Leme                    Numero USP: 3151151
// Curso: computacao                  Data: 10/15/1999
// Professor: Paulo Feofiloff
// Exercicio Programa 1
// Compilador usado: GCC-Linux
// Descricao: le as palavras de um arquivo e imprime aquelas que formam a
//            maior classe de equivalencia (maior numero de anagramas).

#include <stdio.h>
#include <string.h>

#define MAX 225503
#define TAM_MAX 25

int le_arquivo (char a[MAX][TAM_MAX], FILE *arquivo);
void quicksort (char a[], int l , int r);
int partition (char a[], int l, int r);
void quicksort_strings (char a[MAX][TAM_MAX], int l, int r, int indice[]);
int partition_strings (char a[MAX][TAM_MAX], int l, int r, int indice[]);
int maior_classe (char a[MAX][TAM_MAX], int indice[], char *anagrama, int qtas_palavras);
void imprime_anagramas (char palavra[], FILE *arquivo);

// Funcao: main

int main(int argc, char *argv[])
{
  int i, qtas_palavras = 0, num_carac, indices[MAX];
  char vet_palav[MAX][TAM_MAX], palavra[TAM_MAX];
  FILE *arquivo = NULL;

  if (argc != 2)
  {
    printf("Erro! Numero de argumentos invalido!\n");
    return -1;
  }

  arquivo = fopen(argv[1], "r");
  if(arquivo == NULL)
  {
    printf("Erro! Nao pude abrir %s!\n", argv[1]);
    return -2;
  }
  qtas_palavras = le_arquivo(vet_palav, arquivo);
  fclose(arquivo);

  for (i = 0; i < qtas_palavras; i++)
    indices[i] = i;

  quicksort_strings(vet_palav, 0, qtas_palavras - 1, indices);
  num_carac = maior_classe(vet_palav, indices, palavra, qtas_palavras);
  printf("A maior classe de equivalencia possui %d caracteres. Ela e de tamanho %d.\n", num_carac, strlen(palavra));

  arquivo = fopen(argv[1], "r");
  imprime_anagramas(palavra, arquivo);
  fclose(arquivo);
  return 0;
}

// Funcao: le_arquivo
// Descricao: le as palavras de um arquivo e joga-as num vetor de strings que
//            mora em 0..cont_palav - 1.

int le_arquivo (char a[MAX][TAM_MAX], FILE *arquivo)
{
  char palav_lida[TAM_MAX];
  int cont_palav = 0;

  while (!feof(arquivo))
  {
    fgets(palav_lida, TAM_MAX, arquivo);
    palav_lida[strlen(palav_lida) - 1] = '\0';
    quicksort(palav_lida, 0, strlen(palav_lida) - 1);
    strcpy(a[cont_palav++], palav_lida);
  }
  return cont_palav;
}

// Funcao: quicksort
// Descricao: ordena um vetor de caracteres que mora em l..r.

void quicksort (char a[], int l, int r)
{
  int i;

  if ( r <= l) return;
  i = partition (a, l, r);
  if (i - l > r - i)
  {
    quicksort(a, i + 1, r);
    quicksort(a, l, i - 1);
  }
  else
  {
    quicksort(a, l, i - 1);
    quicksort(a, i + 1, r);
  }
}

// Funcao: partition
// Descricao: devolve um elemento i tal que de a[l..i - 1] os elementos de a
//            sao <= a[i], e de a[i + 1..r] sao >= a a[i].

int partition (char a[], int l, int r)
{
  int i = l - 1, j = r;
  char t, v = a[r];

  for (;;)
  {
    while (a[++i] < v);
    while (v < a[--j]) if (j == l) break;
    if (i >= j) break;
    t = a[i]; a[i] = a[j]; a[j] = t;
  }
  t = a[i]; a[i] = a[r]; a[r] = t;
  return i;
}

// Funcao: quicksort_strings
// Descricao: ordena, por um vetor de indices (acesso indireto), um vetor de
//            strings que mora em l..r.

void quicksort_strings (char a[MAX][TAM_MAX], int l, int r, int indices[])
{
  int i;

  if (r <= l) return;
  i = partition_strings(a, l, r, indices);
  if (i - l > r - i)
  {
    quicksort_strings(a, i + 1, r, indices);
    quicksort_strings(a, l, i - 1, indices);
  }
  else
  {
    quicksort_strings(a, l, i - 1, indices);
    quicksort_strings(a, i + 1, r, indices);
  }
}

// Funcao: partition_strings
// Descricao: devolve um elemento i tal que em a[indice[l..i - 1]] os elementos
//            sao <= a a[indice[i]], e em a[indice[i + 1..r]] sao >= a
//            a[indice[i]].

int partition_strings (char a[MAX][TAM_MAX], int l, int r, int indices[])
{
  int i = l - 1, j = r, t;
  char v[TAM_MAX];

  strcpy(v, a[indices[r]]);
  for (;;)
  {
    while (strcmp(a[indices[++i]], v) < 0);
    while (strcmp(v, a[indices[--j]]) < 0) if (j == l) break;
    if (i >= j ) break;
    t = indices[i]; indices[i] = indices[j]; indices[j] = t;
  }
  t = indices[i]; indices[i] = indices[r]; indices[r] = t;
  return i;
}

// Funcao: maior_classe
// Descricao: procura em a[0..qtas_palavras - 1] a maior classe de equivalencia
//            de a, retornando o total de letras dessa classe.

int maior_classe (char a[MAX][TAM_MAX], int indice[], char *anagrama, int qtas_palavras)
{
  int i = 0, j = 1, max_letras = 0, cont_letras = 0, tam_palav;

  while (i < qtas_palavras - 1)
  {
    tam_palav = strlen(a[indice[i]]);
    cont_letras = tam_palav;
    if (j < qtas_palavras)
      while (strcmp(a[indice[i]], a[indice[j++]]) == 0)
        cont_letras += tam_palav;
    if (cont_letras > max_letras)
    {
      max_letras = cont_letras;
      strcpy(anagrama, a[indice[i]]);
    }
    i = j;
  }
  return max_letras;
}

// Funcao: imprime_anagramas
// Descricao: procura num arquivo todos os anagramas da maior classe de
//            equivalencia (ja obtida por maior_classe), imprimindo-os.

void imprime_anagramas (char *palavra, FILE *arquivo)
{
  char palav_lida[TAM_MAX], original[TAM_MAX];

  printf("Anagramas da maior classe de equivalencia:\n");
  while (!feof(arquivo))
  {
    fgets(palav_lida, TAM_MAX, arquivo);
    palav_lida[strlen(palav_lida) - 1] = '\0';
    strcpy(original, palav_lida);
    quicksort(palav_lida, 0, strlen(palav_lida) - 1);
    if (strcmp(palavra, palav_lida) == 0)
      printf("%s\n", original);
  }
}






