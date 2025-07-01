library(ggplot2)

dados <- read.csv("IST/PE/Projeto/Pergunta 2/master.csv")


filtered_data <- subset(dados, year == 1986 & age == "25-34 years")

plot <- ggplot(filtered_data) +
  geom_boxplot(aes(x = sex, y = suicides.100k.pop, fill = sex)) +
  labs(
    title = "Suicides per 100k Population (1986, Age 25-34)",
    x = "Sex",
    y = "Suicides per 100k Population",
    fill = "Sex")
  

ggsave("IST/PE/Projeto/Pergunta 2/suicides_boxplot.pdf", plot, width = 12, height = 8)

