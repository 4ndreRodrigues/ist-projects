set.seed(2822)

# Definir os parâmetros
n <- 100
lambda_H0 <- 2.90
lambda_H1 <- 3.15
k <- 3.234
m <- 5000

# Inicializar contadores para os erros
erro_1_especie <- 0
erro_2_especie <- 0

# Simular m pares de amostras
for (i in 1:m) {
  # Amostra sob H0
  amostra_H0 <- rpois(n, lambda_H0)
  media_H0 <- mean(amostra_H0)
  
  # Amostra sob H1
  amostra_H1 <- rpois(n, lambda_H1)
  media_H1 <- mean(amostra_H1)
  
  # Verificar erro de 1ª espécie (H0 verdadeira, mas rejeitada)
  if (media_H0 > k) {
    erro_1_especie <- erro_1_especie + 1
  }
  
  # Verificar erro de 2ª espécie (H1 verdadeira, mas H0 não rejeitada)
  if (media_H1 <= k) {
    erro_2_especie <- erro_2_especie + 1
  }
}

# Calcular frequências relativas dos erros
prob_erro_1_especie <- erro_1_especie / m
prob_erro_2_especie <- erro_2_especie / m

# Calcular o quociente entre as probabilidades de erro de 2ª e 1ª espécie
quociente_erros <- prob_erro_2_especie / prob_erro_1_especie

# Imprimir o resultado arredondado
print(round(quociente_erros, 2))