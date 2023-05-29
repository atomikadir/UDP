//gerekli kütüphaneler
#include <winSock2.h>
#include <iostream>
using namespace std;

#pragma comment(lib,"ws2_32.lib") // Winsock kütüphanesinin baðlanmasýný saðlar.
#pragma warning(disable:4996) //bazý uyarýlarý devre dýþý býrakýr.

#define BUFLEN 1024 // veri tamponu boyutu
#define PORT 6161 //port numarasi istenilen gibi secilebilir.

int main() {	
	system("title UDP Server"); // konsolun basligi UDP Server olarak degistirir.

	WSADATA wsaData;

	int iResult;

	// winsock kutuphanesi baslatilir.
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}
	//soket olusturma
	SOCKET atom_server;
	atom_server = socket(AF_INET, SOCK_DGRAM, 0);

	if (atom_server == INVALID_SOCKET) {
		printf("soket olusturulamadi. Hata kodu: %d", WSAGetLastError);
	}
	printf("soket olusturuldu\n");

	//server ayarlari
	sockaddr_in server,client; //socket adresini temsil etmek icin sockaddr_in yapýsýnda 1 tane yapýsal degisken
	server.sin_addr.s_addr = 0;
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	
	//bind fonksiyonu --> olusturulan soket belirlenen adres ve porta baglanir
	if (bind(atom_server, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
		printf("baglanamadi. Hata kodu:%d", WSAGetLastError);
		exit(EXIT_FAILURE);
	}
	bool running = true;
	// sunucunun sürekli veri almasýný saglar. recvfrom gelen veriyi bekler ve message tamponunda tutar.
	while (running) {
		char message[BUFLEN] = {};
		int message_len;
		int slen = sizeof(sockaddr_in);
		// gelen cevap recvfrom ile alinir. yanit gelene kadar program burada bekler
		if (message_len = recvfrom(atom_server, message, BUFLEN, 0, (sockaddr*)&client, &slen) == SOCKET_ERROR) {
			printf("mesaj alinamadi. Hata kodu: %d", WSAGetLastError());
			exit(0);
		}
		printf("Client: %s\n", message);
		printf("Server:");

		// gönderilecek mesaj kullanicidan alinir.
		cin.getline(message, BUFLEN);

		// eger cikis yapilmak istenirse 'q' yollanir. 
		if (strcmp(message, "bayso") == 0)
		{
			running = false;
			continue;
		}

		//kullanicidan alinan mesaj gönderilir.
		if (sendto(atom_server, message, strlen(message), 0, (sockaddr*)&client, sizeof(sockaddr_in)) == SOCKET_ERROR) {
			printf("mesah gonderilemedi. Hata kodu: %d", WSAGetLastError());
			return 3;
		}

	}
	// socket kapatilir ve Winsock temizlenir.
	WSACleanup();
	closesocket(atom_server);
	return 0;
}

