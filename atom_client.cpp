//gerekli kütüphaneler
#include <WinSock2.h>
#include <iostream>
using namespace std;

#pragma comment(lib,"ws2_32.lib") // Winsock kütüphanesinin baðlanmasýný saðlar.
#pragma warning(disable:4996) //bazý uyarýlarý devre dýþý býrakýr.

#define SERVER "127.0.0.1"  // ýp adresi
#define BUFLEN 1024  //veri tamponu boyutu
#define PORT 6161  //port numarasi istenilen gibi secilebilir. 

int main() {
    system("title UDP Client"); // konsolun basligi UDP Client olarak degistirir.

    WSADATA wsaData;
    
    int iResult;

    // winsock kutuphanesi baslatilir.
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }
    //soket olusturma 
    sockaddr_in server; 
    SOCKET atom_client;
    atom_client = socket(AF_INET, SOCK_DGRAM, 0);

    if (atom_client == INVALID_SOCKET) {
        printf("soket olusturulamadi. Hata kodu: %d", WSAGetLastError());
    }
    printf("soket olusturuldu\n");

    //server ayarlari
    memset((char*)&server, 0, sizeof(server)); //server yapilarini ait bellek sifirlanir
    server.sin_addr.S_un.S_addr = inet_addr(SERVER);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    bool running = true;
    // sunucunun döngüde olmasýný saglar
    while (running) {
        char message[BUFLEN] = {};
        int message_len;
        printf("Client:");

        //gönderilecek mesaj kullanicidan alinir.
        cin.getline(message, BUFLEN);

        // eger cikis yapilmak istenirse 'q' yollanir. 
        if (strcmp(message, "q") == 0)
        {
            running = false;
            continue;
        }
        //kullanicidan alinan mesaj gonderilir.
        if (sendto(atom_client, message, strlen(message), 0, (sockaddr*)&server, sizeof(sockaddr_in)) == SOCKET_ERROR)
        {
            printf("mesaj gonderilemedi. Hata kodu: %d", WSAGetLastError());
            return 3;
        }
        // Olurda bir cevap gelirse answer temizlenir. ve gelen cevap answer tamponunda tutulur.
        char answer[BUFLEN] = {};

        // gelen cevap recvfrom ile alinir. yanit gelene kadar program burada bekler.
        int answer_length;
        int slen = sizeof(sockaddr_in);
        if (answer_length = recvfrom(atom_client, answer, BUFLEN, 0, (sockaddr*)&server, &slen) == SOCKET_ERROR)
        {
            printf("mesaj alinamadi. Hata kodu: %d", WSAGetLastError());
            exit(0);
        }
        //gelen cevap ekrana yazilir
        cout << "Server: " << answer << "\n";
    }
    // socket kapatilir ve Winsock temizlenir.
    WSACleanup();
    closesocket(atom_client);
    return 0;
}