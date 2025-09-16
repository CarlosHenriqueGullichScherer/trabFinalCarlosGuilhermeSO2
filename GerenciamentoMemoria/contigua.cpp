#include <algorithm> //sort()
#include <iomanip>   //setprecicison()
#include <iostream>
#include <vector> //vetor representa a memoria
#include <map>    //map representa tabela de processos
#include "contigua.hpp"

namespace contigua
{
    // variaveis internas
    static std::vector<int> memoria; // static para evitar conflito e garantir usabilidade em todo o programa
    static std::map<int, Processo> tabela;
    static int ultimoId = 1; // contador de IDs de processo

    void inicMemoria(int n)
    {
        if (n <= 0)
        {
            n = 50;
        }
        memoria.assign(n, -1);
        tabela.clear();
        std::cout << "Memoria incializada com " << n << " unidades.\n";
    }

    void mostrarMemoria()
    {
        if (memoria.empty())
        {
            std::cout << "Memoria não inicializada\n";
            return;
        }
        std::cout << "Memoria (" << memoria.size() << " unidades):\n";
        for (size_t i = 0; i < memoria.size(); i++)
        {
            if (memoria[i] == -1)
            {
                std::cout << "[-]";
            }
            else
            {
                std::cout << "[P" << memoria[i] << "]";
            }
        }
        std::cout << "\n";

        double frag = calcularFragmentacao();
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Fragmentacao externa: " << frag << "%\n";
    }

    void mostrarTabela()
    {
        std::cout << "ID\tBase\tLimite\tTamanho\n";
        if (tabela.empty())
        {
            std::cout << "Tabela vazia!\n";
            return;
        }
        for (auto &entrada : tabela)
        {
            const Processo &p = entrada.second;
            std::cout << "P" << p.id << "\t" << p.base << "\t" << p.limite << "\t" << p.tamanho << "\n";
        }
    }

    void processo_FirstFit(int tamanho)
    {
        int livre = 0;   // contador de posicoes livres
        int inicio = -1; // posicao onde comeca o bloco livre
        for (int i = 0; i < (int)memoria.size(); i++)
        {
            if (memoria[i] == -1)
            {
                if (inicio == -1)
                {
                    inicio = i;
                }
                livre++;

                if (livre == tamanho)
                {
                    Processo p;
                    p.id = ultimoId++;
                    p.base = inicio;
                    p.limite = inicio + tamanho - 1;
                    p.tamanho = tamanho;

                    tabela[p.id] = p;

                    for (int j = inicio; j <= p.limite; j++)
                    {
                        memoria[j] = p.id;
                    }

                    std::cout << "Processo P" << p.id << " criado. (Tamanho: " << p.tamanho << ", base: " << p.base << ", limite: " << p.limite << ")\n";
                    return;
                }
            }
            else
            {
                inicio = -1;
                livre = 0;
            }
        }
        std::cout << "Nao foi possivel alocar processo de tamanho: " << tamanho << " (sem espaco contiguo suficiente)\n";
    }

    void processo_BestFit(int tamanho)
    {
        int melhorInicio = -1;
        int melhorTamanho = memoria.size() + 1;
        int inicio = -1;
        int livre = 0;

        for (int i = 0; i <= (int)memoria.size(); i++)
        {
            if (i < (int)memoria.size() && memoria[i] == -1)
            {
                if (inicio == -1)
                {
                    inicio = i;
                }
                livre++;
            }
            else
            {
                if (livre >= tamanho && livre < melhorTamanho)
                {
                    melhorInicio = inicio;
                    melhorTamanho = livre;
                }
                inicio = -1;
                livre = 0;
            }
        }

        if (melhorInicio != -1)
        {
            Processo p;
            p.id = ultimoId++;
            p.base = melhorInicio;
            p.limite = melhorInicio + tamanho - 1;
            p.tamanho = tamanho;

            tabela[p.id] = p;

            for (int j = p.base; j <= p.limite; j++)
            {
                memoria[j] = p.id;
            }

            std::cout << "Processo P" << p.id << " criado (Best-Fit). " << "(Tamanho: " << p.tamanho << ", base: " << p.base << ", limite: " << p.limite << ")\n";
        }
        else
        {
            std::cout << "Nao foi possivel alocar processo de tamanho: " << tamanho << " (sem espaco contiguo suficiente)\n";
        }
    }

