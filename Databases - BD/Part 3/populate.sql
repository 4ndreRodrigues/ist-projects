DROP TABLE IF EXISTS evento_reposicao;
DROP TABLE IF EXISTS responsavel_por;
DROP TABLE IF EXISTS retalhista;
DROP TABLE IF EXISTS planograma;
DROP TABLE IF EXISTS prateleira;
DROP TABLE IF EXISTS instalada_em;
DROP TABLE IF EXISTS ponto_de_retalho;
DROP TABLE IF EXISTS IVM;
DROP TABLE IF EXISTS tem_categoria;
DROP TABLE IF EXISTS produto;
DROP TABLE IF EXISTS tem_outra;
DROP TABLE IF EXISTS super_categoria;
DROP TABLE IF EXISTS categoria_simples;
DROP TABLE IF EXISTS categoria;

CREATE TABLE categoria
(
    nome VARCHAR NOT NULL ,
    PRIMARY KEY (nome)
);

CREATE TABLE categoria_simples
(
    nome VARCHAR NOT NULL ,
    PRIMARY KEY (nome),
    FOREIGN KEY (nome)
        REFERENCES categoria(nome)
);

CREATE TABLE super_categoria
(
    nome VARCHAR NOT NULL ,
    PRIMARY KEY (nome),
    FOREIGN KEY (nome)
        REFERENCES categoria(nome)
);

CREATE TABLE tem_outra
(
    super_categoria VARCHAR NOT NULL ,
    categoria VARCHAR NOT NULL ,
    PRIMARY KEY (categoria),
    FOREIGN KEY (categoria)
        REFERENCES categoria(nome),
    FOREIGN KEY (super_categoria)
        REFERENCES super_categoria(nome)
);

CREATE TABLE produto
(
    ean INTEGER NOT NULL ,
    cat VARCHAR NOT NULL ,
    descr VARCHAR NOT NULL ,
    PRIMARY KEY (ean),
    FOREIGN KEY (cat)
        REFERENCES categoria(nome)
);

CREATE TABLE tem_categoria
(
    ean INTEGER NOT NULL ,
    nome VARCHAR NOT NULL ,
    FOREIGN KEY (ean)
        REFERENCES produto(ean),
    FOREIGN KEY (nome)
        REFERENCES categoria(nome)
);

CREATE TABLE IVM
(
    num_serie INT NOT NULL ,
    fabricante VARCHAR NOT NULL ,
    PRIMARY KEY (num_serie, fabricante)
);

CREATE TABLE ponto_de_retalho
(
    nome VARCHAR NOT NULL ,
    distrito VARCHAR NOT NULL ,
    concelho VARCHAR NOT NULL ,
    PRIMARY KEY (nome)
);

CREATE TABLE instalada_em
(
    num_serie INTEGER NOT NULL ,
    fabricante VARCHAR NOT NULL ,
    local VARCHAR NOT NULL ,
    PRIMARY KEY (num_serie, fabricante),
    FOREIGN KEY (num_serie, fabricante)
        REFERENCES IVM(num_serie, fabricante),
    FOREIGN KEY (local)
        REFERENCES ponto_de_retalho(nome)
);

CREATE TABLE prateleira
(
    nro INTEGER NOT NULL ,
    num_serie INTEGER NOT NULL ,
    fabricante VARCHAR NOT NULL ,
    altura INTEGER NOT NULL ,
    nome VARCHAR NOT NULL ,
    PRIMARY KEY (nro, num_serie, fabricante),
    FOREIGN KEY (num_serie, fabricante)
        REFERENCES IVM(num_serie, fabricante),
    FOREIGN KEY (nome)
        REFERENCES categoria(nome)
);

CREATE TABLE planograma
(
    ean INTEGER NOT NULL ,
    nro INTEGER NOT NULL ,
    num_serie INTEGER NOT NULL ,
    fabricante VARCHAR NOT NULL ,
    faces INTEGER NOT NULL ,
    unidades INTEGER NOT NULL ,
    loc VARCHAR NOT NULL ,
    PRIMARY KEY (ean, nro, num_serie, fabricante),
    FOREIGN KEY (ean)
        REFERENCES produto(ean),
    FOREIGN KEY (nro, num_serie, fabricante)
        REFERENCES prateleira(nro, num_serie, fabricante)
);

