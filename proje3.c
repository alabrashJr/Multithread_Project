#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>


/*
 bizde birkac kirtik bolge bulunmakta , birisi BerBer Dukkani yani Dukkanda sadece 4 kisi bulunmasi gerekiyor burda thread sayisinin
 ayarlayabilmek icin semaphore kullanmaktayim , 2. kirtik bolgemiz bekleme koltuklari 3tane koltugumuz var ve her birini bir
 kisi sigmakta burda mutex kullanabiliriz. Son bolgemiz tras koltugumuz orada da mutex kullanabiliriz .
 */
typedef struct Koltuklar
{
int thread_no;
int dolumu;
} Koltuk;

pthread_mutex_t bekleme_Koltugu1_Mutex;
pthread_mutex_t bekleme_Koltugu2_Mutex;
pthread_mutex_t bekleme_Koltugu3_Mutex;
pthread_mutex_t tras_Koltugu_Mutex;
pthread_t trasid;
sem_t semaMutex; // berbar salonu
int mevcutSayisi=0;
Koltuk Beklemekoltugu[3];
Koltuk trasK;
pthread_t tids[10];

void * TrasKoltugu(void * parameter );

void *BeklemeKoltugu1 (void * parameter ){
    printf("33.satir\n");
    pthread_mutex_lock(&bekleme_Koltugu1_Mutex);
     Koltuk * offset= ( Koltuk*)parameter;
    printf("K1<%d>:T%d\n",clock(),offset->thread_no);
    if(trasK.dolumu==1)
       wait(TrasKoltugu);
    printf("40.satir\n");
    offset->dolumu=0;
    trasK.thread_no=offset->thread_no;
    pthread_create(&trasid,NULL,TrasKoltugu,&trasK);
    mevcutSayisi--;
    pthread_join(trasid,0);
    printf("47.satir\n");
    pthread_mutex_unlock(&tras_Koltugu_Mutex);

}
void *BeklemeKoltugu2(void * parameter ){
    printf("49.satir\n");
    pthread_mutex_lock(&bekleme_Koltugu2_Mutex);
   Koltuk * offset= ( Koltuk*)parameter;
    printf("\tK2<%d>:T%d\n ",clock(),offset->thread_no);
    if(trasK.dolumu==1)
       wait(TrasKoltugu);
    offset->dolumu=0;
    trasK.thread_no=offset->thread_no;
    mevcutSayisi--;
    pthread_create(&trasid,NULL,TrasKoltugu,&trasK);
    pthread_join(trasid,0);
    pthread_mutex_unlock(&tras_Koltugu_Mutex);

}
void * BeklemeKoltugu3(void * parameter ){
    printf("64.satir\n");
    pthread_mutex_lock(&bekleme_Koltugu3_Mutex);
    Koltuk * offset= ( Koltuk*)parameter;
    printf("\tK3<%d>:T%d\n",clock(),offset->thread_no);
    if(trasK.dolumu==1)
       wait(TrasKoltugu);
    offset->dolumu=0;
    trasK.thread_no=offset->thread_no;
    mevcutSayisi--;
    pthread_create(&trasid,NULL,TrasKoltugu,&trasK);
    pthread_join(trasid,0);
    pthread_mutex_unlock(&tras_Koltugu_Mutex);

}

void * TrasKoltugu(void * parameter ){
    printf("81.satir\n");
   // pthread_mutex_lock(&tras_Koltugu_Mutex);
    printf("\tK0<%d>:T%d\n",clock(),trasK.thread_no);
//    if(offset->thread_no%3==0)
//        pthread_mutex_unlock(&bekleme_Koltugu2_Mutex);
//    if(offset->thread_no%3==1)
//        pthread_mutex_unlock(&bekleme_Koltugu3_Mutex);
//    if(offset->thread_no%3==2)
//        pthread_mutex_unlock(&bekleme_Koltugu1_Mutex);

}

void *BerberSalonu(int tid){
    int i=0;
    sem_wait(&semaMutex);                           //semaphore kritik bolgesi baslangici
    mevcutSayisi++;
    printf("%d. Thread Kiritik Bolgede\n",tid);

        if(Beklemekoltugu[0].dolumu==0){
            printf("103.satir\n");
            Beklemekoltugu[0].dolumu=1;
            Beklemekoltugu[0].thread_no=tid;
            pthread_create(tids+tid,NULL,BeklemeKoltugu1,&Beklemekoltugu[0]);
             pthread_mutex_unlock(&bekleme_Koltugu1_Mutex);
           i= pthread_join(tids[tid], 0);

        }else if(Beklemekoltugu[1].dolumu==0){
            printf("109.satir\n");
            Beklemekoltugu[1].dolumu=1;
            Beklemekoltugu[1].thread_no=tid;
             pthread_create(tids+tid,NULL,BeklemeKoltugu2,&Beklemekoltugu[1]);
             pthread_mutex_unlock(&bekleme_Koltugu2_Mutex);
            i=pthread_join(tids[tid], 0);
        }else if(Beklemekoltugu[2].dolumu==0){
            printf("115.satir\n");
            Beklemekoltugu[2].dolumu=1;
            Beklemekoltugu[2].thread_no=tid;
            pthread_create(tids+tid,NULL,BeklemeKoltugu3,&Beklemekoltugu[2]);
            pthread_mutex_unlock(&bekleme_Koltugu2_Mutex);
            i=pthread_join(tids[tid], 0);
        }
    if(mevcutSayisi==4){
                printf("Girin sayisi 4 oldu beklenyor \n");wait(&i);}
    else{
        printf("sleep %d\n",tid);sleep(tid);   }  // thread nosu saniyeyince bekle

    sem_post(&semaMutex);                           //semaphore kritik bolgesi Bitisi
     //printf("%d. Thread Kiritik Bolgeden Cikti\n",tid);
    return NULL;
}

int main(){
    trasK.dolumu=0; //basta Tras Koltuk bos
    if (pthread_mutex_init(&bekleme_Koltugu3_Mutex, NULL) != 0)
            {
        printf("\n mutex init hatasi\n");
        return 1;
            }
    if (pthread_mutex_init(&bekleme_Koltugu2_Mutex, NULL) != 0)
            {
        printf("\n mutex init hatasi\n");
        return 1;
             }
    if (pthread_mutex_init(&bekleme_Koltugu1_Mutex, NULL) != 0)
             {
            printf("\n mutex init hatasi\n");
            return 1;
            }
    if (pthread_mutex_init(&tras_Koltugu_Mutex, NULL) != 0)
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
       //pthread_mutex_lock(&tras_Koltugu_Mutex);
        pthread_mutex_lock(&bekleme_Koltugu1_Mutex);
       pthread_mutex_lock(&bekleme_Koltugu2_Mutex);
       pthread_mutex_lock(&bekleme_Koltugu3_Mutex);
    for (int var = 1; var <= 10; ++var) {
         pthread_join(tids[var], 0);
    }

//    for (int var = 0; var < 10; ++var) {
//        pthread_join(tid[var],0);
//    }


    sem_destroy(&semaMutex);
    pthread_mutex_destroy(&bekleme_Koltugu1_Mutex);
    pthread_mutex_destroy(&bekleme_Koltugu2_Mutex);
    pthread_mutex_destroy(&bekleme_Koltugu3_Mutex);
    pthread_mutex_destroy(&tras_Koltugu_Mutex);

	
	
	return 0 ;
}
