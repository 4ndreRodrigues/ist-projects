library(ggplot2)

dados <- readxl::read_excel("IST/PE/Projeto/Pergunta 3/electricity.xlsx")

filtered_data <- subset(dados, PRODUCT == "Renewables" & YEAR >= 2015 & 
                          COUNTRY %in% c("IEA Total", "Italy", "Latvia"))

filtered_data$share <- as.numeric(filtered_data$share) * 100

filtered_data$date <- as.Date(paste("1", filtered_data$MONTH, filtered_data$YEAR, sep = "/"), "%d/%m/%Y")


plot <- ggplot(filtered_data) +
  geom_line(aes(x = date, y = share, color = COUNTRY)) +
  geom_point(aes(x = date, y = share, color = COUNTRY)) +
  labs(
    title = "Monthly Proportion of Renewable Electricity Production (2015-2022)",
    x = "Date",
    y = "Renewables Percentage",
    color = "Country")
  

ggsave("IST/PE/Projeto/Pergunta 3/renewables_proportion.pdf", plot, width = 10, height = 6)

plot