CREATE TABLE retalhista
(
    tin INTEGER NOT NULL ,
    name VARCHAR NOT NULL ,
    PRIMARY KEY (tin),
    UNIQUE (name)
);

CREATE TABLE responsavel_por
(
    nome_cat VARCHAR NOT NULL ,
    tin INTEGER NOT NULL ,
    num_serie INTEGER NOT NULL ,
    fabricante VARCHAR NOT NULL ,
    PRIMARY KEY (nome_cat, num_serie, fabricante),
    FOREIGN KEY (num_serie, fabricante)
        REFERENCES IVM(num_serie, fabricante) ,
    FOREIGN KEY (tin)
        REFERENCES retalhista(tin),
    FOREIGN KEY (nome_cat) REFERENCES categoria(nome)
);

CREATE TABLE evento_reposicao
(
    ean INTEGER NOT NULL ,
    nro INTEGER NOT NULL ,
    num_serie INTEGER NOT NULL ,
    fabricante VARCHAR NOT NULL ,
    instante TIMESTAMP NOT NULL ,
    unidades INTEGER NOT NULL ,
    tin INTEGER,
    PRIMARY KEY (ean, nro, num_serie, fabricante, instante),
    FOREIGN KEY (ean, nro, num_serie, fabricante)
        REFERENCES planograma(ean, nro, num_serie, fabricante),
    FOREIGN KEY (tin)
        REFERENCES retalhista(tin)
);

