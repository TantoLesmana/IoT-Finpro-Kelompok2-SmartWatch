# IoT-Finpro-Kelompok2-SmartWatch

## I. Introduction to the problem and the solution

Dalam era teknologi yang semakin maju, kebutuhan akan smart devices yang dapat memantau kesehatan individu dan lingkungan sekitar menjadi semakin mendesak. Wearable device seperti smartwatch telah menjadi solusi populer untuk mendukung gaya hidup sehat melalui pemantauan real-time terhadap berbagai parameter kesehatan. Namun, banyak smartwatch di pasaran yang memiliki keterbatasan dalam integrasi data kesehatan dengan lingkungan users secara langsung dan interaktif. Di sisi lain, kondisi lingkungan, seperti temperature dan humidity, memiliki pengaruh signifikan terhadap kenyamanan dan kesehatan penghuni rumah. Sayangnya, kebanyakan devices yang berfungsi sebagai pemantau lingkungan tidak memiliki konektivitas langsung dengan wearable devices, sehingga informasi penting ini tidak dapat diakses secara cepat dan langsung oleh users.

Dengan demikian, terdapat kebutuhan akan sebuah solusi terintegrasi yang dapat menghubungkan data kesehatan users, seperti detak jantung, dengan kondisi lingkungan mereka. Solusi yang kami bawakan ini diharapkan dapat menyediakan data secara real-time melalui konektivitas internet dan ditampilkan di perangkat yang mudah diakses, seperti smartwatch. Oleh karena itu, pengembangan smartwatch berbasis ESP32 dengan kemampuan memantau detak jantung menggunakan sensor MAX30102 serta menampilkan waktu melalui server NTP menjadi solusi yang akan kami bawakan. Selain itu, integrasi dengan ESP32 remote yang mengumpulkan data temperature dan humidity menggunakan sensor DHT11/22 dan mengirimkan informasi ke platform Blynk memungkinkan users untuk memantau kondisi lingkungan rumah mereka secara bersamaan.


## II. Hardware design and implementation details

Sistem ini menggunakan dua unit ESP32 dengan peran yang berbeda namun saling terhubung. Pada ESP32 Smartwatch, hardware utama meliputi sensor MAX30102 yang berfungsi untuk memantau kadar oksigen dalam darah (SpO2). Sensor ini bekerja menggunakan prinsip fotopletismografi (PPG), di mana red light dan infrared dipancarkan melalui kulit, lalu intensitas cahaya yang diserap oleh darah diukur. Perubahan volume darah tersebut digunakan untuk mendeteksi detak jantung dan tingkat oksigenasi dalam darah. Data hasil pengukuran ini diproses oleh ESP32 dan ditampilkan pada LCD, yang berfungsi sebagai main user interface. Selain itu, ESP32 Smartwatch menampilkan waktu real-time yang diperoleh melalui NTP (Network Time Protocol) melalui koneksi internet.

Sedangkan, ESP32 Remote memiliki fungsi sebagai pemantau kondisi lingkungan rumah. Sensor DHT11/DHT22 digunakan untuk mengukur temperatur dan humidity secara berkala. Data lingkungan ini diambil oleh ESP32 dan dikirimkan ke platform Blynk melalui koneksi Wi-Fi. Blynk berperan sebagai platform monitoring jarak jauh, memungkinkan user mengakses informasi temperature dan humidity rumah dari smartwatch mereka. Kedua unit ESP32 saling terhubung melalui platform Blynk sehingga data dari ESP32 Remote dapat diterima dan ditampilkan di ESP32 Smartwatch. Dengan demikian, hardware design ini memungkinkan sistem untuk memantau kesehatan pengguna melalui kadar oksigen, sekaligus memberikan informasi kondisi lingkungan rumah secara real-time. Integrasi ini menciptakan solusi yang praktis, efisien, dan mudah diakses dalam satu wearable device yang terhubung ke network.


## III. Software implementation details

Software pada ESP32 Smartwatch berfokus pada pembacaan kadar oksigen dalam darah menggunakan sensor MAX30102. Sensor ini bekerja dengan prinsip PPG seperti yang sudah dijelaskan sebelumnya. Data ini kemudian diolah dengan low-pass filter untuk menghilangkan komponen AC, sehingga hasil pengukuran menjadi lebih akurat. Sistem juga dilengkapi mekanisme validasi untuk mendeteksi keberadaan jari pada sensor, memastikan pembacaan hanya dilakukan ketika jari terdeteksi. Hasil pengukuran SpO2 dan suhu ditampilkan secara real-time melalui serial monitor. 

