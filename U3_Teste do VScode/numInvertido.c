/* Programa que lê um numero inteiro positivo de três dígitos (100 a 999) e retorna outro número inteiro positivo invertendo os dígitos do número, exemplo 123 e retorna 321*/

#include <stdio.h>

//Função que inverte um número inteiro positivo
int inverterNumero (int numero){
    int invertido = 0, resto;

    //le o numero inteiro e separa os digitos
    while (numero != 0)
    {
        /* code */
        resto = numero % 10;                //obtem o ultimo digito
        invertido = invertido * 10 + resto; // adiciona o digito para inversão -> invertido
        numero /= 10;                       // remove o ultimo digito
    }
    return invertido;    

}

int main (){
    int numero, numeroInvertido;
    
    //Solicita a entrada para o usuario
    printf("Digite um número de três dígitos positivos (100 - 999): ");
    scanf("%d", &numero);

    //testar se entrada válida
    //sim executado a inversão
    //não retorna uma mensagem para o usuario
    if (numero >= 100 && numero <= 999)
    {
        // inverte e retorna para o usuario
        numeroInvertido = inverterNumero(numero);
        printf("O número digitado é: %d\n", numero);
        printf("O número invertido é: %d\n", numeroInvertido);
    }else{ // erro retorna mensagem
        printf("Número inválido: %d não esta entre 100 e 999\n", numero);
        return 1;
    }
    return 0;
}