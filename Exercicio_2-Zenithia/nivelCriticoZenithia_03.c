/**
 * Autor: Marcio Barbosa
 * 
 * Atividade: 
 * 1) Qual era o erro encontrado?
 *  => a. Avalia três locais: "Rua das Flores", "Avenida Central" e "Praça da Liberdade".
 *        falta portanto "Hospital Geral" e "Centro Tecnologico"
 *  => b. Alterar manipulação de variaveis tipo texto, manipulação de inteiros é mais rápida
 *        do que caracteres textos, para microcontroladores
 *  => c. Tornar o código adaptavel para extensões de novos locais com containers, usando estruturas
 * 2) Como você realizou a correção? 
 *  => a. Identificar as locais dos containes com codigos inteiros
 *  => b. Manipular variaveis caracteres textos apenas no momento de retornar nomes dos locais
 *  => c. Incrementar tratameto logico para os tipos definidos, estudo da função for e struct 
 * 3) Qual o comportamento esperado do programa após a correção?
 *  => Não possuo um microcontralador para testes mas acredito que a proposta foi atingida
 *      conforme ajustes
 *  => Todos os locais foram adicionados e tratados
 * 4) Melhorias??
 *  => Utilizei a manipulação de estruturas em C, desafiante
 *  => Pode ser substituida o LocalContainer por uma solução de banco de dados (query)
 */

#include <stdio.h>

// Nivel critico de lixo em que o container precisa ser esvaziado
#define NIVEL_CRITICO_LIXO 80

// Estrutura para os locais
typedef struct {
    int codigo;
    const char *nome;
    int nivelLixo; // Nível de lixo atual
} LocalContainer;

// Vetor de locais com seus respectivos codigos e nomes
LocalContainer locais[] = {
    {1, "Rua das Flores", 0},
    {2, "Avenida Central", 0},
    {3, "Praca da Liberdade", 0},
    {4, "Hospital Geral", 0},
    {5, "Centro Tecnologico", 0}
};

void verificarContainers();                         // verificar o nivel dos containers
void avaliarESolicitarColeta();                     // solicitar coleta
void solicitarColetaAutonoma(const char *local);    // tratamento de coleta autonoma
void coletaEspecial(const char *local);             // tratamento de lixos especiais

// Numero total de locais para execucao do laco, recebe o valor em tempo de execucao => https://en.wikipedia.org/wiki/Sizeof
#define TOTAL_LOCAIS (sizeof(locais) / sizeof(locais[0]))

int main() {
    printf("Sistema de Gestao de Residuos de Zenithia (SGR)\n");

    // Verificar os containers e atualizar niveis de lixo
    verificarContainers();

    // Avaliar os containers e solicitar coletas autonoma e especial
    avaliarESolicitarColeta();
    return 0;
}

// Funcaoo para verificar e atualizar o nivel de lixo de cada container
void verificarContainers() {
    //percorre a struct e retorna os nomes dos locais, pode ser alterado o nivel
    for (int i = 0; i < TOTAL_LOCAIS; i++) {
        switch (locais[i].codigo) {
            case 1: locais[i].nivelLixo = 30; break; // Rua das Flores
            case 2: locais[i].nivelLixo = 80; break; // Avenida Central
            case 3: locais[i].nivelLixo = 100; break; // Praça da Liberdade
            case 4: locais[i].nivelLixo = 0; break; // Hospital Geral
            case 5: locais[i].nivelLixo = 0; break; // Centro Tecnologico
            default: locais[i].nivelLixo = 0; break;
        }
        printf("\nVerificando container em %s...\n", locais[i].nome);
        printf("O container em %s esta com %d%% de lixo.\n", locais[i].nome, locais[i].nivelLixo);
    }
}

// Funcao para avaliar os níveis de lixo e acionar coletas necessarias
void avaliarESolicitarColeta() {

    //percorre a struct e identifica o local e o tipo de coleta
    for (int i = 0; i < TOTAL_LOCAIS; i++) {
        if (locais[i].nivelLixo >= NIVEL_CRITICO_LIXO) {
            printf("\nO container em %s atingiu o nivel critico de lixo.\n", locais[i].nome);
            // Hospital Geral ou Centro Tecnologico pode adicionar outros
            // Poderia ter um tipo na struct para identificar se necessita de tipo especial de coleta (identificador)
            if (locais[i].codigo == 4 || locais[i].codigo == 5) { 
                coletaEspecial(locais[i].nome);
            } else {
                solicitarColetaAutonoma(locais[i].nome);
            }
        } else {
            printf("\nO container em %s ainda nao precisa de coleta.\n", locais[i].nome);
        }
    }
}

// Função para solicitar a coleta automatizada
void solicitarColetaAutonoma(const char *local) {
    printf("Veiculo autonomo a caminho para coletar o lixo em %s.\n", local);
}

// Função para solicitar a coleta especial
void coletaEspecial(const char *local) {
    printf("Solicitando coleta especial para %s.\n", local);
}
