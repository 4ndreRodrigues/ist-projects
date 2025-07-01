SELECT CAST(ev.instante AS DATE) AS data, v.dia_semana AS dia_da_semana, v.concelho, SUM(v.unidades) AS unidades_vendidas
FROM Vendas AS v, evento_reposicao AS ev
WHERE ev.instante BETWEEN '2021-01-01' AND '2021-12-31'
GROUP BY CUBE(ev.instante, v.dia_semana, v.concelho);


SELECT v.distrito, v.concelho, v.cat AS categoria, v.dia_semana AS dia_da_semana, SUM(v.unidades) AS unidades_vendidas
FROM Vendas AS v
GROUP BY CUBE(v.distrito, v.concelho, v.cat, v.dia_semana);

