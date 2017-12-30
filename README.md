Linux thread senkronizasyon problemi. Berber salonu.
Bir berber salonunda 1 adet tıraş koltuğu ve 3 adet bekleme koltuğu bulunmaktadır. Berber salonuna
girebilen müşteri sayısı en fazla 4 olabilmektedir. Berberin bir müşteriyi tıraş süresi 20 dakikadır. Berber
müşterisini tıraş ettikten sonra koltuklardaki diğer müşterilerden birini işleme almaktadır


![pic](http://i65.tinypic.com/16hrted.png)








Teknik açıklama:
Oluşturulacak müşteri thread sayısı 10’dur. Her thread tıraş olduktan sonra thread nosu saniyesince
beklemede kalacaklardır. Tıraş koltuğuna oturan thread: “thread no <> sistem zamanı” bilgilerini
yazdıracaktır. Thread koltuğuna oturduğu anda bir thread sistemde var olan koltukların durumlarını
yazdıracaktır. Koltukları birer yapı olarak tanımlamalı daha sonra global bir dizide tutmalısınız. Bir thread
berber dükkanından içeri girdiğinde boş olan koltuğa oturmalı diziyi güncellemelidir. Aynı zamanda bir
thread berber dükkanından çıktığında da mevcut dizi güncellenmelidir. Her güncelleme işleminde koltuk
durumları yanyana ekrana yazdırılacaktır: “K1 [T0] <> K2[T1] <> K3[T6] <> K0[T8]” olarak yazılmalıdır.


Koltuk{

int thread_no;

int dolumu;

}
