#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>


typedef struct Koltuklar
{
int thread_no;
int dolumu;
} Koltuk;

sem_t bekleme_Koltugu1_Mutex;
sem_t bekleme_Koltugu2_Mutex;
sem_t bekleme_Koltugu3_Mutex;
pthread_t trasid;
sem_t tras_Koltugu_Mutex;
sem_t semaMutex; // berbar salonu
int mevcutSayisi=0;
Koltuk Beklemekoltugu[3];
Koltuk trasK;
pthread_t tids[10];

void * TrasKoltugu(void * parameter );

void *BeklemeKoltugu1 (void * parameter ){
    sem_wait(&bekleme_Koltugu1_Mutex);                          //semaphore kritik bolgesi baslangici
    printf("34.satir\n");
     Koltuk * offset= ( Koltuk*)parameter;
    printf("\tK1<%d>:T%d\n",clock(),offset->thread_no);
   if(trasK.dolumu==1)
    printf("girildi");
    //printf("40.satir\n");
    offset->dolumu=0;
    trasK.thread_no=offset->thread_no;
    pthread_create(&trasid,NULL,TrasKoltugu,&trasK);
    mevcutSayisi--;
    pthread_join(trasid,0);
     sem_post(&bekleme_Koltugu1_Mutex);                           //semaphore kritik bolgesi Bitisi

}
void *BeklemeKoltugu2(void * parameter ){
    printf("49.satir\n");
    sem_wait(&bekleme_Koltugu2_Mutex);                          //semaphore kritik bolgesi baslangici
   Koltuk * offset= ( Koltuk*)parameter;
    printf("\tK2<%d>:T%d\n ",clock(),offset->thread_no);
    if(trasK.dolumu==1)
       printf("girildi");
    offset->dolumu=0;
    trasK.thread_no=offset->thread_no;
    mevcutSayisi--;
    pthread_create(&trasid,NULL,TrasKoltugu,&trasK);
    pthread_join(trasid,0);
     sem_post(&bekleme_Koltugu2_Mutex);                           //semaphore kritik bolgesi Bitisi

}
void * BeklemeKoltugu3(void * parameter ){
   printf("64.satir\n");
    sem_wait(&bekleme_Koltugu3_Mutex);                          //semaphore kritik bolgesi baslangici
    Koltuk * offset= ( Koltuk*)parameter;
    printf("\tK3<%d>:T%d\n",clock(),offset->thread_no);
    if(trasK.dolumu==1)
       printf("girildi");
    offset->dolumu=0;
    trasK.thread_no=offset->thread_no;
    mevcutSayisi--;
    pthread_create(&trasid,NULL,TrasKoltugu,&trasK);
    pthread_join(trasid,0);
     sem_post(&bekleme_Koltugu3_Mutex);                           //semaphore kritik bolgesi Bitisi

}

void * TrasKoltugu(void * parameter ){
    printf("80.satir\n");
    sem_wait(&tras_Koltugu_Mutex);                           //semaphore kritik bolgesi baslangici
    trasK.dolumu=1;
    printf("\tK0<%d>:T%d\n",clock(),trasK.thread_no);
    sleep(trasK.thread_no);
    trasK.dolumu=0;
    sem_post(&tras_Koltugu_Mutex);                           //semaphore kritik bolgesi Bitisi
}

void *BerberSalonu(int tid){
    int i=0;
  //  sem_wait(&semaMutex);                           //semaphore kritik bolgesi baslangici
    mevcutSayisi++;
    printf("%d mevcut sayisi\n",mevcutSayisi);
    //printf("\t\t%d. Thread Kiritik Bolgede\n",tid);
        if(Beklemekoltugu[0].dolumu==0){
            printf("91.satir %d\n",tid);
            Beklemekoltugu[0].dolumu=1;
            Beklemekoltugu[0].thread_no=tid;
            pthread_create(tids+tid,NULL,BeklemeKoltugu1,&Beklemekoltugu[0]);
           i= pthread_join(tids[tid], 0);
        }else if(Beklemekoltugu[1].dolumu==0){
            printf("97.satir %d\n",tid);
            Beklemekoltugu[1].dolumu=1;
            Beklemekoltugu[1].thread_no=tid;
             pthread_create(tids+tid,NULL,BeklemeKoltugu2,&Beklemekoltugu[1]);
            i=pthread_join(tids[tid], 0);
        }else if(Beklemekoltugu[2].dolumu==0){
            printf("103.satir %d\n",tid);
            Beklemekoltugu[2].dolumu=1;
            Beklemekoltugu[2].thread_no=tid;
            pthread_create(tids+tid,NULL,BeklemeKoltugu3,&Beklemekoltugu[2]);
            i=pthread_join(tids[tid], 0);
        }
        if(mevcutSayisi==4){
                printf("Girin sayisi 4 oldu beklenyor \n");
                sleep(10);
         }


   // sem_post(&semaMutex);                           //semaphore kritik bolgesi Bitisi

}

int main(){
    trasK.dolumu=0; //basta Tras Koltuk bos
    if (sem_init(&bekleme_Koltugu3_Mutex,0,1) != 0)
            {
        printf("\n mutex init hatasi\n");
        return 1;
            }
    if (sem_init(&bekleme_Koltugu2_Mutex, 0,1) != 0)
            {
        printf("\n mutex init hatasi\n");
        return 1;
             }
    if (sem_init(&bekleme_Koltugu1_Mutex, 0,1) != 0)
             {
            printf("\n mutex init hatasi\n");
            return 1;
            }
    if (sem_init(&tras_Koltugu_Mutex, 0, 1) != 0)
             {
            printf("\n mutex init hatasi\n");
            return 1;
            }
    if (sem_init(&semaMutex, 0, 1) != 0)
      {
          printf("\n semafor init hatasi\n");
          return 1;
      }


    for (int var = 1; var <= 10; ++var) {
          pthread_create(tids+var, NULL, BerberSalonu, var);
    }

    for (int var = 1; var <= 10; ++var) {
         pthread_join(tids[var], 0);
    }



    sem_destroy(&semaMutex);
    sem_destroy(&bekleme_Koltugu1_Mutex);
    sem_destroy(&bekleme_Koltugu2_Mutex);
    sem_destroy(&bekleme_Koltugu3_Mutex);
    sem_destroy(&tras_Koltugu_Mutex);

	
	
	return 0 ;
}
