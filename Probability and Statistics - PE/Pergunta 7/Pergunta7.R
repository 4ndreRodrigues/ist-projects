# Carregar o pacote necessário
if (!requireNamespace("stats4", quietly = TRUE)) {
  install.packages("stats4")
}
library(stats4)

# Amostra fornecida
amostra <- c(4.37, 4.3, 5.15, 5.11, 5.15, 4.66, 6.15, 5.72, 5.87, 5.64, 4.05)

# Definir a função de verossimilhança
logverossim <- function(theta) {
  a <- 4
  n <- length(amostra)
  if (theta <= 0) return(Inf)  # Garantir que theta é positivo
  logL <- n * log(theta) - (theta + 1) * sum(log(amostra)) + n * theta * log(a)
  return(-logL)  # Negativo porque a função mle minimiza
}

# Estimar theta usando a função mle
estimativa <- mle(minuslogl = logverossim, start = list(theta = 3.6))
theta_est <- coef(estimativa)
theta_est

# Estimar o quantil de probabilidade p = 0.75
p <- 0.75
a <- 4
Q_p <- a * (1 - p)^(-1 / theta_est)
Q_p

# Verdadeiro valor do quantil quando θ = 3.6
theta_true <- 3.6
Q_p_true <- a * (1 - p)^(-1 / theta_true)
Q_p_true

# Calcular o desvio absoluto
desvio_absoluto <- abs(Q_p - Q_p_true)
desvio_absoluto_arredondado <- round(desvio_absoluto, 4)
desvio_absoluto_arredondado