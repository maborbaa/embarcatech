/**
 * Autor: Marcio Barbosa
 * 
 * Atividade: 
 * 1) Qual era o erro encontrado?
 *  => a. Avalia três locais: "Rua das Flores", "Avenida Central" e "Praça da Liberdade".
 *        falta portanto "Hospital Geral" e "Centro Tecnológico"
 *  => b. Alterar manipulação de variaveis tipo texto, manipulação de inteiros é mais rápida
 *        do que caracteres textos, para microcontroladores
 *  => c. Tornar o código adaptavel para extensões de novos locais com containers
 * 2) Como você realizou a correção? 
 *  => a. Identificar as locais dos containes com códigos inteiros
 *  => b. Manipular variaveis caracteres textos apenas no momento de retornar nomes dos locais
 *  => c. Incrementar tratameto logico para os tipos definidos, estudo da função switch - case 
 * 3) Qual o comportamento esperado do programa após a correção?
 *  => Não possuo um microcontralador para testes para acredito que a proposta foi atingida
 *      conforme ajustes
 *  => Todos os locais foram adicionados e tratados
 * 4) Melhorias??
 *  => É possivel manipulação de vetores ou matrizes para manipular os locais, imaginando 
 *      a integração com um banco de dados para o posicionamento dos containes
 */

#include <stdio.h>

// nivel de lixo em que o container precisa ser esvaziado
#define NIVEL_CRITICO_LIXO 80

/*Alterar variaveris texto para utilizar em microcontraldores
    oferece mais velocidade de processamento
    nao utilizar texto para manipulação de variaveis
*/

//Identificar posicionamento dos containers
#define RUA_das_FLORES 1
#define AVENIDA_CENTRAL 2
#define PRACA_LIBERADE 3
#define HOSPITAL_GERAL 4
#define CENTRO_TECNOLOGICO 5

//Identificar nome dos locais
#define ruaFlores    "Rua das Flores"
#define aveCentral   "Avenida Central"
#define praLiberdade "Praca Liberdade"
#define hospGeral    "Hospital Geral"
#define centroTec    "Centro Tecnologico"

void verificarContainer(int local, int *nivelLixo);        // verificar o nivel dos containers
void avaliarESolicitarColeta(int local, int nivelLixo);    // solicitar coleta
void solicitarColetaAutonoma(int local);                   // tratamento de coleta autonoma
void coletaEspecial(int local);                            // tratamento de lixos especiais

int main() {
    printf("Sistema de Gestao de Residuos de Zenithia (SGR)\n");

    // Variáveis para armazenar os níveis de lixo
    int nivelRuaFlores, nivelAvenidaCentral, nivelPracaLiberdade, nivelHospitalGeral, nivelCentroTecnologico;

    // Verificando diferentes locais e obtendo o nivel de lixo
    //incluir Hospital Geral e Centro Tecnologico
    verificarContainer(RUA_das_FLORES, &nivelRuaFlores);
    verificarContainer(AVENIDA_CENTRAL, &nivelAvenidaCentral);
    verificarContainer(PRACA_LIBERADE, &nivelPracaLiberdade);
    verificarContainer(HOSPITAL_GERAL, &nivelHospitalGeral);
    verificarContainer(CENTRO_TECNOLOGICO, &nivelCentroTecnologico);

    // Avaliando e solicitando coleta e se necessário coleta especial
    // incluir Hospital Geral e Centro Tecnologico
    avaliarESolicitarColeta(RUA_das_FLORES, nivelRuaFlores);
    avaliarESolicitarColeta(AVENIDA_CENTRAL, nivelAvenidaCentral);
    avaliarESolicitarColeta(PRACA_LIBERADE, nivelPracaLiberdade);
    avaliarESolicitarColeta(HOSPITAL_GERAL,nivelHospitalGeral);
    avaliarESolicitarColeta(CENTRO_TECNOLOGICO, nivelCentroTecnologico);

    return 0;
}

