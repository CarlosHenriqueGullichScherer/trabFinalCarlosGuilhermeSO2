#include <iostream>
#include <string>
#include "contigua.hpp"
#include "memoria.hpp"

using namespace std;

void menuPrincipal() {
    int escolha = 0;

    do { // menu com do while para que ele seja executado pelo menos uma vez
        cout << "\n===== MENU PRINCIPAL =====" << endl;
        cout << "1. Alocacao Contigua" << endl;
        cout << "2. Paginacao" << endl;
        cout << "3. Sair" << endl;
        cout << "Escolha: ";
        cin >> escolha;

        switch (escolha) {
            case 1:
                contigua::menuContigua();
                break;
            case 2:
                gerenMemoria::menuPaginacao();
                break;
            case 3:
                break;
            default:
                cout << "Opcao invalida! Tente novamente." << endl;
        }
    } while (escolha != 3);
}

int main() {
    menuPrincipal();
    return 0;
}