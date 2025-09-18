#include <algorithm> //sort()
#include <iomanip>   //setprecicison(), setw()
#include <iostream>
#include <map>    //map representa tabela de processos
#include <string> //trabalhar com entradas em string
#include <vector> //vetor representa a memoria
#include "contigua.hpp"

namespace contigua {
    // variaveis internas
    static std::vector<int> memoria; // static para evitar conflito e garantir usabilidade em todo o programa
    static std::map<int, Processo> tabela;
    static int ultimoId = 1; // contador de IDs de processo

    bool ehValido(const std::string &s) {
        if (s.empty()) return true;
        if (s[0] == '-') return true;
        for (char c : s) {
            if (!isdigit(c)) return true;
        }

        return false;
    }

    void inicMemoria(int n) {
        if (n <= 0) {
            n = 50;
        }
        memoria.assign(n, -1);
        tabela.clear();
        std::cout << "Memoria incializada com " << n << " unidades.\n";
    }

    void mostrarMemoria() {
        if (memoria.empty()) {
            std::cout << "Memoria não inicializada\n";
            return;
        }
        std::cout << "Memoria (" << memoria.size() << " unidades):\n";
        int contador=0;
        for (size_t i = 0; i < memoria.size(); i++) {
            if (memoria[i] == -1) {
                std::cout << std::setw(5) << "[-]";
            } else {
                std::string bloco = "P" + std::to_string(memoria[i]);
                std::cout << std::setw(5) << ("[" + bloco + "]");
            }
            contador++;
            if (contador == 20) {
                std::cout << "\n";
                contador = 0;
            }
        }
        std::cout << "\n";
    }

    void mostrarTabela() {
        std::cout << "ID\tBase\tLimite\tTamanho\n";
        if (tabela.empty()) {
            std::cout << "Tabela vazia!\n";
            return;
        }
        for (auto &entrada : tabela) {
            const Processo &p = entrada.second;
            std::cout << "P" << p.id << "\t" << p.base << "\t" << p.limite << "\t" << p.tamanho << "\n";
        }
    }

    void processo_FirstFit(int tamanho) {
        int livre = 0;   // contador de posicoes livres
        int inicio = -1; // posicao onde comeca o bloco livre
        for (int i = 0; i < (int)memoria.size(); i++) {
            if (memoria[i] == -1) {
                if (inicio == -1) {
                    inicio = i;
                }
                livre++;

                if (livre == tamanho) {
                    Processo p;
                    p.id = ultimoId++;
                    p.base = inicio;
                    p.limite = inicio + tamanho - 1;
                    p.tamanho = tamanho;

                    tabela[p.id] = p;

                    for (int j = inicio; j <= p.limite; j++) {
                        memoria[j] = p.id;
                    }

                    std::cout << "Processo P" << p.id << " criado. (Tamanho: " << p.tamanho << ", base: " << p.base << ", limite: " << p.limite << ")\n";
                    return;
                }
            } else {
                inicio = -1;
                livre = 0;
            }
        }
        std::cout << "Nao foi possivel alocar processo de tamanho: " << tamanho << " (sem espaco contiguo suficiente)\n";
    }

    void processo_BestFit(int tamanho) {
        int melhorInicio = -1;
        int melhorTamanho = memoria.size() + 1;
        int inicio = -1;
        int livre = 0;

        for (int i = 0; i <= (int)memoria.size(); i++) {
            if (i < (int)memoria.size() && memoria[i] == -1) {
                if (inicio == -1) {
                    inicio = i;
                }
                livre++;
            } else {
                if (livre >= tamanho && livre < melhorTamanho) {
                    melhorInicio = inicio;
                    melhorTamanho = livre;
                }
                inicio = -1;
                livre = 0;
            }
        }

        if (melhorInicio != -1) {
            Processo p;
            p.id = ultimoId++;
            p.base = melhorInicio;
            p.limite = melhorInicio + tamanho - 1;
            p.tamanho = tamanho;

            tabela[p.id] = p;

            for (int j = p.base; j <= p.limite; j++) {
                memoria[j] = p.id;
            }

            std::cout << "Processo P" << p.id << " criado (Best-Fit). " << "(Tamanho: " << p.tamanho << ", base: " << p.base << ", limite: " << p.limite << ")\n";
        } else {
            std::cout << "Nao foi possivel alocar processo de tamanho: " << tamanho << " (sem espaco contiguo suficiente)\n";
        }
    }

