library(benford.analysis)

benford <- function(x) log10(1 + 1 / x)

x1 <- 1
x2 <- 9

expInf <- 7
expSup <- 20

# P(x=x1) + P(x=x2)
prob <- benford(x1) + benford(x2)

potencias <- 2^seq(expInf, expSup, by = 1)
digits <- extract.digits(potencias, number.of.digits = 1)
digits

# numerador <- length(which(digits$data.digits == x1 | digits$data.digits == x2))
numerador <- nrow(digits[digits$data.digits == x1 | digits$data.digits == x2, ])
denominador <- expSup - expInf + 1
fracao <- numerador / denominador

abs(prob - fracao)
round(abs(prob - fracao), 4)

