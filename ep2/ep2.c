// Autores: Paulo Marcel Caldeira Yokosawa         Numero USP: 3095430
//          Rodrigo Mendes Leme                    Numero USP: 3151151
// Curso: computacao                  Data: 17/11/1999
// Professor: Paulo Feofiloff
// Exercicio Programa 2
// Compilador usado: GCC-Linux
// Nome do arquivo: ep2.c
// Descricao: le um arquivo de palavras de um dado tamanho em busca da menor
//            escada que ligue duas outras palavras dadas.

#include <stdio.h>
#include <string.h>

#define MAX 40000
#define TAM_MAX 25

typedef struct elemento um_elemento;

struct elemento
{
  char palav[TAM_MAX];
  int marca;
};

typedef struct node * link;

struct node
{
  char palav[TAM_MAX];
  link next;
};

// Funcao: le_arquivo
// Descricao: le as palavras de um arquivo e joga as de um mesmo tamanho
//            tam_palav num vetor de structs que mora em 0..cont_palav - 1.

int le_arquivo (um_elemento a[], FILE *arquivo, int tam_palav)
{
  char palav_lida[TAM_MAX];
  int cont_palav = 0;

  while (!feof(arquivo) && cont_palav <= MAX)
  {
    fgets(palav_lida, TAM_MAX, arquivo);
    palav_lida[strlen(palav_lida) - 1] = '\0';
    if (strlen(palav_lida) == tam_palav)
      strcpy(a[cont_palav++].palav, palav_lida);
  }
  return cont_palav;
}

// Funcao: merge
// Descricao: recebe vetores crescentes a[l..m] e a[m + 1..r] e rearranja o
//            vetor a[l..r] de modo que ele fique crescente.

int merge (um_elemento a[], int l, int m, int r)
{
  um_elemento aux[MAX];
  int i,
      j,
      k;

  for (i = m + 1; i > l; i--)
    aux[i - 1] = a[i - 1];
  for (j = m; j < r; j++)
    aux[r + m - j] = a[j + 1];
  for (k = l; k <= r; k++)
    if (strcmp(aux[i].palav, aux[j].palav) < 0)
      a[k] = aux[i++];
    else
      a[k] = aux[j--];
}

// Funcao: mergesort
// Descricao: ordena por ordem alfabetica um vetor de structs que mora em l..r.

void mergesort (um_elemento a[], int l, int r)
{
  int m = (l + r) / 2;

  if (r <= l)
    return;
  else
  {
    mergesort(a, l, m);
    mergesort(a, m + 1, r);
    merge(a, l, m, r);
  }
}

// Funcao: procura
// Descricao: procura num vetor, de l ate r, um elemento procurado.

int procura (um_elemento a[], int l, int r, char *procurado)
{
  int m;

  while (l <= r)
  {
    m = (l + r) / 2;
    if (strcmp(a[m].palav, procurado) == 0)
      return m;
    else
      if (strcmp(a[m].palav, procurado) < 0)
        l = m + 1;
      else
        r = m - 1;
  }
  return -1;
}

// Funcao: e_adj
// Descricao: recebe duas strings retornando a existencia ou nao de adjacencia
//            entre elas.

int e_adj (char *palav1, char *palav2)
{
  int i,
      j;

  for (i = 0; palav1[i] != '\0' && palav1[i] == palav2[i]; i++);
  for (j = strlen(palav1) - 1; j >= 0 && palav1[j] == palav2[j]; j--);
  return i == j;
}

// Funcao: escada
// Descricao: procura num vetor a[0..r] palavras que formem a menor escada
//            entre as strings p1 e p2.

