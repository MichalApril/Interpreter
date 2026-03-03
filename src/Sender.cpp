#include "Sender.hh"
#include "Port.hh"
#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <mutex>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

Sender::~Sender() {
    CloseConnection();
}

void Sender::OpenConnection() {
    struct sockaddr_in DaneAdSerw;
    bzero((char *)&DaneAdSerw, sizeof(DaneAdSerw));
    DaneAdSerw.sin_family = AF_INET;
    DaneAdSerw.sin_addr.s_addr = inet_addr("127.0.0.1");
    DaneAdSerw.sin_port = htons(PORT);

    _Socket = socket(AF_INET, SOCK_STREAM, 0);

    if (_Socket < 0) {
       
        throw std::runtime_error("Socket nie działa");
    }

   
    if (connect(_Socket, (struct sockaddr*)&DaneAdSerw, sizeof(DaneAdSerw)) < 0) {
        close(_Socket);
        _Socket = 0;
        
        
        throw std::runtime_error("Nie mogę połączyć się z serwerem graficznym na porcie " + std::to_string(PORT));
    }
}


void Sender::Send(const string& message) {
    cout << "DEBUG: [" << message << "]" << endl;
    

    if (_Socket == 0) {
        cerr << "Brak polaczenia z serwerem." << endl;
        return;
    }

    const char* sMesg = message.c_str();
    ssize_t IlWyslanych;
    ssize_t IlDoWyslania = (ssize_t) message.length();

    while ((IlWyslanych = write(_Socket, sMesg, IlDoWyslania)) > 0) {
        IlDoWyslania -= IlWyslanych;
        sMesg += IlWyslanych;
    }
}

void Sender::CloseConnection() {
    if (_Socket != 0) 
    {
        string closeCmd = "Close\n"; 
        write(_Socket, closeCmd.c_str(), closeCmd.length());
        
        close(_Socket);
        _Socket = 0;
    }
}
void Sender::Init(int Socket) {
    _Socket = Socket;
}

int Sender::GetSocket() const {
    return _Socket;
}

void Sender::LockAccess() {
    _Mutex.lock();
}

void Sender::UnlockAccess() {
    _Mutex.unlock();
}
std::mutex & Sender::UseGuard() {
    return _Mutex;
}