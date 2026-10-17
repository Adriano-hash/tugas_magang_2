Nama Repo: tugas_magang_2
Author: Adriano Laurensius Anggrek
NRP: 5025261079

Penjelasan Kode
Di bagian source code terdapat 3 file .cpp
1) pose_listener.cpp yang berupa node subscriber sederhana untuk pentagram.cpp menggunakan topic "turtle1/pose" yang berfungsi sebagai logger.
2) pentagram.cpp yang berupa node publisher untuk menggambar sebuah pentagram atau bintang bersudut lima, menggunakan topic "turtle1/cmd_vel"
   pentagram.cpp juga memiliki parameter scalar yang bisa diubah di terminal untuk mengubah ukuran pentagram.
4) pentagram2.cpp berupa node publisher-subscriber yang juga menggambar sebuah pentagram tetapi disertai proportional control sederhana dan
   alur program yang lebih terpadu dengan enumeration, pentagram2.cpp dapat menggambar dengan lebih akurat dibanding pentagram.cpp
   meski dengan pergerakan yang lebih lambat. pentagram2.cpp juga disertai parameter size untuk mengatur ukuran gambar dan angular_scalar untuk
   mengubah kecepatan putaran di turtlesim.

link video penjelasan : https://youtu.be/M4smsLM7eGU