    void processo_WorstFit(int tamanho)
    {
        int piorInicio = -1;
        int piorTamanho = -1;
        int livre = 0;
        int inicio = -1;

        for (int i = 0; i <= (int)memoria.size(); i++)
        {
            if (i < (int)memoria.size() && memoria[i] == -1)
            {
                if (inicio == -1)
                {
                    inicio = i;
                }
                livre++;
            }
            else
            {
                if (livre >= tamanho && livre > piorTamanho)
                {
                    piorInicio = inicio;
                    piorTamanho = livre;
                }
                inicio = -1;
                livre = 0;
            }
        }

        if (piorInicio != -1)
        {
            Processo p;
            p.id = ultimoId++;
            p.base = piorInicio;
            p.limite = piorInicio + tamanho - 1;
            p.tamanho = tamanho;

            tabela[p.id] = p;

            for (int j = p.base; j <= p.limite; j++)
            {
                memoria[j] = p.id;
            }

            std::cout << "Processo P" << p.id << " criado (Worst-Fit). " << "(Tamanho: " << p.tamanho << ", base: " << p.base << ", limite: " << p.limite << ")\n";
        }
        else
        {
            std::cout << "Nao foi possivel alocar processo de tamanho: " << tamanho << " (sem espaco contiguo suficiente)\n";
        }
    }

    void processo_CircularFit(int tamanho)
    {
        static int ultimaPos = 0;
        int livre = 0;
        int pos = -1;
        int i = ultimaPos;
        int percorrido = 0;

        while (percorrido < (int)memoria.size())
        {
            if (memoria[i] == -1)
            {
                if (livre == 0)
                {
                    pos = i;
                }
                livre++;

                if (livre == tamanho)
                {
                    ultimaPos = (i + 1) % memoria.size();

                    Processo p;
                    p.id = ultimoId++;
                    p.base = pos;
                    p.limite = pos + tamanho - 1;
                    p.tamanho = tamanho;
                    tabela[p.id] = p;

                    for (int j = p.base; j <= p.limite; j++)
                    {
                        memoria[j] = p.id;
                    }

                    std::cout << "Processo P" << p.id << " criado (Circular-Fit). (Tamanho: " << p.tamanho << ", base: " << p.base << ", limite: " << p.limite << ")\n";
                    return;
                }
            }
            else
            {
                livre = 0;
                pos = -1;
            }

            i = (i + 1) % memoria.size();
            percorrido++;
        }

        std::cout << "Nao foi possivel alocar processo de tamanho: " << tamanho << " (sem espaco contiguo suficiente)\n";
    }

    double calcularFragmentacao()
    {
        int livreTotal = 0;
        int maiorBloco = 0;
        int atual = 0;

        for (int v : memoria)
        {
            if (v == -1)
            {
                atual++;
                livreTotal++;
            }
            else
            {
                if (atual > maiorBloco)
                {
                    maiorBloco = atual;
                }
                atual = 0;
            }
        }
        if (atual > maiorBloco)
        {
            maiorBloco = atual;
        }

        if (livreTotal == 0)
        {
            return 0.0;
        }

        return 100.0 * (livreTotal - maiorBloco) / (double)livreTotal;
    }

    void removerProcessoContigua(int id)
    {
        auto it = tabela.find(id);
        if (it == tabela.end())
        {
            std::cout << "Processo P" << id << " nao encontrado!\n";
            return;
        }

        Processo p = it->second;

        for (int i = p.base; i <= p.limite; i++)
        {
            memoria[i] = -1;
        }

        tabela.erase(it);

        std::cout << "Processo P" << id << " removido com sucesso!\n";
    }

    void realocarMemoria()
    {
        std::vector<Processo> processos;
        for (const auto &kv : tabela)
        {
            processos.push_back(kv.second);
        }

        std::sort(processos.begin(), processos.end(), [](const Processo &a, const Processo &b)
                  { return a.base < b.base; });

        int posLivre = 0;
        for (auto &proc : processos)
        {
            for (int i = proc.base; i <= proc.limite; i++)
            {
                memoria[i] = -1;
            }

            proc.base = posLivre;
            proc.limite = posLivre + proc.tamanho - 1;

            tabela[proc.id] = proc;

            for (int i = proc.base; i <= proc.limite; i++)
            {
                memoria[i] = proc.id;
            }

            posLivre = proc.limite + 1;
        }
        std::cout << "Memoria realocada!\n";
    }
}

int main()
{
    return 0;
}

// rodar: cd GerenciamentoMemoria
// g++ contigua.cpp -o simulador
// ./simulador