    void processo_WorstFit(int tamanho) {
        int piorInicio = -1;
        int piorTamanho = -1;
        int livre = 0;
        int inicio = -1;

        for (int i = 0; i <= (int)memoria.size(); i++) {
            if (i < (int)memoria.size() && memoria[i] == -1) {
                if (inicio == -1) {
                    inicio = i;
                }
                livre++;
            } else {
                if (livre >= tamanho && livre > piorTamanho) {
                    piorInicio = inicio;
                    piorTamanho = livre;
                }
                inicio = -1;
                livre = 0;
            }
        }

        if (piorInicio != -1) {
            Processo p;
            p.id = ultimoId++;
            p.base = piorInicio;
            p.limite = piorInicio + tamanho - 1;
            p.tamanho = tamanho;

            tabela[p.id] = p;

            for (int j = p.base; j <= p.limite; j++) {
                memoria[j] = p.id;
            }

            std::cout << "Processo P" << p.id << " criado (Worst-Fit). " << "(Tamanho: " << p.tamanho << ", base: " << p.base << ", limite: " << p.limite << ")\n";
        } else {
            std::cout << "Nao foi possivel alocar processo de tamanho: " << tamanho << " (sem espaco contiguo suficiente)\n";
        }
    }

    void processo_CircularFit(int tamanho) {
        static int ultimaPos = 0;
        int livre = 0;
        int pos = -1;
        int i = ultimaPos;
        int percorrido = 0;

        while (percorrido < (int)memoria.size()) {
            if (memoria[i] == -1) {
                if (livre == 0) {
                    pos = i;
                }
                livre++;

                if (livre == tamanho) {
                    ultimaPos = (i + 1) % memoria.size();

                    Processo p;
                    p.id = ultimoId++;
                    p.base = pos;
                    p.limite = pos + tamanho - 1;
                    p.tamanho = tamanho;
                    tabela[p.id] = p;

                    for (int j = p.base; j <= p.limite; j++) {
                        memoria[j] = p.id;
                    }

                    std::cout << "Processo P" << p.id << " criado (Circular-Fit). (Tamanho: " << p.tamanho << ", base: " << p.base << ", limite: " << p.limite << ")\n";
                    return;
                }
            } else {
                livre = 0;
                pos = -1;
            }

            i = (i + 1) % memoria.size();
            percorrido++;
        }

        std::cout << "Nao foi possivel alocar processo de tamanho: " << tamanho << " (sem espaco contiguo suficiente)\n";
    }

    double calcularFragmentacaoExterna() {
        int livreTotal = 0;
        int maiorBloco = 0;
        int atual = 0;

        for (int v : memoria) {
            if (v == -1) {
                atual++;
                livreTotal++;
            } else {
                if (atual > maiorBloco)
                {
                    maiorBloco = atual;
                }
                atual = 0;
            }
        }

        if (atual > maiorBloco) {
            maiorBloco = atual;
        }

        if (livreTotal == 0) {
            return 0.0;
        }

        return 100.0 * (livreTotal - maiorBloco) / (double)livreTotal;
    }

    void removerProcessoContigua(int id) {
        auto it = tabela.find(id);
        if (it == tabela.end()) {
            std::cout << "Processo P" << id << " nao encontrado!\n";
            return;
        }

        Processo p = it->second;

        for (int i = p.base; i <= p.limite; i++) {
            memoria[i] = -1;
        }

        tabela.erase(it);

        std::cout << "Processo P" << id << " removido com sucesso!\n";
    }

