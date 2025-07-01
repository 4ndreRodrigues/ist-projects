library(ggplot2)

data <- read_csv("IST/PE/Projeto/Pergunta 1/Paises_PIB_ICH.csv")
filtered_data <- subset(data, Continent == "Europe" | Continent == "Americas")
countries <- c("Lithuania", "Iceland", "United States", "Saint Lucia")

dados$Shape <- ifelse(dados$Country %in% paises, dados$Country, "Other")

plot <- ggplot(filtered_data, aes(x = GDP, y = HCI, color = Continent, label = Country)) +
  geom_point(aes(shape = shape)) +
  scale_x_log10() +
  geom_text(data = subset(filtered_data, Country %in% countries),
            aes(label = Country), vjust = -1, hjust = 0.5) +
  theme_minimal() +
  labs(
    title = "HCI vs GDP per capita (Europe and Americas)",
    x = "GDP (log scale)",
    y = "HCI",
    color = "Continent"
  )

ggsave("IST/PE/Projeto/Pergunta 1/scatter_plot.pdf", plot, width = 8, height = 6)

plot