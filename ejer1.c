#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>

int main(){
    key_t llave;
    int shmid, opc;
    int *entero;
    //paso 1: conseguir clave común
    llave= ftok("/bin/ls", 39);
    if(llave==-1){
        perror("Error al generar llave\n");
        exit(-1);
    }

    //paso 2: crear la zona de memoria
    shmid= shmget(llave, sizeof(int), IPC_CREAT|0777);
    if(shmid==-1){
        perror("Error al generar zona de memoria\n");
        exit(-1);
    }

    //paso 3: vincular zona de memoria con otro programa
    entero= (int *)shmat(shmid, 0, 0);
    if(entero==NULL){
        perror("Error al vincular zona de memoria\n");
        exit(-1);
    }

    //Utilizamos la memoria compartida

    while(1){
        printf("\n----------------------\n");
        printf("\nVaraible entera COMPARTIDA\n");
        printf("1.- Modificar\n 2.- Consultar\n 3.- Finalizar\n\n");
        printf("Elige una opción\n");
        scanf("%i", &opc);
        switch(opc){
            case 1:
                printf("\nVariable >");
                scanf("%i", entero);
            break;
            case 2:
                printf("\nVariable >%i", *entero);
            break;
            case 3:
                //paso 4
                shmdt((char *)entero);
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
