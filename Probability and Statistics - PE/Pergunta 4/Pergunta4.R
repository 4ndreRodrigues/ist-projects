# Fixar a semente
set.seed(2336)

# Definir o número de simulações
num_simulacoes <- 650

# Definir o número de circuitos
num_circuitos <- 7

# Definir as probabilidades dos sinais de 1 a 9
probabilidades <- c(1, 2, 3, 4, 5, 6, 7, 8, 9) / sum(c(1, 2, 3, 4, 5, 6, 7, 8, 9))

# Inicializar contadores
contador_aviso <- 0
contador_nao_desligado <- 0

# Simular as realizações
for (sim in 1:num_simulacoes) {
  sinais <- sample(1:9, num_circuitos, replace = TRUE, prob = probabilidades)
  
  # Verificar as condições
  aviso_sonoro <- any(sinais == 2)
  desligado <- any(sinais == 1)
  
  if (!desligado) {
    contador_nao_desligado <- contador_nao_desligado + 1
    if (aviso_sonoro) {
      contador_aviso <- contador_aviso + 1
    }
  }
}

# Calcular a proporção
proporcao <- contador_aviso / contador_nao_desligado

# Imprimir o resultado arredondado a 2 casas decimais
proporcao_arredondada <- round(proporcao, 2)
print(proporcao_arredondada)

