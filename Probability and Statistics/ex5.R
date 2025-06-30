library(ggplot2)
library(tidyverse)
library(kableExtra)
library(dplyr)

set.seed(1594)  # Definir a semente para reproduzibilidade

# Função para gerar amostras da distribuição geométrica
gerar_amostra_geom <- function(n, p) {
  u <- runif(n)  # Simular valores uniformes no intervalo (0, 1)
  x <- floor(log(u) / log(1 - p))  # Aplicar o método de transformação inversa
  return(x)
}

n <- 1042  # Tamanho da amostra
p <- 0.35  # Parâmetro da distribuição geométrica

amostra <- gerar_amostra_geom(n, p)  # Gerar a amostra

media <- mean(amostra)  # Calcular a média amostral
desvio_padrao <- sd(amostra)  # Calcular o desvio padrão amostral

valores_superiores <- amostra[amostra > (media + desvio_padrao)]  # Selecionar valores superiores à média + desvio padrão
proporcao <- length(valores_superiores) / n  # Calcular a proporção

# Imprimir o resultado com 4 casas decimais
cat("A proporção de valores simulados que são superiores à média + desvio padrão é:", format(proporcao, digits = 4), "\n")
