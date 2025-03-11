#include <stdio.h>

// Nível de lixo em que o container precisa ser esvaziado
#define NIVEL_CRITICO_LIXO 80

// Identificadores dos locais
#define RUA_DAS_FLORES       1
#define AVENIDA_CENTRAL      2
#define PRACA_LIBERDADE      3
#define HOSPITAL_GERAL       4
#define CENTRO_TECNOLOGICO   5

// Protótipos das funções
void verificarContainer(int local, int *nivelLixo);
void avaliarESolicitarColeta(int local, int nivelLixo);
void solicitarColetaAutonoma(const char *local);
void coletaEspecial(const char *local);

// Protótipo de função auxiliar para mapear nomes dos locais
const char* obterNomeLocal(int local);

// Função principal
int main() {
    printf("Sistema de Gestão de Resíduos de Zenithia (SGR)\n");

    // Variáveis para armazenar os níveis de lixo
    int nivelRuaFlores, nivelAvenidaCentral, nivelPracaLiberdade, nivelHospitalGeral, nivelCentroTecnologico;

    // Verificação dos containers
    verificarContainer(RUA_DAS_FLORES, &nivelRuaFlores);
    verificarContainer(AVENIDA_CENTRAL, &nivelAvenidaCentral);
    verificarContainer(PRACA_LIBERDADE, &nivelPracaLiberdade);
    verificarContainer(HOSPITAL_GERAL, &nivelHospitalGeral);
    verificarContainer(CENTRO_TECNOLOGICO, &nivelCentroTecnologico);

    // Avaliação e solicitação de coleta
    avaliarESolicitarColeta(RUA_DAS_FLORES, nivelRuaFlores);
    avaliarESolicitarColeta(AVENIDA_CENTRAL, nivelAvenidaCentral);
    avaliarESolicitarColeta(PRACA_LIBERDADE, nivelPracaLiberdade);
    avaliarESolicitarColeta(HOSPITAL_GERAL, nivelHospitalGeral);
    avaliarESolicitarColeta(CENTRO_TECNOLOGICO, nivelCentroTecnologico);

    return 0;
}

// Função para verificar o nível de lixo
void verificarContainer(int local, int *nivelLixo) {
    switch (local) {
        case RUA_DAS_FLORES:       *nivelLixo = 30; break;
        case AVENIDA_CENTRAL:      *nivelLixo = 80; break;
        case PRACA_LIBERDADE:      *nivelLixo = 100; break;
        case HOSPITAL_GERAL:       *nivelLixo = 0; break;
        case CENTRO_TECNOLOGICO:   *nivelLixo = 0; break;
        default:                   *nivelLixo = 0; break;
    }
    printf("Nível de lixo em %s: %d%%\n", obterNomeLocal(local), *nivelLixo);
}

// Função para avaliar e solicitar coleta
void avaliarESolicitarColeta(int local, int nivelLixo) {
    if (nivelLixo >= NIVEL_CRITICO_LIXO) {
        printf("\nO container em %s atingiu nível critico de lixo.\n", obterNomeLocal(local));
        solicitarColetaAutonoma(obterNomeLocal(local));
        if (local == HOSPITAL_GERAL || local == CENTRO_TECNOLOGICO) {
            coletaEspecial(obterNomeLocal(local));
        }
    } else {
        printf("\nO container em %s ainda não precisa de coleta.\n", obterNomeLocal(local));
    }
}

// Função para solicitar coleta automatizada
void solicitarColetaAutonoma(const char *local) {
    printf("Veiculo autonomo a caminho para coletar o lixo em %s.\n", local);
}

// Função para solicitar coleta especial
void coletaEspecial(const char *local) {
    printf("Solicitando coleta especial no local %s.\n", local);
}

// Função auxiliar para mapear nomes dos locais
const char* obterNomeLocal(int local) {
    switch (local) {
        case RUA_DAS_FLORES:       return "Rua das Flores";
        case AVENIDA_CENTRAL:      return "Avenida Central";
        case PRACA_LIBERDADE:      return "Praca da Liberdade";
        case HOSPITAL_GERAL:       return "Hospital Geral";
        case CENTRO_TECNOLOGICO:   return "Centro Tecnologico";
        default:                   return "Local Desconhecido";
    }
}
