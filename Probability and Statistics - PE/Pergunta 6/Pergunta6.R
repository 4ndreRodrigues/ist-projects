# Fixar a semente
set.seed(1973)

# Parâmetros
n <- 40
a <- 4
num_amostras <- 1000

# Gerar amostras
amostras <- matrix(rexp(n * num_amostras, rate = 1 / a), nrow = num_amostras)

# Calcular Y para cada amostra
Ys <- rowSums(amostras)

# Calcular a proporção de Ys > 126
proporcao_simulada <- mean(Ys > 126)
proporcao_simulada


# Carregar o pacote necessário
if (!requireNamespace("stats", quietly = TRUE)) {
  install.packages("stats")
}
library(stats)

# Parâmetros da distribuição gama
alpha <- 40
beta <- 4

# Calcular a probabilidade P(Y > 126)
valor_exato <- 1 - pgamma(126, shape = alpha, scale = beta)
valor_exato


# Calcular a diferença absoluta multiplicada por 100 e com 4 casas decimais
diferenca <- abs(proporcao_simulada - valor_exato) * 100
diferenca_arredondada <- round(diferenca, 4)
diferenca_arredondada