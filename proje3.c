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


void * TrasKoltugu(void * parameter );

void *BeklemeKoltugu1 (void * parameter ){
    pthread_mutex_lock(&bekleme_Koltugu1_Mutex);
    //printf("24");
     Koltuk * offset= ( Koltuk*)parameter;
    printf("\tK1<%d>:T%d ",clock(),offset->thread_no);
    offset->dolumu=0;
    pthread_create(&trasid,NULL,TrasKoltugu,&offset);
    pthread_join(&trasid,0);
    pthread_mutex_unlock(&tras_Koltugu_Mutex);

}
void *BeklemeKoltugu2(void * parameter ){
    pthread_mutex_lock(&bekleme_Koltugu2_Mutex);
   Koltuk * offset= ( Koltuk*)parameter;
    printf("\tK2<%d>:T%d ",clock(),offset->thread_no);
    offset->dolumu=0;
    pthread_create(&trasid,NULL,TrasKoltugu,&offset);
    pthread_join(&trasid,0);
    pthread_mutex_unlock(&tras_Koltugu_Mutex);

}
void * BeklemeKoltugu3(void * parameter ){
    pthread_mutex_lock(&bekleme_Koltugu3_Mutex);
    Koltuk * offset= ( Koltuk*)parameter;
    printf("\tK3<%d>:T%d ",clock(),offset->thread_no);
    offset->dolumu=0;
    pthread_create(&trasid,NULL,TrasKoltugu,&offset);
    pthread_join(&trasid,0);
    pthread_mutex_unlock(&tras_Koltugu_Mutex);

}

void * TrasKoltugu(void * parameter ){
    pthread_mutex_lock(&tras_Koltugu_Mutex);
    Koltuk * offset= ( Koltuk*)parameter;
    printf("\tK0<%d>:T%d ",clock(),offset->thread_no);
    if(offset->thread_no==10)
        printf("\n\t\tsssssssssssssssssssssssssssssssssss");
    if(offset->thread_no%3==0)
        pthread_mutex_unlock(&bekleme_Koltugu2_Mutex);
    if(offset->thread_no%3==1)
        pthread_mutex_unlock(&bekleme_Koltugu3_Mutex);
    if(offset->thread_no%3==2)
        pthread_mutex_unlock(&bekleme_Koltugu1_Mutex);

}

void *BerberSalonu(int tid){

    sem_wait(&semaMutex);                           //semaphore kritik bolgesi baslangici
    printf("%d. Thread Kiritik Bolgede\n",tid);

    sleep(tid);     // thread nosu saniyeyince bekle
    sem_post(&semaMutex);                           //semaphore kritik bolgesi Bitisi
     //printf("%d. Thread Kiritik Bolgeden Cikti\n",tid);
    return NULL;
}

int main(){
    Koltuk Beklemekoltugu[3];
    pthread_t tid[10];
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

   // printf("93");
    int beklemeSirasi=0;
   /*
    for (int var = 1; var <=10; ++var) {
        printf("96");
        if(beklemeSirasi==0){
           if(Beklemekoltugu[beklemeSirasi].dolumu==0){
               Beklemekoltugu[beklemeSirasi].dolumu=1;
               Beklemekoltugu[beklemeSirasi].thread_no=var;
               pthread_create(tid+var,NULL,BeklemeKoltugu1,Beklemekoltugu+beklemeSirasi);
               printf("102");
            }
        }if(beklemeSirasi==1){
            if(Beklemekoltugu[beklemeSirasi].dolumu==0){
                Beklemekoltugu[beklemeSirasi].dolumu=1;
                Beklemekoltugu[beklemeSirasi].thread_no=var;
           pthread_create(tid+var,NULL,BeklemeKoltugu2,Beklemekoltugu+beklemeSirasi);
           printf("109");
            }
        }if(beklemeSirasi==2){
            if(Beklemekoltugu[beklemeSirasi].dolumu==0){
                Beklemekoltugu[beklemeSirasi].dolumu=1;
                Beklemekoltugu[beklemeSirasi].thread_no=var;
           pthread_create(tid+var,NULL,BeklemeKoltugu3,Beklemekoltugu+beklemeSirasi);
           printf("116");
             }
        }if(beklemeSirasi>=3){
            beklemeSirasi=0;
            printf("120");
        }beklemeSirasi++;
    }
//    pthread_create(&tid,NULL,fonk1,0);
//    pthread_create(&tid2,NULL,fonk2,1);
//    pthread_create(&tid3,NULL,fonk3,2);
    */
  //  printf("126");
    for (int var = 1; var <= 10; ++var) {
          pthread_create(tid+var, NULL, BerberSalonu, var);
    }

    for (int var = 1; var <= 10; ++var) {
         pthread_join(tid[var], 0);
    }
//    pthread_mutex_lock(&tras_Koltugu_Mutex);
//    pthread_mutex_lock(&bekleme_Koltugu2_Mutex);
//    pthread_mutex_lock(&bekleme_Koltugu3_Mutex);
//    for (int var = 0; var < 10; ++var) {
//        pthread_join(tid[var],0);
//    }

   // printf("134");

    sem_destroy(&semaMutex);
    pthread_mutex_destroy(&bekleme_Koltugu1_Mutex);
    pthread_mutex_destroy(&bekleme_Koltugu2_Mutex);
    pthread_mutex_destroy(&bekleme_Koltugu3_Mutex);
    pthread_mutex_destroy(&tras_Koltugu_Mutex);

	
	
	return 0 ;
}
