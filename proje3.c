#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <unistd.h>

pthread_mutex_t tras_Koltugu_Mutex; //tras koltugununn 
sem_t semaMutex; // berbar salonunun

typedef struct Koltuklar
{
int thread_no;
int dolumu;
} Koltuk ;

Koltuk Beklemekoltugu[3]; //3 tane bekleme koltugu bulunmakta
Koltuk trasKoltugu;	//1 tane tras koltugu bulunmakta 

void *BerberSalonu(int tid){
    sem_wait(&semaMutex);                           //semaphore kritik bolgesi baslangici

    for (int var = 0; var < 3; ++var) {             //bos olan koltuga oturturmak icin
        if(Beklemekoltugu[var].dolumu==0){
            Beklemekoltugu[var].dolumu=1;
            Beklemekoltugu[var].thread_no=tid;
            break;
        }
    }
    if(Beklemekoltugu[0].dolumu==0)
             printf("K1[BOS]<>");
         else
             printf("K1[T%d]<>",Beklemekoltugu[0].thread_no);

    if(Beklemekoltugu[1].dolumu==0)
             printf("K2[BOS]<>");
        else
             printf("K2[T%d]<>",Beklemekoltugu[1].thread_no);

    if(Beklemekoltugu[2].dolumu==0)
             printf("K3[BOS]<>");
        else
             printf("K3[T%d]<>",Beklemekoltugu[2].thread_no);

    if(trasKoltugu.thread_no==0)
             printf("K0[BOS]\n");
        else
             printf("K0[T%d]\n",trasKoltugu.thread_no);
    

    TrasKoltugunaGec(tid);
    

    if(Beklemekoltugu[0].dolumu==0)
            printf("K1[BOS]<>");
       else
            printf("K1[T%d]<>",Beklemekoltugu[0].thread_no);

    if(Beklemekoltugu[1].dolumu==0)
            printf("K2[BOS]<>");
       else
            printf("K2[T%d]<>",Beklemekoltugu[1].thread_no);

    if(Beklemekoltugu[2].dolumu==0)
            printf("K3[BOS]<>");
       else
            printf("K3[T%d]<>",Beklemekoltugu[2].thread_no);

    if(trasKoltugu.thread_no==0)
         printf("K0[BOS]\n");
    else
         printf("K0[T%d]\n",trasKoltugu.thread_no);

    sem_post(&semaMutex);                           //semaphore kritik bolgesi Bitisi
    printf("\t\t%d. Thread dükkandan ayrıldı.\n",tid);
}
void TrasKoltugunaGec(int tid){
    pthread_mutex_lock(&tras_Koltugu_Mutex); // kritik bolgesi Baslangici

    //diziyi guncelleniyor, trasKoltugunaGecen Thread bekleme koltugundan kaldiriyorum
    for (int var = 0; var < 3; ++var) {
        if(Beklemekoltugu[var].thread_no==tid){
            Beklemekoltugu[var].dolumu=0;
            Beklemekoltugu[var].thread_no=0;
        }
    }
    trasKoltugu.thread_no=tid; //tras koltuguna gecen thread

    printf("\t\t%d. Thread berber koltuğunda traş oluyor<%d>\n",tid,clock());

    sleep(tid*0.5); //thread nosus saniyesince beklemede kalacaktir. 0.5ile carpma sebebi ise 10.threed yavaslatmak icin

    trasKoltugu.thread_no=0;

    printf("\t\t%d.  Thread berber koltuğundan kalktı<%d>\n",tid,clock());

    pthread_mutex_unlock(&tras_Koltugu_Mutex);  // kritik bolgesi Bitisi
}

int main(){
pthread_t tids[11];
    if (sem_init(&semaMutex, 0,4) != 0)
        {
            printf("\n sema mutex init hatasi\n");
            return 1;
         }
    if (pthread_mutex_init(&tras_Koltugu_Mutex, NULL) != 0)
       {
         printf("\n mutex init hatasi\n");
            return 1;
       }

    for (int var = 1; var <= 10; var++) {
           pthread_create(&tids[var], NULL, BerberSalonu, var);
    }

    for (int var = 1; var <=10; var++) {
         pthread_join(tids[var], 0);
    }

    sem_destroy(&semaMutex);
    pthread_mutex_destroy(&tras_Koltugu_Mutex);

	return 0 ;
}

