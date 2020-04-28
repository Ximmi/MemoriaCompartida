#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>

typedef struct{
    char elemento[10];
}pila;

int main(){
    key_t llave;
    int shmid, opc, i;
    int cont;
    char *caracter; 
    pila *pile;
    char caux;
    
    //Initialize(pile);

    //paso 1: conseguir clave común
    llave= ftok("/bin/ls", 20);
    if(llave==-1){
        perror("Error al generar llave\n");
        exit(-1);
    }

    //paso 2: crear la zona de memoria
    shmid= shmget(llave, sizeof(pile), IPC_CREAT|0777);
    if(shmid==-1){
        perror("Error al generar zona de memoria\n");
        exit(-1);
    }

    //paso 3: vincular zona de memoria con otro programa
    cont=atoi(shmat(shmid, 0, 0));
    pile=shmat(shmid, 0, 0);
    if(pile==NULL){
        perror("Error al vincular zona de memoria\n");
        exit(-1);
    }
    
    //Utilizamos la memoria compartida
    while(1){
        printf("\n---------------------------------\n");
        printf("\n\tPila de caracteres\n");
        printf("Elige una opción\n");
        printf("1: Ingresar elemento\n");
        printf("2: Retirar elemento\n");
        printf("3: Visualizar elemento\n");
        printf("4: Visualizar pila\n");
        printf("5: Finalizar\n");
        
        scanf("%i", &opc);
        switch(opc){
            case 1:
                //Ingresar elemento
                printf("\nElemento >");
                //fgets(caux, 1, stdin);
                //caux= getchar();
                scanf("%s", &caux);
                //caux= *caracter;
                
                if(cont==0){
                    pile->elemento[0]=caux;
                    cont++;
                    printf("caracter ingresado: %c\n", caux);
                    printf("cont: %d\n", cont);
                }
                else{
                    if(cont>=10){
                        printf("No se pueden ingresar más caracteres\n");
                    }
                    else{
                        pile->elemento[cont]=caux;
                        cont++;
                        printf("cont: %d\n", cont);
                    }
                }
            break;
            case 2:
                //Retirar elemento
                if(cont==0){
                    printf("No hay elementos en la pila\n");
                }
                else{
                    pile->elemento[cont]='x';
                    cont--;
                    printf("Elemento eliminado\n");
                    printf("cont: %d\n", cont);
                }
                printf("Elemento del tope retirado\n");
            break;
            case 3:
                //Visualizar elemento
                if(cont==0){
                    printf("No hay elementos en la pila\n");
                }
                else{
                    printf("cont: %d\n", cont);
                    printf("Elemento > %c\n", pile->elemento[cont]);
                }

            break;
            case 4:
               for(i=0; i<cont-1; i++){
                    printf(" %c |", pile->elemento[i]);
                }
            break;
            case 5:
                //Finalizar
                //paso 4
                shmdt((char *)&cont);
                shmdt((char *)pile);
                shmctl(shmid, IPC_RMID, 0);
                exit(0);
            break;
            default:
                printf("No es opción valida");
            break;        
        }    
    }


return 0;
}