    void realocarMemoria() {
        std::vector<Processo> processos;
        for (const auto &kv : tabela) {
            processos.push_back(kv.second);
        }

        std::sort(processos.begin(), processos.end(), [](const Processo &a, const Processo &b){ return a.base < b.base; });

        int posLivre = 0;
        for (auto &proc : processos) {
            for (int i = proc.base; i <= proc.limite; i++) {
                memoria[i] = -1;
            }

            proc.base = posLivre;
            proc.limite = posLivre + proc.tamanho - 1;

            tabela[proc.id] = proc;

            for (int i = proc.base; i <= proc.limite; i++) {
                memoria[i] = proc.id;
            }

            posLivre = proc.limite + 1;
        }
        std::cout << "Memoria realocada!\n";
    }

    void menuContigua() {
    bool sair = false;

    while (!sair) {
        std::cout << "Voce esta no menu da Alocacao Contigua!" << std::endl;
        std::cout << "Digite o tamanho da memoria (em unidades): ";

        std::string entrada;
        std::getline(std::cin, entrada);
        while (ehValido(entrada)) {
            std::cout << "Digite um valor valido: ";
            std::getline(std::cin, entrada);
        }

        int tamanhoMemoria = std::stoi(entrada);
        inicMemoria(tamanhoMemoria);

        bool reiniciar = false;
        while (!reiniciar && !sair) {
            std::cout << "\nSelecione uma das opcoes abaixo:\n";
            std::cout << "1. Criar processo (First-Fit)\n";
            std::cout << "2. Criar processo (Best-Fit)\n";
            std::cout << "3. Criar processo (Worst-Fit)\n";
            std::cout << "4. Criar processo (Circular-Fit)\n";
            std::cout << "5. Remover processo\n";
            std::cout << "6. Ver memoria\n";
            std::cout << "7. Ver tabela de processos\n";
            std::cout << "8. Calcular fragmentacao\n";
            std::cout << "9. Realocar memoria\n";
            std::cout << "10. Reiniciar simulacao\n";
            std::cout << "11. Sair\n";
            std::cout << "Escolha: ";

            std::getline(std::cin, entrada);
            if (ehValido(entrada)) continue;

            int opcao = std::stoi(entrada);

            switch (opcao) {
                case 1: {
                    std::cout << "Digite o tamanho do processo: ";
                    std::getline(std::cin, entrada);
                    if (!ehValido(entrada))
                        processo_FirstFit(std::stoi(entrada));
                    break;
                }
                case 2: {
                    std::cout << "Digite o tamanho do processo: ";
                    std::getline(std::cin, entrada);
                    if (!ehValido(entrada))
                        processo_BestFit(std::stoi(entrada));
                    break;
                }
                case 3: {
                    std::cout << "Digite o tamanho do processo: ";
                    std::getline(std::cin, entrada);
                    if (!ehValido(entrada))
                        processo_WorstFit(std::stoi(entrada));
                    break;
                }
                case 4: {
                    std::cout << "Digite o tamanho do processo: ";
                    std::getline(std::cin, entrada);
                    if (!ehValido(entrada))
                        processo_CircularFit(std::stoi(entrada));
                    break;
                }
                case 5:
                    std::cout << "Digite o ID do processo para remover: ";
                    std::getline(std::cin, entrada);
                    if (!ehValido(entrada))
                        removerProcessoContigua(std::stoi(entrada));
                    break;
                case 6:
                    mostrarMemoria();
                    break;
                case 7:
                    mostrarTabela();
                    break;
                case 8: {
                    double frag = calcularFragmentacaoExterna();
                    std::cout << std::fixed << std::setprecision(2);
                    std::cout << "Fragmentacao externa: " << frag << "%\n";
                    break;
                }
                case 9:
                    realocarMemoria();
                    break;
                case 10:
                    reiniciar = true; // volta ao inicio da simulacao
                    break;
                case 11:
                    sair = true; // encerra tudo
                    break;
                default:
                    std::cout << "Opcao invalida!\n";
                    break;
                }
            }
        }
    }
}