Software pada ESP32 Remote dirancang untuk memantau temperatur dan humidity menggunakan sensor DHT11. Pembacaan data dilakukan secara berkala dengan memanfaatkan task FreeRTOS, yang memungkinkan eksekusi pembacaan sensor berjalan secara paralel tanpa mengganggu sistem utama. Sistem juga dilengkapi timer timeout yang memantau jika pembacaan sensor gagal. Jika terjadi kegagalan, data temperatur dan humidity terakhir yang tersimpan akan ditampilkan sebagai cadangan. Semua data temperatur dan humidity ditampilkan secara real-time melalui serial monitor dan siap dikirim ke platform Blynk. Alur dari program yang dirancang akan berdasarkan mengikuti flowchart yang tertera dibawah ini.

Flowchart untuk home:

<img width="362" alt="Picture1" src="https://github.com/user-attachments/assets/f03067ef-70c5-4171-a962-fcd832c6fcc5">


Flowchart untuk watch:

<img width="331" alt="Picture2" src="https://github.com/user-attachments/assets/06086185-15d2-48cc-80e3-157c185f0ab9">


## IV. Test results and performance evaluation

Berdasarkan hasil pengujian yang telah dilakukan, sistem smartwatch berbasis IoT ini berhasil menunjukkan fungsionalitasnya dalam pemantauan parameter kesehatan dan lingkungan secara real-time. Sistem ini mampu menampilkan data humidity, temperature, dan kadar oksigen pada interface smartwatch (display) yang terhubung ke ESP32 melalui Wi-Fi. Seperti yang terlihat dalam pengoperasiannya, smartwatch menampilkan data secara real-time, seperti temperature 28°C, humidity 71%, dan saturasi oksigen 73%. Fungsi utama sistem berjalan dengan baik, dengan data yang dikirimkan dan ditampilkan di layar dengan benar dan sesuai dengan serial monitor. 

Walaupun testing berhasil, masih ada beberapa room of improvement. Pertama, fitur notes belum sepenuhnya diimplementasikan, sehingga tampilan notes atau teks tambahan yang memberikan informasi lebih belum dapat muncul dengan tepat. Kedua, ada masalah dengan pengaturan timezone, karena sistem belum dapat melakukan perubahan timezone dengan baik, yang memengaruhi fleksibilitas pemilihan waktu yang ditampilkan pada device. Meskipun ada masalah ini, sistem secara keseluruhan sudah berfungsi dengan baik dan memenuhi tujuan dasar untuk pemantauan kesehatan dan lingkungan.


ESP Display :

![Picture6](https://github.com/user-attachments/assets/7e828722-2b11-46ee-992e-b5095e5c3443)

ESP Home :

![Picture7](https://github.com/user-attachments/assets/477b9cbd-837a-4005-8cb2-88d70dc8f04a)

ESP Watch :


![Picture8](https://github.com/user-attachments/assets/e3f50d5a-013f-4143-9520-976301c4ff5c)



Hasil Implementasi pada sistem:


![Picture4](https://github.com/user-attachments/assets/7f0dd9ed-c727-4a4b-96e6-ef29cd757464)


![Picture5](https://github.com/user-attachments/assets/67e1937b-402f-41cf-8797-0345ac2d6226)


## V. Conclusion and future work

Proyek smartwatch berbasis IoT ini berhasil memantau parameter kesehatan dan lingkungan secara real-time, termasuk temperature, humidity, dan kadar oksigen. Sistem berjalan baik dengan integrasi ESP32 Smartwatch dan ESP32 Remote melalui Blynk. Walaupun fitur notes dan penyesuaian timezone masih perlu diperbaiki, sistem ini telah memenuhi tujuan utama untuk pemantauan kesehatan dan lingkungan yang terhubung dan dapat diakses secara real-time. Meskipun proyek ini telah berhasil mencapai tujuan utamanya, terdapat beberapa area yang dapat dikembangkan lebih lanjut untuk meningkatkan kinerja dan fungsionalitas sistem seperti :

  1. Pengembangan Fitur Tambahan


     Fitur notes yang belum sepenuhnya diimplementasikan dapat dikembangkan untuk memberikan informasi tambahan kepada pengguna, sehingga tampilan notes atau teks tambahan yang memberikan informasi lebih belum dapat muncul dengan tepat. Selain itu, kemampuan untuk mengatur zona waktu secara fleksibel akan memberikan kenyamanan lebih bagi pengguna yang berada di wilayah dengan perbedaan waktu.

  3. Pengoptimalan Performa Sistem

     
     Implementasi algoritma pengolahan data yang lebih efisien dapat mengurangi konsumsi daya dan meningkatkan responsivitas sistem. Selain itu, penggunaan teknik low-power mode pada ESP32 dapat diperluas untuk memperpanjang masa pakai perangkat jika digunakan dalam kondisi daya terbatas.

  4. Keamanan Data dan Konektivitas
     
     Peningkatan pada keamanan koneksi Wi-Fi, seperti penggunaan protokol enkripsi yang lebih aman, dapat dilakukan untuk melindungi data pengguna dari ancaman keamanan. Selain itu, sistem dapat ditingkatkan dengan fitur otentikasi berbasis akun untuk memastikan data hanya dapat diakses oleh pengguna yang sah.

