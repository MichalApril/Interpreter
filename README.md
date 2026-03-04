# Interpreter Poleceń Robota

Projekt realizujący wielowątkowy interpreter poleceń wczytywanych z pliku XML, sterujący obiektami na scenie. Projekt korzysta z dynamicznie ładowanych bibliotek (wtyczek) oraz narzędzi Autotools do budowania pakietu.

## Prawa Autorskie

Część kodu źródłowego (szkielet architektury, interfejsy bazowe oraz narzędzia pomocnicze) została udostępniona przez dr.inż. Bogdana Kreczmera w ramach kursu na Politechnice i stanowi jego własność intelektualną. Moim zadaniem w ramach projektu była implementacja:
* Wielowątkowośći i synchronizacji.
* Parsera plików XML z użyciem biblioteki Xerces-C.
* Wtyczek realizujących ruch, obrót i pauzę obiektów geometrycznych.
* Konfiguracji i budowy pakietu instalacyjnego przy użyciu Autotools.

## Wymagania systemowe

Aby skompilować i uruchomić projekt, potrzebujesz systemu Linux z zainstalowanymi pakietami:

* g++ (z obsługą standardu C++17)
* libxerces-c-dev
* autoconf, automake, libtool
* make

## Budowanie projektu

Pobierz repozytorium, a następnie wygeneruj skrypty budujące i skompiluj projekt:

autoreconf -fvi
./configure
make

## Uruchamianie

Po poprawnej kompilacji upewnij się, że biblioteki dynamiczne (.so) znajdują się w odpowiednim katalogu i uruchom program, podając plik konfiguracyjny XML:

export LD_LIBRARY_PATH="./src-lib/.libs"
./src/interp4cmds config.xml

## Budowa paczki instalacyjnej (distcheck)

make distcheck

## Architektura i Działanie

Aplikacja działa jako wielowątkowy klient TCP/IP. Jej głównym zadaniem jest:
1. Wczytanie scenariusza z pliku XML.
2. Dynamiczne załadowanie odpowiednich bibliotek (.so) za pomocą `dlopen`.
3. Równoległe przetwarzanie komend w osobnych wątkach.
4. Synchronizacja zasobów (mutex) i wysyłanie instrukcji ruchu do zewnętrznego serwera graficznego.

W przypadku braku serwera graficznego, program demonstruje poprawne działanie wielowątkowości oraz wtyczek poprzez logi wypisywane bezpośrednio w konsoli.
