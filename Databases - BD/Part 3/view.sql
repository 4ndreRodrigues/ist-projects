CREATE VIEW Vendas(ean, cat, ano, trimestre, mes, dia_mes, dia_semana, distrito, concelho, unidades) AS
SELECT tc.ean, tc.nome, 
    EXTRACT(YEAR FROM er.instante),
    EXTRACT(QUARTER FROM er.instante),
    EXTRACT(MONTH FROM er.instante),
    EXTRACT(DAY FROM er.instante),
    EXTRACT(DOW FROM er.instante),
    pr.distrito, pr.concelho, er.unidades
FROM tem_categoria AS tc 
    INNER JOIN evento_reposicao AS er ON tc.ean = er.ean
    INNER JOIN responsavel_por AS rp ON rp.num_serie = er.num_serie
    INNER JOIN instalada_em AS ie ON ie.num_serie = rp.num_serie
    INNER JOIN ponto_de_retalho AS pr ON pr.nome = ie.local
WHERE tc.ean = er.ean;