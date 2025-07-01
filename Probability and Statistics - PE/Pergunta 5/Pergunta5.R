set.seed(1950)

# Parâmetros
n <- 23
r <- 300
m <- 170
threshold <- 1.5

# Função para calcular T
calculate_T <- function() {
  Z <- rnorm(n + 1)
  Z1 <- Z[1]
  Z2_to_nplus1 <- Z[-1]
  T <- sqrt(n) * Z1 / sqrt(sum(Z2_to_nplus1^2))
  return(T)
}

set.seed(1950)

# Gerar amostras e calcular proporções
proportions <- numeric(r)
for (i in 1:r) {
  T_values <- replicate(m, calculate_T())
  proportions[i] <- mean(T_values <= threshold)
}

# Aproximação empírica de p
p_empirical <- mean(proportions)

# Probabilidade teórica
p_theoretical <- pt(threshold, df = n, lower.tail = TRUE)

# Calcular a diferença e multiplicar por 100
difference <- abs(p_empirical - p_theoretical) * 100

# Resultado final arredondado a 5 casas decimais
resultado_final <- round(difference, 5)
print(resultado_final)