link escada (um_elemento a[], char *p1, char *p2, int r)
{
  int i, f, k, x = 0, y, teste = 1, sentinela = 1;
  link head, t;

  i = procura(a, 0, r, p1);
  if (i == -1)
  {
    printf ("Erro: palavra %s inexistente!!!\n", p1);
    return NULL;
  }

  f = procura(a, 0, r, p2);
  if (f == -1)
  {
    printf("Erro: palavra %s inexistente!!!\n", p2);
    return NULL;
  }
  a[i].marca = 1;

  while (a[f].marca == 0 && sentinela == 1)
  {
    sentinela = 0;
    for (k = 0; k <= r; k++)
      if (a[k].marca == teste)
        for (y = 0; y <= r; y++)
          if(a[y].marca == 0 && a[k].marca > a[y].marca)
            if (e_adj(a[k].palav, a[y].palav))
            {
              sentinela = 1;
              a[y].marca = a[k].marca + 1;
            }
    teste++;
  }

  if (a[f].marca == 0)
  {
    printf("Nenhuma escada liga %s e %s.\n", p1, p2);
    return NULL;
  }

  head = (link) malloc(sizeof (struct node));
  strcpy(head->palav, a[f].palav);
  head->next = NULL;
  teste--;
  while (strcmp(head->palav, a[i].palav) != 0)
  {
    if (a[x].marca == teste && e_adj(a[x].palav, head->palav))
    {
      t = head;
      head = (link) malloc(sizeof (struct node));
      strcpy(head->palav, a[x].palav);
      head->next = t;
      x = -1;
      teste--;
      t = head;
    }
    x++;
  }
  return head;
}

// Funcao: main
// Obs.: argv[1] == arquivo de entrada; argv[2] == tamanho das palavras que
//       serao processadas; argv[3] == 1 se o arquivo estiver ordenado, 0 caso
//       contrario.

int main (int argc, char *argv[])
{
  int qtas_palavras,
      indice;
  um_elemento elementos[MAX];
  char palavra1[TAM_MAX],
       palavra2[TAM_MAX];
  FILE *arquivo;
  link pri,
       atu;

  if (argc != 4)
  {
    printf("Erro: numero de argumentos invalido.\n");
    return -1;
  }

  if (atoi(argv[3]) != 0 && atoi(argv[3]) != 1)
  {
    printf("Erro: 3o. arg deve valer 1 (arq ordenado) ou 0 (nao ordenado).\n");
    return -2;
  }

  if (atoi(argv[2]) < 1 || atoi(argv[2]) > TAM_MAX)
  {
    printf("Erro: 2o. argumento deve ser >= 1 e <= %d.\n", TAM_MAX);
    return -3;
  }

  arquivo = fopen(argv[1], "r");
  if(arquivo == NULL)
  {
    printf("Erro: %s nao pode ser aberto.\n", argv[1]);
    return -4;
  }
  qtas_palavras = le_arquivo(elementos, arquivo, atoi(argv[2]));
  fclose(arquivo);
  if (qtas_palavras > MAX)
  {
    printf("Erro: numero de palavras a ser processado e muito grande.\n");
    return -5;
  }

  if (atoi(argv[3]) == 0)        // Arquivo nao esta ordenado
    mergesort(elementos, 0, qtas_palavras - 1);

  do
  {
    for (indice = 0; indice <= qtas_palavras - 1; indice++)
      elementos[indice].marca = 0;

    do
    {
      printf("Digite a primeira palavra: ");
      gets(palavra1);
      if (strlen(palavra1) != atoi(argv[2]) && palavra1[0] != '\0')
        printf("Erro: tamanho da palavra invalido.\n");
    }
    while (strlen(palavra1) != atoi(argv[2]) && palavra1[0] != '\0');

    if (palavra1[0] != '\0')
      do
      {
        printf("Digite a segunda palavra: ");
        gets(palavra2);
        if (strlen(palavra2) != atoi(argv[2]) && palavra2[0] != '\0')
          printf("Erro: tamanho da palavra invalido.\n");
      }
      while (strlen(palavra2) != atoi(argv[2]) && palavra2[0] != '\0');

    if (palavra1[0] != '\0' && palavra2[0] != '\0')
    {
      pri = escada(elementos, palavra1, palavra2, qtas_palavras - 1);
      atu = pri;
      while (atu != NULL)
      {
        printf("%s\n", atu->palav);
        atu = atu->next;
      }
    }
  }
  while (palavra1[0] != '\0' && palavra2[0] != '\0');
  return 0;
}
