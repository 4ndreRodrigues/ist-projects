DROP TRIGGER IF EXISTS verifica_categoria ON tem_outra;
DROP TRIGGER IF EXISTS verifica_reposicao ON evento_reposicao;
DROP TRIGGER IF EXISTS verifica_produto_reposto ON planograma;

CREATE OR REPLACE FUNCTION verifica_categoria_trigger_proc()
RETURNS TRIGGER AS
$$
BEGIN
    IF (SELECT count(*) FROM tem_outra WHERE super_categoria = categoria) != 0 THEN
        RAISE EXCEPTION 'Categoria esta incluida em si propria';
    END IF;
    RETURN new;
   END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER verifica_categoria AFTER INSERT OR UPDATE ON
tem_outra
EXECUTE PROCEDURE verifica_categoria_trigger_proc();

CREATE OR REPLACE FUNCTION verifica_reposicao_trigger_proc()
RETURNS TRIGGER AS
$$
BEGIN
    IF (SELECT tin FROM evento_reposicao, planograma
        WHERE planograma.ean = evento_reposicao.ean
        AND planograma.nro = evento_reposicao.nro
        AND planograma.num_serie = evento_reposicao.num_serie
        AND planograma.fabricante = evento_reposicao.fabricante
        AND planograma.unidades < evento_reposicao.unidades) != 0 THEN
            RAISE EXCEPTION 'Unidades repostas superiores ao que esta no planograma';
    END IF;
    RETURN new;
   END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER verifica_reposicao AFTER INSERT OR UPDATE ON
evento_reposicao
FOR EACH ROW EXECUTE PROCEDURE verifica_reposicao_trigger_proc();


CREATE OR REPLACE FUNCTION verifica_produto_reposto_trigger_proc()
RETURNS TRIGGER AS
$$
BEGIN
    IF (SELECT count(planograma.ean) FROM planograma, tem_categoria, prateleira, produto
        WHERE planograma.nro = prateleira.nro
        AND planograma.ean = produto.ean
        AND produto.cat != prateleira.nome AND produto.cat != prateleira.nome) != 0 THEN
            RAISE EXCEPTION 'Produto nao atribuido a prateleira certa';
    END IF;
    RETURN new;
END ;
$$ LANGUAGE plpgsql;
CREATE TRIGGER verifica_produto_reposto AFTER INSERT OR UPDATE ON
planograma
EXECUTE PROCEDURE verifica_produto_reposto_trigger_proc();
