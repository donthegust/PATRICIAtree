#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define TRUE 1
#define FALSE !TRUE
#define D 8


typedef  char *String; // Ponteiro do tipo char
typedef  short boolean; // Short suporte metade do tamanho de um int

typedef enum{
  	Interno, Externo
} TipoNo;

typedef struct TipoPatNo *TArvore;
typedef struct TipoPatNo{
	TipoNo nt;
	union{
    	struct{
        	short Index;
        	char Caractere;
        	TArvore Esq, Dir;
    	} NInterno ;
    	String Chave;
	} NO;
} TipoPatNo;

char Caractere(short i, String k){
	/* Retorna o i-esimo Caractere da chave k a partir da esquerda */
	return i > strlen(k) ? NULL: k[i];
}

boolean MenorIgual(char a, char b){
	if(a == NULL)
   		return NULL;
	if( b == NULL)
   		return NULL;
	return a <= b ? TRUE : FALSE;
}

boolean EExterno(TArvore p){
	/* Verifica se o no e externo */
	return (p->nt == Externo) ? TRUE : FALSE;
}

TArvore CriaNoInt(short i, TArvore *Esq,  TArvore *Dir, char Caractere){ // No interno carrega index e caracter de comparacao
	TArvore p;
	p = (TArvore)malloc(sizeof(TipoPatNo));
	p->nt = Interno;
	p->NO.NInterno.Esq = *Esq;
	p->NO.NInterno.Dir = *Dir;
	p->NO.NInterno.Index = i;
	p->NO.NInterno.Caractere = Caractere;
	return p;
}

TArvore CriaNoExt(String k){ // No externo carrega valor
	TArvore p;
	p = (TArvore)malloc(sizeof(TipoPatNo));
	p->nt = Externo;
	p->NO.Chave = k;
	return p;
}

boolean Pesquisa(String k, TArvore t){
	if(EExterno(t)){ // Caminha ate um no filho
    	if (strncmp(k,t->NO.Chave,strlen(k)) == 0){ // Compara o valor procurado com o no atual (se baseando no tamanho do valor buscado)
      		return TRUE;
    	}else{
      		return FALSE;
  		}
	}
	if (MenorIgual(Caractere(t->NO.NInterno.Index, k),t->NO.NInterno.Caractere)) // Compara o caracter teste do valor pesquisado com o caracter teste do no interno atual
    	return Pesquisa(k, t->NO.NInterno.Esq); // se o caracter buscado e menor, a pesquisa caminha a esquerda da arvore recursivamente
	else 
		return Pesquisa(k, t->NO.NInterno.Dir); // Se o caracter for maior, a pesquisa caminha pra direita
}