// Função para verificar o nivel de lixo do container e atribuir o valor à variável fornecida
void verificarContainer(int local, int *nivelLixo) {
   
    // Simulando o nivel de lixo (substituir por um sensor real em um sistema real)
    switch (local)
    {
    case RUA_das_FLORES:
        *nivelLixo = 30; // Exemplo: 30% de lixo
        printf("\nVerificando container em %s...\n", ruaFlores);
        printf("O container em %s esta com %d%% de lixo.\n", ruaFlores, *nivelLixo);
        break;
    case AVENIDA_CENTRAL:
        *nivelLixo = 80; // Exemplo: 80% de lixo
        printf("\nVerificando container em %s...\n", aveCentral);
        printf("O container em %s esta com %d%% de lixo.\n", aveCentral, *nivelLixo);
        break;
    case PRACA_LIBERADE:
        *nivelLixo = 100; // Exemplo: 100% de lixo
        printf("\nVerificando container em %s...\n", praLiberdade);
        printf("O container em %s esta com %d%% de lixo.\n", praLiberdade, *nivelLixo);
        break;
    case HOSPITAL_GERAL:
        *nivelLixo = 0; // Exemplo: 0% de lixo
        printf("\nVerificando container em %s...\n", hospGeral);
        printf("O container em %s esta com %d%% de lixo.\n", hospGeral, *nivelLixo);
        break;
    
    case CENTRO_TECNOLOGICO:
        *nivelLixo = 0; // Exemplo: 0% de lixo
        printf("\nVerificando container em %s...\n", centroTec);
        printf("O container em %s esta com %d%% de lixo.\n", centroTec, *nivelLixo);
        break;
        
    default:
        *nivelLixo = 0; // Valor padrão para outros locais ou caso nao exista
        printf("\nContainer nao localizado.\n");        
        break;
    }
    
}

// Função para avaliar o nivel de lixo e solicitar a coleta especial quando necessário
void avaliarESolicitarColeta(int local, int nivelLixo) {
    
    //Avaliar o nivel critico
    //Se Hospital Geral ou Centro Tecnologico => Solicitar coleta Especial
    if (nivelLixo >= NIVEL_CRITICO_LIXO) {
    
        //Testar Hospital Geral ou Centro Tecnologico => coleta especial   
        switch (local)
        {
        case HOSPITAL_GERAL:
            printf("\nO container em %s atingiu o nivel critico de lixo.\n", hospGeral);
            coletaEspecial(HOSPITAL_GERAL);
            break;
        
        case CENTRO_TECNOLOGICO:
            printf("\nO container em %s atingiu o nivel critico de lixo.\n", centroTec);
            coletaEspecial(CENTRO_TECNOLOGICO);
            break;

        case RUA_das_FLORES:
            printf("\nO container em %s atingiu o nivel critico de lixo.\n", ruaFlores);        
            solicitarColetaAutonoma(RUA_das_FLORES);
            break;
        
        case AVENIDA_CENTRAL:
            printf("\nO container em %s atingiu o nivel critico de lixo.\n", aveCentral);        
            solicitarColetaAutonoma(AVENIDA_CENTRAL);
            break;
        
        case PRACA_LIBERADE:
            printf("\nO container em %s atingiu o nivel critico de lixo.\n", praLiberdade);        
            solicitarColetaAutonoma(PRACA_LIBERADE);
            break;
        
        default:
            printf("\nO container nao identificado.\n");
            break;
        }
      
    } else {
        switch (local)
        {
        case HOSPITAL_GERAL:
            printf("\nO container em %s ainda nao precisa de coleta.\n", hospGeral);
            break;
        
        case CENTRO_TECNOLOGICO:
            printf("\nO container em %s ainda nao precisa de coleta.\n", centroTec);
            break;

        case RUA_das_FLORES:
            printf("\nO container em %s ainda nao precisa de coleta.\n", ruaFlores);
            break;
        
        case AVENIDA_CENTRAL:
            printf("\nO container em %s ainda nao precisa de coleta.\n", aveCentral);
            break;
        
        case PRACA_LIBERADE:
            printf("\nO container em %s ainda nao precisa de coleta.\n", praLiberdade);
            break;
        
        default:
            printf("\nO container nao identificado.\n");
            break;
        }
    }
}

// Função para solicitar a coleta automatizada
void solicitarColetaAutonoma(int local) {
    
    switch (local)
    {
    case 1:
        printf("Veiculo autonomo a caminho para coletar o lixo em %s.\n", ruaFlores);
        break;
    case 2:
        printf("Veiculo autonomo a caminho para coletar o lixo em %s.\n", aveCentral);
        break;
    case 3:
        printf("Veiculo autonomo a caminho para coletar o lixo em %s.\n", praLiberdade);
        break;
    case 4:
        printf("Veiculo autonomo a caminho para coletar o lixo em %s.\n", hospGeral);
        break;
    case 5:
        printf("Veiculo autonomo a caminho para coletar o lixo em %s.\n", centroTec);
        break;
    default:
        printf("Coleta autonoma de tipo desconhecido.\n");
        break;
    }
}

// Função para solicitar a coleta especial
void coletaEspecial(int local) {
   
    // Identifica o tipo de resíduo
    switch (local) {
        case 4:
            printf("Solicitando coleta especial para %s.\n",hospGeral);
            break;
        case 5:
            printf("Solicitando coleta especial para %s.\n",centroTec);
            break;
            
        default:
            printf("\nColeta especial de tipo desconhecido.\n");
            break;
    }
}