/*IVM*/
INSERT INTO IVM VALUES (90,'fab1');
INSERT INTO IVM VALUES (91,'fab1');
INSERT INTO IVM VALUES (92,'fab1');
INSERT INTO IVM VALUES (93,'fab1');
INSERT INTO IVM VALUES (94,'fab1');
INSERT INTO IVM VALUES (91,'fab2');
INSERT INTO IVM VALUES (95,'fab2');
INSERT INTO IVM VALUES (96,'fab2');
INSERT INTO IVM VALUES (97,'fab2');
INSERT INTO IVM VALUES (97,'fab3');
INSERT INTO IVM VALUES (98,'fab3');
INSERT INTO IVM VALUES (99,'fab3');
INSERT INTO IVM VALUES (100,'fab3');
/*categoria*/
INSERT INTO categoria VALUES ('leite com chocolate');
INSERT INTO categoria VALUES ('leite');
INSERT INTO categoria VALUES ('bebidas em lata');
INSERT INTO categoria VALUES ('sumos');
INSERT INTO categoria VALUES ('agua');
INSERT INTO categoria VALUES ('bebidas');
INSERT INTO categoria VALUES ('barras energeticas');
INSERT INTO categoria VALUES ('barritas');
INSERT INTO categoria VALUES ('doces');
INSERT INTO categoria VALUES ('bolachas');
INSERT INTO categoria VALUES ('comida');
/*produto*/
INSERT INTO produto VALUES (1000,'sumos','compal pera');
INSERT INTO produto VALUES (1001,'sumos','ice tea manga');
INSERT INTO produto VALUES (1002,'bebidas em lata','pepsi');
INSERT INTO produto VALUES (1003,'bebidas em lata','coca cola');
INSERT INTO produto VALUES (1004,'agua','luso');
INSERT INTO produto VALUES (1005,'agua','Vimeiro');
INSERT INTO produto VALUES (1006,'leite','Matinal');
INSERT INTO produto VALUES (1007,'leite','Vigor');
INSERT INTO produto VALUES (1008,'leite com chocolate','Mimosa');
INSERT INTO produto VALUES (1009,'leite com chocolate','UCAL');
INSERT INTO produto VALUES (1010,'bebidas','Sagres');
INSERT INTO produto VALUES (1011,'barras energeticas','PowerBar');
INSERT INTO produto VALUES (1012,'barras energeticas','Roo Bar');
INSERT INTO produto VALUES (1013,'barras energeticas','Victory Endurance');
INSERT INTO produto VALUES (1014,'barritas','GOLDEN GRAHAMS');
INSERT INTO produto VALUES (1015,'doces','Twix');
INSERT INTO produto VALUES (1016,'doces','Kit Kat');
INSERT INTO produto VALUES (1017,'doces','mars');
INSERT INTO produto VALUES (1018,'doces','maltesers');
INSERT INTO produto VALUES (1019,'bolachas','oreo');
INSERT INTO produto VALUES (1020,'bolachas','MY COOKIE');
INSERT INTO produto VALUES (1021,'comida','Lasanha');
INSERT INTO produto VALUES (1022,'comida','Hamburger');
/*tem categoria*/
INSERT INTO tem_categoria VALUES (1000,'sumos');
INSERT INTO tem_categoria VALUES (1001,'sumos');
INSERT INTO tem_categoria VALUES (1002,'bebidas em lata');
INSERT INTO tem_categoria VALUES (1003,'bebidas em lata');
INSERT INTO tem_categoria VALUES (1004,'agua');
INSERT INTO tem_categoria VALUES (1005,'agua');
INSERT INTO tem_categoria VALUES (1006,'leite');
INSERT INTO tem_categoria VALUES (1007,'leite');
INSERT INTO tem_categoria VALUES (1008,'leite com chocolate');
INSERT INTO tem_categoria VALUES (1009,'leite com chocolate');
INSERT INTO tem_categoria VALUES (1010,'bebidas');
INSERT INTO tem_categoria VALUES (1011,'barras energeticas');
INSERT INTO tem_categoria VALUES (1012,'barras energeticas');
INSERT INTO tem_categoria VALUES (1013,'barras energeticas');
INSERT INTO tem_categoria VALUES (1014,'barritas');
INSERT INTO tem_categoria VALUES (1015,'doces');
INSERT INTO tem_categoria VALUES (1016,'doces');
INSERT INTO tem_categoria VALUES (1017,'doces');
INSERT INTO tem_categoria VALUES (1018,'doces');
INSERT INTO tem_categoria VALUES (1019,'bolachas');
INSERT INTO tem_categoria VALUES (1020,'bolachas');
INSERT INTO tem_categoria VALUES (1021,'comida');
INSERT INTO tem_categoria VALUES (1022,'comida');
/*super categoria*/
INSERT INTO super_categoria VALUES ('comida');
INSERT INTO super_categoria VALUES ('barritas');
INSERT INTO super_categoria VALUES ('bebidas');
INSERT INTO super_categoria VALUES ('leite');
/*categoria simples*/
INSERT INTO categoria_simples VALUES ('bebidas em lata');
INSERT INTO categoria_simples VALUES ('agua');
INSERT INTO categoria_simples VALUES ('sumos');
INSERT INTO categoria_simples VALUES ('doces');
INSERT INTO categoria_simples VALUES ('bolachas');
INSERT INTO categoria_simples VALUES ('leite com chocolate');
INSERT INTO categoria_simples VALUES ('barras energeticas');
/*tem outra*/
INSERT INTO tem_outra VALUES ('bebidas','bebidas em lata');
INSERT INTO tem_outra VALUES ('leite','leite com chocolate');
INSERT INTO tem_outra VALUES ('bebidas','leite');
INSERT INTO tem_outra VALUES ('bebidas','sumos');
INSERT INTO tem_outra VALUES ('bebidas','agua');
INSERT INTO tem_outra VALUES ('barritas','barras energeticas');
INSERT INTO tem_outra VALUES ('barritas','doces');
INSERT INTO tem_outra VALUES ('comida','barritas');
INSERT INTO tem_outra VALUES ('comida','bolachas');
/*ponto de retalho*/
INSERT INTO ponto_de_retalho VALUES ('IST','Lisboa','Lisboa');
INSERT INTO ponto_de_retalho VALUES ('GALP','Santarem','Coruche');
INSERT INTO ponto_de_retalho VALUES ('Colombo','Lisboa','Carnide');
/*instalada em*/
INSERT INTO instalada_em VALUES (90,'fab1','IST');
INSERT INTO instalada_em VALUES (91,'fab1','IST');
INSERT INTO instalada_em VALUES (91,'fab2','IST');
INSERT INTO instalada_em VALUES (95,'fab2','IST');
INSERT INTO instalada_em VALUES (97,'fab3','IST');
INSERT INTO instalada_em VALUES (100,'fab3','IST');
INSERT INTO instalada_em VALUES (92,'fab1','GALP');
INSERT INTO instalada_em VALUES (93,'fab1','GALP');
INSERT INTO instalada_em VALUES (94,'fab1','Colombo');
INSERT INTO instalada_em VALUES (96,'fab2','Colombo');
INSERT INTO instalada_em VALUES (97,'fab2','Colombo');
INSERT INTO instalada_em VALUES (98,'fab3','Colombo');
INSERT INTO instalada_em VALUES (99,'fab3','Colombo');
/*prateleira*/
INSERT INTO prateleira VALUES (1,90,'fab1',30,'doces');
INSERT INTO prateleira VALUES (2,90,'fab1',30,'bolachas');
INSERT INTO prateleira VALUES (3,90,'fab1',30,'barritas');
INSERT INTO prateleira VALUES (4,90,'fab1',30,'sumos');
INSERT INTO prateleira VALUES (5,91,'fab1',30,'barras energeticas');
INSERT INTO prateleira VALUES (6,91,'fab1',30,'bebidas em lata');
INSERT INTO prateleira VALUES (7,91,'fab1',30,'comida');
INSERT INTO prateleira VALUES (8,91,'fab1',30,'agua');
INSERT INTO prateleira VALUES (9,91,'fab2',30,'doces');
INSERT INTO prateleira VALUES (10,91,'fab2',30,'bolachas');
INSERT INTO prateleira VALUES (11,91,'fab2',30,'barritas');
INSERT INTO prateleira VALUES (12,91,'fab2',30,'leite com chocolate');
INSERT INTO prateleira VALUES (13,95,'fab2',30,'barras energeticas');
INSERT INTO prateleira VALUES (14,95,'fab2',30,'bolachas');
INSERT INTO prateleira VALUES (15,95,'fab2',30,'comida');
INSERT INTO prateleira VALUES (16,95,'fab2',30,'bebidas');
INSERT INTO prateleira VALUES (17,97,'fab3',30,'doces');
INSERT INTO prateleira VALUES (18,97,'fab3',30,'bolachas');
INSERT INTO prateleira VALUES (19,97,'fab3',30,'barritas');
INSERT INTO prateleira VALUES (20,97,'fab3',30,'comida');
INSERT INTO prateleira VALUES (21,100,'fab3',30,'bebidas em lata');
INSERT INTO prateleira VALUES (22,100,'fab3',30,'sumos');
INSERT INTO prateleira VALUES (23,100,'fab3',30,'leite com chocolate');
INSERT INTO prateleira VALUES (24,100,'fab3',30,'agua');
INSERT INTO prateleira VALUES (25,92,'fab1',30,'doces');
INSERT INTO prateleira VALUES (26,92,'fab1',30,'barras energeticas');
INSERT INTO prateleira VALUES (27,92,'fab1',30,'barritas');
INSERT INTO prateleira VALUES (28,92,'fab1',30,'leite com chocolate');
INSERT INTO prateleira VALUES (29,93,'fab1',30,'barras energeticas');
INSERT INTO prateleira VALUES (30,93,'fab1',30,'bebidas');
INSERT INTO prateleira VALUES (31,93,'fab1',30,'comida');
INSERT INTO prateleira VALUES (32,93,'fab1',30,'agua');
INSERT INTO prateleira VALUES (33,98,'fab3',30,'doces');
INSERT INTO prateleira VALUES (34,98,'fab3',30,'bolachas');
INSERT INTO prateleira VALUES (35,98,'fab3',30,'barritas');
INSERT INTO prateleira VALUES (36,98,'fab3',30,'sumos');
INSERT INTO prateleira VALUES (37,99,'fab3',30,'barras energeticas');
INSERT INTO prateleira VALUES (38,99,'fab3',30,'bebidas em lata');
INSERT INTO prateleira VALUES (39,99,'fab3',30,'comida');
INSERT INTO prateleira VALUES (40,99,'fab3',30,'agua');
INSERT INTO prateleira VALUES (41,96,'fab2',30,'bolachas');
INSERT INTO prateleira VALUES (42,96,'fab2',30,'doces');
INSERT INTO prateleira VALUES (43,96,'fab2',30,'comida');
INSERT INTO prateleira VALUES (44,96,'fab2',30,'sumos');
INSERT INTO prateleira VALUES (45,97,'fab2',30,'barras energeticas');
INSERT INTO prateleira VALUES (46,97,'fab2',30,'bebidas em lata');
INSERT INTO prateleira VALUES (47,97,'fab2',30,'leite');
INSERT INTO prateleira VALUES (48,97,'fab2',30,'sumos');
INSERT INTO prateleira VALUES (49,94,'fab1',30,'doces');
INSERT INTO prateleira VALUES (50,94,'fab1',30,'comida');
INSERT INTO prateleira VALUES (51,94,'fab1',30,'agua');
INSERT INTO prateleira VALUES (52,94,'fab1',30,'sumos');
/*planograma*/
INSERT INTO planograma VALUES (1015,1,90,'fab1',2,10,3);
INSERT INTO planograma VALUES (1016,1,90,'fab1',2,10,3);
INSERT INTO planograma VALUES (1017,1,90,'fab1',2,10,3);
INSERT INTO planograma VALUES (1018,1,90,'fab1',2,10,3);
INSERT INTO planograma VALUES (1019,2,90,'fab1',4,20,6);
INSERT INTO planograma VALUES (1020,2,90,'fab1',4,20,6);
INSERT INTO planograma VALUES (1014,3,90,'fab1',5,25,8);
INSERT INTO planograma VALUES (1000,4,90,'fab1',4,20,6);
INSERT INTO planograma VALUES (1001,4,90,'fab1',4,20,6);
INSERT INTO planograma VALUES (1011,5,91,'fab1',3,15,4);
INSERT INTO planograma VALUES (1012,5,91,'fab1',3,15,4);
INSERT INTO planograma VALUES (1013,5,91,'fab1',3,15,4);
INSERT INTO planograma VALUES (1002,6,91,'fab1',4,20,6);
INSERT INTO planograma VALUES (1003,6,91,'fab1',4,20,6);
INSERT INTO planograma VALUES (1021,7,91,'fab1',4,20,6);
INSERT INTO planograma VALUES (1022,7,91,'fab1',4,20,6);
INSERT INTO planograma VALUES (1004,8,91,'fab1',4,20,6);
INSERT INTO planograma VALUES (1005,8,91,'fab1',4,20,6);
INSERT INTO planograma VALUES (1015,9,91,'fab2',2,10,3);
INSERT INTO planograma VALUES (1016,9,91,'fab2',2,10,3);
INSERT INTO planograma VALUES (1017,9,91,'fab2',2,10,3);
INSERT INTO planograma VALUES (1018,9,91,'fab2',2,10,3);
INSERT INTO planograma VALUES (1019,10,91,'fab2',4,20,6);
INSERT INTO planograma VALUES (1020,10,91,'fab2',4,20,6);
INSERT INTO planograma VALUES (1014,11,91,'fab2',5,25,8);
INSERT INTO planograma VALUES (1008,12,91,'fab2',4,20,6);
INSERT INTO planograma VALUES (1009,12,91,'fab2',4,20,6);
INSERT INTO planograma VALUES (1011,13,95,'fab2',3,15,4);
INSERT INTO planograma VALUES (1012,13,95,'fab2',3,15,4);
INSERT INTO planograma VALUES (1013,13,95,'fab2',3,15,4);
INSERT INTO planograma VALUES (1019,14,95,'fab2',4,20,6);
INSERT INTO planograma VALUES (1020,14,95,'fab2',4,20,6);
INSERT INTO planograma VALUES (1021,15,95,'fab2',4,20,6);
INSERT INTO planograma VALUES (1022,15,95,'fab2',4,20,6);
INSERT INTO planograma VALUES (1010,16,95,'fab2',8,30,6);
INSERT INTO planograma VALUES (1015,17,97,'fab3',2,10,3);
INSERT INTO planograma VALUES (1016,17,97,'fab3',2,10,3);
INSERT INTO planograma VALUES (1017,17,97,'fab3',2,10,3);
INSERT INTO planograma VALUES (1018,17,97,'fab3',2,10,3);
INSERT INTO planograma VALUES (1019,18,97,'fab3',4,20,6);
INSERT INTO planograma VALUES (1020,18,97,'fab3',4,20,6);
INSERT INTO planograma VALUES (1014,19,97,'fab3',5,25,8);
INSERT INTO planograma VALUES (1021,20,97,'fab3',4,20,6);
INSERT INTO planograma VALUES (1022,20,97,'fab3',4,20,6);
INSERT INTO planograma VALUES (1002,21,100,'fab3',4,20,6);
INSERT INTO planograma VALUES (1003,21,100,'fab3',4,20,6);
INSERT INTO planograma VALUES (1000,22,100,'fab3',4,20,6);
INSERT INTO planograma VALUES (1001,22,100,'fab3',4,20,6);
INSERT INTO planograma VALUES (1008,23,100,'fab3',4,20,6);
INSERT INTO planograma VALUES (1009,23,100,'fab3',4,20,6);
INSERT INTO planograma VALUES (1004,24,100,'fab3',4,20,6);
INSERT INTO planograma VALUES (1005,24,100,'fab3',4,20,6);
INSERT INTO planograma VALUES (1015,25,92,'fab1',2,10,3);
INSERT INTO planograma VALUES (1016,25,92,'fab1',2,10,3);
INSERT INTO planograma VALUES (1017,25,92,'fab1',2,10,3);
INSERT INTO planograma VALUES (1018,25,92,'fab1',2,10,3);
INSERT INTO planograma VALUES (1011,26,92,'fab1',3,15,4);
INSERT INTO planograma VALUES (1012,26,92,'fab1',3,15,4);
INSERT INTO planograma VALUES (1013,26,92,'fab1',3,15,4);
INSERT INTO planograma VALUES (1014,27,92,'fab1',5,25,8);
INSERT INTO planograma VALUES (1008,28,92,'fab1',4,20,6);
INSERT INTO planograma VALUES (1009,28,92,'fab1',4,20,6);
INSERT INTO planograma VALUES (1011,29,93,'fab1',3,15,4);
INSERT INTO planograma VALUES (1012,29,93,'fab1',3,15,4);
INSERT INTO planograma VALUES (1013,29,93,'fab1',3,15,4);
INSERT INTO planograma VALUES (1010,30,93,'fab1',8,30,6);
INSERT INTO planograma VALUES (1021,31,93,'fab1',4,20,6);
INSERT INTO planograma VALUES (1022,31,93,'fab1',4,20,6);
INSERT INTO planograma VALUES (1004,32,93,'fab1',4,20,6);
INSERT INTO planograma VALUES (1005,32,93,'fab1',4,20,6);
INSERT INTO planograma VALUES (1015,33,98,'fab3',2,10,3);
INSERT INTO planograma VALUES (1016,33,98,'fab3',2,10,3);
INSERT INTO planograma VALUES (1017,33,98,'fab3',2,10,3);
INSERT INTO planograma VALUES (1018,33,98,'fab3',2,10,3);
INSERT INTO planograma VALUES (1019,34,98,'fab3',4,20,6);
INSERT INTO planograma VALUES (1020,34,98,'fab3',4,20,6);
INSERT INTO planograma VALUES (1014,35,98,'fab3',5,25,8);
INSERT INTO planograma VALUES (1000,36,98,'fab3',4,20,6);
INSERT INTO planograma VALUES (1001,36,98,'fab3',4,20,6);
INSERT INTO planograma VALUES (1011,37,99,'fab3',3,15,4);
INSERT INTO planograma VALUES (1012,37,99,'fab3',3,15,4);
INSERT INTO planograma VALUES (1013,37,99,'fab3',3,15,4);
INSERT INTO planograma VALUES (1002,38,99,'fab3',4,20,6);
INSERT INTO planograma VALUES (1003,38,99,'fab3',4,20,6);
INSERT INTO planograma VALUES (1021,39,99,'fab3',4,20,6);
INSERT INTO planograma VALUES (1022,39,99,'fab3',4,20,6);
INSERT INTO planograma VALUES (1004,40,99,'fab3',4,20,6);
INSERT INTO planograma VALUES (1005,40,99,'fab3',4,20,6);
INSERT INTO planograma VALUES (1019,41,96,'fab2',4,20,6);
INSERT INTO planograma VALUES (1020,41,96,'fab2',4,20,6);
INSERT INTO planograma VALUES (1015,42,96,'fab2',2,10,3);
INSERT INTO planograma VALUES (1016,42,96,'fab2',2,10,3);
INSERT INTO planograma VALUES (1017,42,96,'fab2',2,10,3);
INSERT INTO planograma VALUES (1018,42,96,'fab2',2,10,3);
INSERT INTO planograma VALUES (1021,43,96,'fab2',4,20,6);
INSERT INTO planograma VALUES (1022,43,96,'fab2',4,20,6);
INSERT INTO planograma VALUES (1000,44,96,'fab2',4,20,6);
INSERT INTO planograma VALUES (1001,44,96,'fab2',4,20,6);
INSERT INTO planograma VALUES (1011,45,97,'fab2',3,15,4);
INSERT INTO planograma VALUES (1012,45,97,'fab2',3,15,4);
INSERT INTO planograma VALUES (1013,45,97,'fab2',3,15,4);
INSERT INTO planograma VALUES (1002,46,97,'fab2',4,20,6);
INSERT INTO planograma VALUES (1003,46,97,'fab2',4,20,6);
INSERT INTO planograma VALUES (1006,47,97,'fab2',4,20,6);
INSERT INTO planograma VALUES (1007,47,97,'fab2',4,20,6);
INSERT INTO planograma VALUES (1000,48,97,'fab2',4,20,6);
INSERT INTO planograma VALUES (1001,48,97,'fab2',4,20,6);
INSERT INTO planograma VALUES (1015,49,94,'fab1',2,10,3);
INSERT INTO planograma VALUES (1016,49,94,'fab1',2,10,3);
INSERT INTO planograma VALUES (1017,49,94,'fab1',2,10,3);
INSERT INTO planograma VALUES (1018,49,94,'fab1',2,10,3);
INSERT INTO planograma VALUES (1021,50,94,'fab1',4,20,6);
INSERT INTO planograma VALUES (1022,50,94,'fab1',4,20,6);
INSERT INTO planograma VALUES (1004,51,94,'fab1',4,20,6);
INSERT INTO planograma VALUES (1005,51,94,'fab1',4,20,6);
INSERT INTO planograma VALUES (1000,52,94,'fab1',4,20,6);
INSERT INTO planograma VALUES (1001,52,94,'fab1',4,20,6);
/*retalhista*/
INSERT INTO retalhista VALUES (1,'Miguel Bento');
INSERT INTO retalhista VALUES (2,'Bernardo Pimenta');
INSERT INTO retalhista VALUES (3,'Manuel Santos');
INSERT INTO retalhista VALUES (4,'Samuel Fernando');
INSERT INTO retalhista VALUES (5,'Cristiano Almeida');
INSERT INTO retalhista VALUES (6,'João Silva');
INSERT INTO retalhista VALUES (7,'Alexandre Silva');
INSERT INTO retalhista VALUES (8,'Miguel Almeida');
INSERT INTO retalhista VALUES (9,'Sérgio Silva');
INSERT INTO retalhista VALUES (10,'João Fernandes');
/*responsavel por*/
INSERT INTO responsavel_por VALUES ('doces',1,90,'fab1');
INSERT INTO responsavel_por VALUES ('bolachas',2,90,'fab1');
INSERT INTO responsavel_por VALUES ('barritas',3,90,'fab1');
INSERT INTO responsavel_por VALUES ('sumos',4,90,'fab1');
INSERT INTO responsavel_por VALUES ('barras energeticas',5,91,'fab1');
INSERT INTO responsavel_por VALUES ('bebidas em lata',6,91,'fab1');
INSERT INTO responsavel_por VALUES ('comida',2,91,'fab1');
INSERT INTO responsavel_por VALUES ('agua',8,91,'fab1');
INSERT INTO responsavel_por VALUES ('doces',1,91,'fab2');
INSERT INTO responsavel_por VALUES ('bolachas',4,91,'fab2');
INSERT INTO responsavel_por VALUES ('barritas',2,91,'fab2');
INSERT INTO responsavel_por VALUES ('leite com chocolate',2,91,'fab2');
INSERT INTO responsavel_por VALUES ('barras energeticas',3,95,'fab2');
INSERT INTO responsavel_por VALUES ('bolachas',4,95,'fab2');
INSERT INTO responsavel_por VALUES ('comida',5,95,'fab2');
INSERT INTO responsavel_por VALUES ('bebidas',6,95,'fab2');
INSERT INTO responsavel_por VALUES ('doces',1,97,'fab3');
INSERT INTO responsavel_por VALUES ('bolachas',8,97,'fab3');
INSERT INTO responsavel_por VALUES ('barritas',9,97,'fab3');
INSERT INTO responsavel_por VALUES ('comida',10,97,'fab3');
INSERT INTO responsavel_por VALUES ('bebidas em lata',1,100,'fab3');
INSERT INTO responsavel_por VALUES ('sumos',4,100,'fab3');
INSERT INTO responsavel_por VALUES ('leite com chocolate',3,100,'fab3');
INSERT INTO responsavel_por VALUES ('agua',8,100,'fab3');
INSERT INTO responsavel_por VALUES ('doces',1,92,'fab1');
INSERT INTO responsavel_por VALUES ('barras energeticas',6,92,'fab1');
INSERT INTO responsavel_por VALUES ('barritas',7,92,'fab1');
INSERT INTO responsavel_por VALUES ('leite com chocolate',8,92,'fab1');
INSERT INTO responsavel_por VALUES ('barras energeticas',9,93,'fab1');
INSERT INTO responsavel_por VALUES ('bebidas',2,93,'fab1');
INSERT INTO responsavel_por VALUES ('comida',1,93,'fab1');
INSERT INTO responsavel_por VALUES ('agua',8,93,'fab1');
INSERT INTO responsavel_por VALUES ('doces',1,98,'fab3');
INSERT INTO responsavel_por VALUES ('bolachas',7,98,'fab3');
INSERT INTO responsavel_por VALUES ('barritas',5,98,'fab3');
INSERT INTO responsavel_por VALUES ('sumos',4,98,'fab3');
INSERT INTO responsavel_por VALUES ('barras energeticas',7,99,'fab3');
INSERT INTO responsavel_por VALUES ('bebidas em lata',1,99,'fab3');
INSERT INTO responsavel_por VALUES ('comida',9,99,'fab3');
INSERT INTO responsavel_por VALUES ('agua',8,99,'fab3');
INSERT INTO responsavel_por VALUES ('bolachas',2,96,'fab2');
INSERT INTO responsavel_por VALUES ('doces',1,96,'fab2');
INSERT INTO responsavel_por VALUES ('comida',3,96,'fab2');
INSERT INTO responsavel_por VALUES ('sumos',4,96,'fab2');
INSERT INTO responsavel_por VALUES ('barras energeticas',5,97,'fab2');
INSERT INTO responsavel_por VALUES ('bebidas em lata',6,97,'fab2');
INSERT INTO responsavel_por VALUES ('leite',7,97,'fab2');
INSERT INTO responsavel_por VALUES ('sumos',4,97,'fab2');
INSERT INTO responsavel_por VALUES ('doces',1,94,'fab1');
INSERT INTO responsavel_por VALUES ('comida',3,94,'fab1');
INSERT INTO responsavel_por VALUES ('agua',8,94,'fab1');
INSERT INTO responsavel_por VALUES ('sumos',4,94,'fab1');
/*eventos de reposicao*/
INSERT INTO evento_reposicao VALUES (1014,3,90,'fab1','2021-12-12 10:12',10,3);
INSERT INTO evento_reposicao VALUES (1001,4,90,'fab1','2022-06-05 16:20',8,4);
INSERT INTO evento_reposicao VALUES (1002,6,91,'fab1','2021-10-18 14:50',7,6);
INSERT INTO evento_reposicao VALUES (1015,9,91,'fab2','2021-10-18 14:50',7,9);
INSERT INTO evento_reposicao VALUES (1016,9,91,'fab2','2021-10-18 14:51',7,9);
INSERT INTO evento_reposicao VALUES (1017,9,91,'fab2','2021-10-18 14:52',5,9);
INSERT INTO evento_reposicao VALUES (1018,9,91,'fab2','2021-10-18 14:53',9,9);
INSERT INTO evento_reposicao VALUES (1000,22,100,'fab3','2021-09-20 11:31',4,2);
INSERT INTO evento_reposicao VALUES (1001,22,100,'fab3','2021-09-20 11:32',7,2);
INSERT INTO evento_reposicao VALUES (1004,24,100,'fab3','2021-09-20 11:34',16,4);
INSERT INTO evento_reposicao VALUES (1005,24,100,'fab3','2021-09-20 11:35',12,4);
INSERT INTO evento_reposicao VALUES (1009,23,100,'fab3','2022-02-28 11:20',5,8);
INSERT INTO evento_reposicao VALUES (1011,29,93,'fab1','2022-02-28 11:40',8,9);
INSERT INTO evento_reposicao VALUES (1012,29,93,'fab1','2021-02-28 11:44',6,9);
INSERT INTO evento_reposicao VALUES (1013,29,93,'fab1','2021-02-28 11:48',10,9);