void printBits(size_t const size, void const * const ptr){
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

void printNo(TArvore no, int esp){
    int i;
	if(no == NULL) // Se a arvore estiver vazia
    	printf("No Nulo\n");
	else if(no->nt == Externo){ // Se o no for uma folha e impresso com valor
   	 	for(i=0; i<esp; i++)
   		 	printf("\t");
    	printf("No{\n");
//    	for(i=0; i<esp; i++)
//   		 printf("\t");
//    	printf("  Tipo: Externo\n");
    	for(i=0; i<esp; i++)
   		 	printf("\t");
    	printf("  Chave: %s\n",no->NO.Chave);
    	for(i=0; i<esp; i++)
   		 	printf("\t");
    	printf("};\n\n",no->NO.Chave);
	}else{ // Se o no for comparativo e impresso com index e caracter teste
	   	for(i=0; i<esp; i++)
	   		printf("\t");
	    printf("Comp{\n");
//    	for(i=0; i<esp; i++)
//   		 printf("\t");
//    	printf("  Tipo: Interno\n");
	    for(i=0; i<esp; i++)
	   		printf("\t");
	    printf("  Indice: %d\n",no->NO.NInterno.Index);
	    for(i=0; i<esp; i++)
	   	 	printf("\t");
	    printf("  Caractere: %c\n",no->NO.NInterno.Caractere);
//	    for(i=0; i<esp; i++)
//	   	 	printf("\t");
//	    printf("  ASCII: %d\n",no->NO.NInterno.Caractere);
//	    printBits(sizeof(no->NO.NInterno.Caractere), &no->NO.NInterno.Caractere);
//	    printf("\n");
	    for(i=0; i<esp; i++)
	   	 	printf("\t");
	    printf("};\n\n");
	}
}

TArvore InsereEntre(String k, TArvore *t, short i){
	TArvore p;
	char caractere;
	if (EExterno(*t) || i < (*t)->NO.NInterno.Index){ // Se o no atual for externo ou indice teste novo ser menor que o antigo
    	/* cria um novo no externo */
    	if(EExterno(*t)){ //Se o no for externo
       		caractere = Caractere(i,(*t)->NO.Chave); // Atribui a var caracter o novo caracter teste
    	}else{
      		caractere = (*t)->NO.NInterno.Caractere; // Atribui a var caracter o caracter teste antigo
    	}
    	p = CriaNoExt(k); // Cria um novo no externo
    	if (MenorIgual(Caractere(i, k),caractere)) // Se o caracter teste atual for maior que o caracter da var
        	return (CriaNoInt(i,&p,t,Caractere(i, k))); //Retorna novo no interno com novo caracter teste
    	else 
			return (CriaNoInt(i,t,&p,caractere)); //Se não, retorna novo no interno com antigo caracter teste
	}else{						//Retorna caracter i do no interno
    	if (MenorIgual(Caractere((*t)->NO.NInterno.Index, k),(*t)->NO.NInterno.Caractere)) // Se o caracter teste do no interno atual for maior que o caracter teste do no
        	(*t)->NO.NInterno.Esq = InsereEntre(k,&(*t)->NO.NInterno.Esq,i); // Anda a esquerda da arvore
    	else
        	(*t)->NO.NInterno.Dir = InsereEntre(k,&(*t)->NO.NInterno.Dir,i); // Anda a direita da arvore
    	return (*t); // Retorna o no
	}
}

TArvore Insere(String k, TArvore *t){
	TArvore p;
	int i;
	// k = strlwr(k);
	if (*t == NULL){ // Se arvore vazia, cria um no para o valor
    	return (CriaNoExt(k));
	}else{
    	p = *t;
    	while (!EExterno(p)){ // Enquanto não chegarmos a um no externo
        	if (MenorIgual(Caractere(p->NO.NInterno.Index,k),p->NO.NInterno.Caractere)) // Compara o caracter teste do valor inserido com o caracter teste do no interno atual
            	p = p->NO.NInterno.Esq; // Se o novo valor for menor que o atual, caminha para esquerda da arvore
        	else
            	p = p->NO.NInterno.Dir; // Se o novo valor for maior que o atual, caminha para esquerda da arvore
    	}
    	/* acha o primeiro Caractere diferente */
    	i = 0;
    	if(strlen(k)>strlen(p->NO.Chave)){ // Testa se a palavra nova é maior que a do atual no externo
         	 
        	while ((i <= strlen(k)) && (Caractere(i, k) == Caractere(i, p->NO.Chave)))
            	i++; // Enquanto o contador for menor que a palavra nova e os caracteres das duas palavras forem iguais, i++
   	 
          	if (i >= strlen(k)){ // Se ultrapassar o tamanho da nova palavra e todos os caracteres eram iguais
            	printf("Erro: chave ja esta na arvore: %s == %s\n",k,p->NO.Chave); // A palavra já havia sido inserida
            	return (*t); // Retorna a arvore sem mudanças
          	}else
            	return (InsereEntre(k, t, i)); // Se não, insere na arvore
    	}else{
          	while ((i <= strlen(p->NO.Chave)) && (Caractere(i, k) == Caractere(i, p->NO.Chave)))
            	i++; // Enquanto o contador for menor que a palavra antiga e os caracteres das duas palavras forem iguais, i++
   	 
          	if (i >= strlen(p->NO.Chave)){ // Se ultrapassar o tamanho da nova palavra e todos os caracteres eram iguais
            	printf("Erro: chave ja esta na arvore: %s == %s\n",k,p->NO.Chave); // A palavra já havia sido inserida
            	return (*t); // Retorna a arvore sem mudanças
          	}else
            	return (InsereEntre(k, t, i)); // Se não, insere na arvore
    	}
	}
}

void Ordem(TArvore ap, int esp){
    int i;
	if(ap == NULL) // Se arvore vazia
    	return;
	if(!EExterno(ap)) // Se o no não for externo
    	Ordem(ap->NO.NInterno.Dir, esp+1); // Imprime direita
//	for(i=0; i<esp; i++)
//   	 printf("\t");
	printNo(ap, esp); // Imprime no atual
	if(!EExterno(ap)) // Se o no não for externo
    	Ordem(ap->NO.NInterno.Esq, esp+1); // Imprime esquerda
}

/*
 *
 */
 
int main(int argc, char** argv){
    
	FILE *file; 
	DIR *dir;
	struct dirent *lsdir;
	char diretorio[30], caminhoArquivo[50];
	String *palavra;
	int idDoc = 1, i;
	TArvore raiz = NULL;
    
	printf("Digite o caminho para a pasta onde estao os arquivos a serem lidos..\n");
	scanf("%s",&diretorio); // Le o nome do diretorio
	strcpy(caminhoArquivo, diretorio); // Copia o nome do diretorio na var caminhoArquivo
	printf("\n");
    
	dir = opendir(diretorio); // Abre o diretorio na var dir
	strcat(diretorio, "/");
	if(dir){ // Se o diretorio existir
    	while((lsdir = readdir(dir))){ // Enquanto for possivel ler o diretorio
       	 	
        	if(strcmp(lsdir->d_name,".directory") && strcmp(lsdir->d_name,".") && strcmp(lsdir->d_name,"..")){ // Se existir arquivos no diretorio
            	strcat(diretorio,lsdir->d_name); // Nome do arquivo e adicionado ao fim do diretorio

            	file = fopen(diretorio,"r"); // Le o arquivo

             	while(!feof(file)){ // Enquanto nao chegar no fim do arquivo
                 	palavra = (String)malloc(sizeof(char)*30); // Aloca nova palavra
               	 
                 	fscanf(file,"%s ",palavra); // Le e armazena palavra
                	 
                 	//if(strcmp(palavra,"")!=0 && strcmp(palavra,"\n")!=0&&strcmp(palavra,"\t")!=0)
                    	raiz = Insere(palavra,&raiz); // Insere palavra na arvore
                 	}
                	 
                	fclose(file); // Fecha o arquivo
                	strcpy(diretorio,caminhoArquivo); // Copia o nome do diretorio na var caminhoArquivo
                	strcat(diretorio,"/"); // Nome do arquivo e adicionado ao fim do diretorio
                	//idDoc++;
        	}
    	}  	 
	}else
    	printf("ERRO ! Caminho nao encontrado!\n");    
   	 
    	printf("\n");
    	Ordem(raiz, 0);
    	closedir(dir); // Fecha diretorio
    
	return (EXIT_SUCCESS);
}
