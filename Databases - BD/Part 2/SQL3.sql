/*
a)
*/
SELECT name FROM retalhista WHERE retalhista.tin =
(SELECT func.tin_max FROM (SELECT tin_max, count(n.tin_max)
AS tin_ FROM (SELECT DISTINCT tin AS tin_max, nome_cat, count(responsavel_por.tin)
FROM responsavel_por GROUP BY tin, nome_cat ORDER BY tin
) AS n GROUP BY tin_max ORDER BY tin_max) AS func
WHERE func.tin_ = (SELECT max(tin_) from (SELECT tin_max, count(n.tin_max)
AS tin_ FROM (SELECT DISTINCT tin AS tin_max, nome_cat, count(responsavel_por.tin)
FROM responsavel_por GROUP BY tin, nome_cat ORDER BY tin
) AS n GROUP BY tin_max ORDER BY tin_max) AS func_2));

/*
b)
*/
SELECT DISTINCT retalhista.name FROM responsavel_por, retalhista, categoria_simples
WHERE retalhista.tin = responsavel_por.tin AND responsavel_por.nome_cat = categoria_simples.nome;

/*
c)
*/
SELECT ean FROM produto
WHERE ean NOT IN (SELECT ean FROM evento_reposicao);

/*
d)
*/
SELECT ean FROM (SELECT ean, count(ean) AS count_ean FROM (SELECT DISTINCT ean, tin FROM evento_reposicao
GROUP BY ean, tin ORDER BY ean)AS n GROUP BY ean) AS m
WHERE m.count_ean = 1;
