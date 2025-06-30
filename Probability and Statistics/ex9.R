set.seed(1518)

# Parâmetros
p <- 0.5
gamma <- 0.95
n_values <- c(30, 50, 100, 200, 300, 500, 1000)
k <- 1500

# Vetor para armazenar as médias das diferenças
mean_diffs <- numeric(length(n_values))

# Função para calcular o comprimento do intervalo de confiança usando o Método 1
interval_length_method1 <- function(x_bar, n, z) {
  delta <- z * sqrt((x_bar * (1 - x_bar)) / n)
  return(2 * delta)
}

# Função para calcular o comprimento do intervalo de confiança usando o Método 2
interval_length_method2 <- function(x_bar, n) {
  delta <- qnorm(1 - (1 - gamma) / 2) * sqrt((x_bar * (1 - x_bar)) / n)
  return(2 * delta)
}

# Loop sobre os valores de n
for (i in seq_along(n_values)) {
  n <- n_values[i]
  diffs <- numeric(k)
  
  # Loop para gerar as amostras e calcular as diferenças
  for (j in 1:k) {
    samples <- rbinom(n, 1, p)
    x_bar <- mean(samples)
    z <- qnorm(1 - (1 - gamma) / 2)
    diff <- interval_length_method2(x_bar, n) - interval_length_method1(x_bar, n, z)
    diffs[j] <- diff
  }
  
  # Calcular a média das diferenças
  mean_diffs[i] <- mean(diffs)
}

# Gráfico
plot(n_values, mean_diffs, type = "b", xlab = "Tamanho da amostra (n)", ylab = "Diferença média", 
     main = "Diferença média entre os métodos 1 e 2", pch = 16, col = "blue")

# Salvar o gráfico em um arquivo PDF
pdf("diferencas_medias.pdf", width = 8.27, height = 11.69)
plot(n_values, mean_diffs, type = "b", xlab = "Tamanho da amostra (n)", ylab = "Diferença média", 
     main = "Diferença média entre os métodos 1 e 2", pch = 16, col = "blue")
dev.off()
