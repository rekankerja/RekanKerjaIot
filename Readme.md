# Rekan Kerja  
## Introduction  
Rekan Kerja merupakan sebuah projek opensource yang bertujuan untuk membuat sebuah sistem pemantauan pekerja pada saat melakukan WFH (Work Frome Home). Dengan Rekan Kerja diharapkan akan meningkatkan produktivitas pekerja saat melakukan WFH. Peningkatan produktivitas terjadi karena dengan Rekan Kerja integrasi antara management dengan pekerja saat melakukan WFH  dapat tetap terjadi meskipun terpisah jarak.  
Rekan kerja diprogram menggunakan bahasa pemrograman C++ dengan framework Arduino. Terdapat 2 versi yang ada pada rekan kerja saat ini yaitu versi lite dan original. Versi lite menggunakan perangkat mikrokontroller jenis [TTGO T-Display ESP32](http://www.lilygo.cn/prod_view.aspx?Id=1126) sedangkan untuk yang versi original digunakan ESP32 Cam.  
  
## Fitur  
 - Pemantauan pekerja melalui motion sensor.  
 - Pemantauan pekerja melalui face detection.  
 - Pesan Alarm.  
 - Perhitungan waktu produktif.  
 - Tracking Pekerja.  
  
## Contoh produk  
![Rekan Kerja](https://raw.githubusercontent.com/rekankerja/RekanKerjaIot/main/Images/RekanKerja.jpeg)  
![Rekan Kerja Lite Produk](https://raw.githubusercontent.com/rekankerja/RekanKerjaIot/main/Images/Rekan%20kerja%20Lite.jpeg)
![Rekan Kerja Original Produk](https://raw.githubusercontent.com/rekankerja/RekanKerjaIot/main/Images/Rekan%20Kerja%20Original.jpeg)

## Getting Started  
Rangkaian yang digunakan pada rekan kerja versi original adalah sebagai berikut:  
![Rekan Kerja Original](https://raw.githubusercontent.com/rekankerja/RekanKerjaIot/main/Images/Rekan%20Kerja%20Original%20Schematic.png)  
Sementara untuk versi lite digunakan rangkaian sebagai berikut:  
![Rekan Kerja Lite](https://raw.githubusercontent.com/rekankerja/RekanKerjaIot/main/Images/Rekan%20Kerja%20Lite%20Schematic.png)  

Dalam melakukan pemrograman terhadap alat perlu digunakan Arduino IDE dan jangan lupa untuk menginstall board ESP. 