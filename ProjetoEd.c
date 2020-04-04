#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_BUCKETS 369539   //buckets number 
#define SIZE_STRING 100      //word max size

int numColisoes;

typedef struct Node { //struct node
    struct Node *next;
    char *key;
};

typedef struct HashTable {              //struct of the hashtable
    struct Node* buckets[MAX_BUCKETS];
    int numElements[MAX_BUCKETS];
} HashTable;


unsigned int Hash(char *word){         //Hash function: jb2 hashing
    
    unsigned long hash = 5381;
    int c;
    while (c = *word++)
        hash = ((hash << 5) + hash) + c;  
    return hash % MAX_BUCKETS;

}

HashTable* Create(){   
    HashTable *h1 = (HashTable*)malloc(sizeof(HashTable)); //allocating hashtable in the memory
    for (int i = 0; i<MAX_BUCKETS;i++){
        h1->buckets[i]= NULL;                              //seting every bucket for null
        h1->numElements[i]=0;           
    }
    return h1;                                             //returning created hashtable
}

void Insert(HashTable *hashtable, char *word){
    unsigned hashNumber = Hash(word);                                  //aplying the hash function to the word received

    struct Node* newNode = (struct Node*) malloc(sizeof(struct Node)); //allocating node in the memory
    newNode->key = (char*) malloc(SIZE_STRING * sizeof(char));         //allocating string in the memory
    
    strcpy(newNode->key,word);                                         //copying the word received to the node key
    newNode->next=NULL;
    

    if(!hashtable->buckets[hashNumber]){                   //if the bucket related to the hash number is empty
        
        hashtable->buckets[hashNumber] = newNode;          //seting this bucket to the node previously created
        hashtable->numElements[hashNumber]++;              //increasing the number of elements in the hashtable
        
    }else{         // if the bucket is already filled
        struct Node* aux = hashtable->buckets[hashNumber]; //creating an auxiliar node

        while(aux->next != NULL){                          //while the node is not the last one, move foward
            aux = aux->next;
            
        }
        aux->next = newNode;                               //seting the last node to the node previously created
        hashtable->numElements[hashNumber]++;              //increasing the number of elements in the hashtable
    numColisoes++;                                         //increasing the number of colisions for testing purposes
        
    }

    //printf("Bucket: [%d] Numero de elementos no bucket: %d\n", hashNumber, hashtable->numElements[hashNumber]);
    //printf("Bucket: [%d] colisoes: %d\n",hashNumber, hashtable->numElements[hashNumber]);

}

int Search(HashTable *hashtable, char *word){

    unsigned hashNumber = Hash(word);                   //aplying the hash function to the word received

    struct Node* aux = hashtable->buckets[hashNumber];  //creating an auxiliar node

    while(aux){                                         //if the node is filled
        if(strcmp(aux->key, word) == 0 )                //compare the word received with the node's key
            return 1;                                   //return true
        
        //printf("The word found was: %s with hash : %d \n", word ,Hash(word));
        aux = aux->next;                                //move foward
    }


    return 0;
}

void printaTudo(HashTable *hashtable){                  //an experimental function to see how many elements are in each bucket
    int empty=0,full=0,bucket1=0,bucket2=0,bucket3=0,bucket4=0,bucket5=0,bucket6=0,bucketmaior=0;
    for (int i = 0; i<MAX_BUCKETS;i++){
        /*if(hashtable->numElements[i]==0){
            empty++;
        }else{
            full++;
        }*/
        if(hashtable->numElements[i]==1){
            bucket1++;
        }else if (hashtable->numElements[i]==2){
            bucket2++;
        }else if(hashtable->numElements[i]==3){
            bucket3++;
        }else if (hashtable->numElements[i]==4){
            bucket4++;
        }else if(hashtable->numElements[i]==5){
            bucket5++;
        }else if (hashtable->numElements[i]==6){
            bucket6++;
        }else if(hashtable->numElements[i]>6){
            bucketmaior++;
        }
        
        //printf("Bucket: [%d], Numero de elementos: %d\n",i,hashtable->numElements[i]);
    }
    printf("Buckets com 1: %d\nBuckets com 2: %d\nBuckets com 3: %d\nBuckets com 4:%d\nBuckets com 5 :%d\nBuckets com 6:%d\nBuckets com mais de 6:%d\n",bucket1,bucket2,bucket3,bucket4,bucket5,bucket6,bucketmaior);
    //printf("Buckets vazios:%d Buckets preenchidos:%d\n", empty,full);
}

int main(void){
    FILE *dictionary = fopen("ascii_noaccent_noduplicates_FIXED_v2.txt","r");   
    FILE *output = fopen("outpux.txt", "w");                                    //opening the input, output and the dictionary text files
    FILE *input = fopen("got.txt", "r");

    clock_t tempo[2]= {0,0};    
    char space[]= " ";
    char *token = NULL;
    char entradaDictionary[SIZE_STRING], entradaTexto[SIZE_STRING];
    int erro=0, palavras=0;

    HashTable *hashtable = Create();    //creating hashtable
    
    //inserting dictionary words in the hashtable
    while(fgets(entradaDictionary,SIZE_STRING,dictionary)){          //while the fgets function is fifnding words
        
        if(entradaDictionary[strlen(entradaDictionary) - 1] == '\n') //if the last char in the string is a \n change to a \0
            entradaDictionary[strlen(entradaDictionary) - 1] = '\0';
        
        Insert(hashtable, entradaDictionary); 
        
    }

    //searching words
    while(fgets(entradaTexto,SIZE_STRING,input)){           //while the fgets function is fifnding words
        
        if(entradaTexto[strlen(entradaTexto) - 1] == '\n')  //if the last char in the string is a \n change to a \0
            entradaTexto[strlen(entradaTexto) - 1] = '\0';
        
        tempo[0] += clock();                                //seting the initial time of execution
        token = strtok(entradaTexto, space);                //defining the token as a space to separate each word
        
        while( token != NULL ) {                            //while the token isnt empty
            palavras++;
            if(!Search(hashtable, token)){                  //if the word is not found in the dictionary
                erro++;
                
            }
                
            token = strtok(NULL, space);                    //reseting token
        }
        tempo[1]+=clock();                                  //seting final time of execution
    }
    
    fprintf(output, "Numero total de palavras do texto: %d.\n", palavras); //print total words in the text 
    fprintf(output, "Numero total de palavras que falharam no spell check: %d.\n", erro);   //printing errors in the text   
    fprintf(output, "Tempo de pesquisa: %.1lf ms.\n", (double) (tempo[1] - tempo[0]) * 1000.0 / CLOCKS_PER_SEC);    //print the time in miliseconds
    

    
    fclose(input);
    fclose(output);     //closing the archives
    fclose(dictionary);

    return 0;
}