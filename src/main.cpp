#include <iostream>
#include "ProgramInterpreter.hh"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Uzycie: " << argv[0] << " plik_konfiguracyjny.xml" << endl;
        return 1;
    }

    ProgramInterpreter Interpreter;

    Interpreter.InitGraphic();

    if (!Interpreter.ReadConfig(argv[1])) {
        cerr << "Blad: Nie udalo sie przetworzyc pliku XML." << endl;
        return 2;
    }

    Interpreter.Wait();

    return 0;
}