# IoT-Finpro-Kelompok2-SmartWatch

## Introduction to the problem and the solution

Dalam era teknologi yang semakin maju, kebutuhan akan smart devices yang dapat memantau kesehatan individu dan lingkungan sekitar menjadi semakin mendesak. Wearable device seperti smartwatch telah menjadi solusi populer untuk mendukung gaya hidup sehat melalui pemantauan real-time terhadap berbagai parameter kesehatan. Namun, banyak smartwatch di pasaran yang memiliki keterbatasan dalam integrasi data kesehatan dengan lingkungan users secara langsung dan interaktif. Di sisi lain, kondisi lingkungan, seperti temperature dan humidity, memiliki pengaruh signifikan terhadap kenyamanan dan kesehatan penghuni rumah. Sayangnya, kebanyakan devices yang berfungsi sebagai pemantau lingkungan tidak memiliki konektivitas langsung dengan wearable devices, sehingga informasi penting ini tidak dapat diakses secara cepat dan langsung oleh users.

Dengan demikian, terdapat kebutuhan akan sebuah solusi terintegrasi yang dapat menghubungkan data kesehatan users, seperti detak jantung, dengan kondisi lingkungan mereka. Solusi yang kami bawakan ini diharapkan dapat menyediakan data secara real-time melalui konektivitas internet dan ditampilkan di perangkat yang mudah diakses, seperti smartwatch. Oleh karena itu, pengembangan smartwatch berbasis ESP32 dengan kemampuan memantau detak jantung menggunakan sensor MAX30102 serta menampilkan waktu melalui server NTP menjadi solusi yang akan kami bawakan. Selain itu, integrasi dengan ESP32 remote yang mengumpulkan data temperature dan humidity menggunakan sensor DHT11/22 dan mengirimkan informasi ke platform Blynk memungkinkan users untuk memantau kondisi lingkungan rumah mereka secara bersamaan.


## Hardware design and implementation details

Sistem ini menggunakan dua unit ESP32 dengan peran yang berbeda namun saling terhubung. Pada ESP32 Smartwatch, hardware utama meliputi sensor MAX30102 yang berfungsi untuk memantau kadar oksigen dalam darah (SpO2). Sensor ini bekerja menggunakan prinsip fotopletismografi (PPG), di mana red light dan infrared dipancarkan melalui kulit, lalu intensitas cahaya yang diserap oleh darah diukur. Perubahan volume darah tersebut digunakan untuk mendeteksi detak jantung dan tingkat oksigenasi dalam darah. Data hasil pengukuran ini diproses oleh ESP32 dan ditampilkan pada LCD, yang berfungsi sebagai main user interface. Selain itu, ESP32 Smartwatch menampilkan waktu real-time yang diperoleh melalui NTP (Network Time Protocol) melalui koneksi internet.

Sedangkan, ESP32 Remote memiliki fungsi sebagai pemantau kondisi lingkungan rumah. Sensor DHT11/DHT22 digunakan untuk mengukur temperatur dan humidity secara berkala. Data lingkungan ini diambil oleh ESP32 dan dikirimkan ke platform Blynk melalui koneksi Wi-Fi. Blynk berperan sebagai platform monitoring jarak jauh, memungkinkan user mengakses informasi temperature dan humidity rumah dari smartwatch mereka. Kedua unit ESP32 saling terhubung melalui platform Blynk sehingga data dari ESP32 Remote dapat diterima dan ditampilkan di ESP32 Smartwatch. Dengan demikian, hardware design ini memungkinkan sistem untuk memantau kesehatan pengguna melalui kadar oksigen, sekaligus memberikan informasi kondisi lingkungan rumah secara real-time. Integrasi ini menciptakan solusi yang praktis, efisien, dan mudah diakses dalam satu wearable device yang terhubung ke network.


## Software implementation details

Software pada ESP32 Smartwatch berfokus pada pembacaan kadar oksigen dalam darah menggunakan sensor MAX30102. Sensor ini bekerja dengan prinsip PPG seperti yang sudah dijelaskan sebelumnya. Data ini kemudian diolah dengan low-pass filter untuk menghilangkan komponen AC, sehingga hasil pengukuran menjadi lebih akurat. Sistem juga dilengkapi mekanisme validasi untuk mendeteksi keberadaan jari pada sensor, memastikan pembacaan hanya dilakukan ketika jari terdeteksi. Hasil pengukuran SpO2 dan suhu ditampilkan secara real-time melalui serial monitor.

Software pada ESP32 Remote dirancang untuk memantau temperatur dan humidity menggunakan sensor DHT11. Pembacaan data dilakukan secara berkala dengan memanfaatkan task FreeRTOS, yang memungkinkan eksekusi pembacaan sensor berjalan secara paralel tanpa mengganggu sistem utama. Sistem juga dilengkapi timer timeout yang memantau jika pembacaan sensor gagal. Jika terjadi kegagalan, data temperatur dan humidity terakhir yang tersimpan akan ditampilkan sebagai cadangan. Semua data temperatur dan humidity ditampilkan secara real-time melalui serial monitor dan siap dikirim ke platform Blynk.

Dengan pemanfaatan fitur multithreading FreeRTOS pada kedua ESP32, sistem dapat berjalan lebih responsif dan efisien. Smartwatch tidak hanya menampilkan data kesehatan pengguna tetapi juga informasi kondisi lingkungan rumah, menciptakan solusi pemantauan yang terintegrasi dan mudah diakses kapan saja.


## Test results and performance evaluation
## Conclusion and future work
