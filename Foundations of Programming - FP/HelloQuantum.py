# Nome: Andre Jacinto de Souda Rodrigues  Nº 92421

# ---------------------------------------------------------------------------- #
#                             FUNCOES AUXILIARES                               #
# ---------------------------------------------------------------------------- #

def troca_celulas_tabuleiro(t, coor1, coor2):
    '''troca_celulas_tabuleiro: tabuleiro x coordenana x coordenada -> tabuleiro
    Esta funcao devolve o tabuleiro que resulta da troca da celula presente
    na coordenada 1 pela celula presente na coordenada 2.'''
    celula_trocada = tabuleiro_celula(t, coor1)
    t = tabuleiro_substitui_celula(t, tabuleiro_celula(t, coor2), coor1)
    t = tabuleiro_substitui_celula(t, celula_trocada, coor2)
    return t

# ---------------------------------------------------------------------------- #
#                                   CELULA                                     #
# ---------------------------------------------------------------------------- #

# CONSTRUTOR

def cria_celula(v):
    '''cria_celula: {1, 0, -1} -> celula
    Esta funcao recebe o valor do estado de uma celula do qubit (0, representa
    inativo, 1, ativo e -1 incerto) e devolve uma celula com esse valor.'''
    if v == -1:
        return [v]
    elif v == 0:
        return [v]
    elif v == 1:
        return [v]
    else:
        raise ValueError('cria_celula: argumento invalido.')

# SELETOR

def obter_valor(c):
    '''obter_valor: celula -> {1, 0, -1}
    Esta funcao recebe uma celula e devolve o valor associado.'''
    return c[0]

# MODIFICADOR

def inverte_estado(c):
    '''inverte_estado: celula -> celula
    Esta funcao devolve a celula resultante de inverter o estado da celula que e
    seu argumento: uma celula inativa torna-se ativa; uma celula ativa torna-se
    inativa e uma celula no estado incerto mantem-se no mesmo estado.'''
    if obter_valor(c) == 1:
        c[0] = 0
    elif obter_valor(c) == 0:
        c[0] = 1
    return c

# RECONHECEDOR

def eh_celula(arg):
    '''eh_celula: universal -> logico
    Esta funcao devolve verdadeiro apenas no caso do seu argumento ser do tipo
    celula.'''
    if not isinstance(arg, list) or len(arg) != 1 or arg[0] not in [1, 0, -1]:
        return False
    else:
        return True

# TESTE

def celulas_iguais(c1, c2):
    '''celulas_iguais: celula x celula -> logico
    Esta funcao devolve verdadeiro apenas se c1 e c2 sao celulas no mesmo
    estado.'''
    return eh_celula(c1) and eh_celula(c2) and obter_valor(c1) == obter_valor(c2)

# TRANSFORMADOR

def celula_para_str(c):
    '''celula_para_str: celula -> cad. caracteres
    Esta funcao devolve uma cadeia de caracteres que representa a celula que
    e seu argumento: o estado ativo e representado por '1'; o estado inativo e
    representado por '0' e o estado incerto e representado por 'x'.'''
    if obter_valor(c) == 1:
        return '1'
    elif obter_valor(c) == 0:
        return '0'
    else:
        return 'x'

# ---------------------------------------------------------------------------- #
#                                 COORDENADA                                   #
# ---------------------------------------------------------------------------- #

# CONSTRUTOR

def cria_coordenada(l, c):
    '''cria_coordenada: N x N -> coordenada
    Esta funcao devolve a coordenada correspondente a linha l e a coluna c.'''
    if l not in [0, 1, 2] or c not in [0, 1, 2]:
        raise ValueError('cria_coordenada: argumentos invalidos.')
    else:
        return [l, c]

# SELETORES

def coordenada_linha(c):
    '''coordenada_linha: coordenada -> N
    Esta funcao devolve o natural correspondente a linha da coordenada c.'''
    return c[0]

def coordenada_coluna(c):
    '''coordenada_coluna: coordenada -> N
    Esta funcao devolve o natural correspondente a coluna da coordenada c.'''
    return c[1]

# RECONHECEDOR

def eh_coordenada(arg):
    '''eh_coordenada: universal -> logico
    Esta funcao devolve verdadeiro apenas no caso em que o seu argumento
    e do tipo coordenada.'''
    if not isinstance(arg, list) or len(arg) != 2 or arg[0] not in [0, 1, 2] or arg[1] not in [0, 1, 2]:
        return False
    else:
        return True

# TESTE

def coordenadas_iguais(c1, c2):
    '''coordenadas_iguais: coordenada x coordenada -> logico
    Esta funcao devolve verdadeiro apenas se c1 e c2 representam coordenadas
    iguais, ou seja representam a mesma posicao.'''
    return eh_coordenada(c1) and eh_coordenada(c2) and c1 == c2

# TRANSFORMADOR

def coordenada_para_str(c):
    '''coordenada_para_str: coordenada -> cad. caracteres
    Esta funcao devolve a cadeia de caracteres que representa o seu
    argumento.'''
    return '(' + str(coordenada_linha(c)) + ', ' + str(coordenada_coluna(c)) + ')'

# ---------------------------------------------------------------------------- #
#                                 TABULEIRO                                    #
# ---------------------------------------------------------------------------- #

# CONSTRUTOR

def tabuleiro_inicial():
    '''tabuleiro_inicial: {} -> tabuleiro
    Esta funcao devolve o tabuleiro que representa o seu estado inicial do
    jogo.'''
    return [[cria_celula(-1), cria_celula(-1), cria_celula(-1)], \
            [cria_celula(0), cria_celula(0), cria_celula(-1)], \
            [cria_celula(-1), cria_celula(0), cria_celula(-1)]]

def str_para_tabuleiro(s):
    '''str_para_tabuleiro: cad. caracteres -> tabuleiro_inicial
    Esta funcao devolve o tabuleiro correspondente a cadeia de caracteres
    que e seu argumento.'''
    if not isinstance(s, str):
        raise ValueError('str_para_tabuleiro: argumento invalido.')
    conv = eval(s)
    if not isinstance(conv, tuple) or len(conv) != 3 or not isinstance(conv[0], tuple) \
        or not isinstance(conv[1], tuple) or not isinstance(conv[2], tuple) or \
        len(conv[0]) != 3 or len(conv[1]) != 3 or len(conv[2]) != 2:
        raise ValueError('str_para_tabuleiro: argumento invalido.')
    for i in s:
        if i not in ['1','0','-','(',')',',',' ']:
            raise ValueError('str_para_tabuleiro: argumento invalido.')
    return [[cria_celula(conv[0][0]), cria_celula(conv[0][1]), cria_celula(conv[0][2])], \
            [cria_celula(conv[1][0]), cria_celula(conv[1][1]), cria_celula(conv[1][2])], \
            [cria_celula(-1), cria_celula(conv[2][0]), cria_celula(conv[2][1])]]

# SELETORES

def tabuleiro_dimensao(t):
    '''tabuleiro_dimensao: tabuleiro -> N
    Esta funcao devolve o natural correspondente ao numero de linhas (e,
    consequentemente, tambem ao numero de colunas) existentes em t.'''
    return len(t)

def tabuleiro_celula(t, coor):
    '''tabuleiro_celula: tabuleiro x coordenada -> celula
    Esta funcao devolve a celula presente na coordenada coor do tabuleiro t.'''
    return t[coor[0]][coor[1]]

# MODIFICADORES

def tabuleiro_substitui_celula(t, cel, coor):
    '''tabuleiro_substitui_celula: tabuleiro x celulas x coordenada -> tabuleiro
    Esta funcao devolve o tabuleiro que resulta de substituir a celula
    existente na coordenada coor do tabuleiro, pela nova celula.'''
    if not eh_celula(cel) or not eh_coordenada(coor) or not eh_tabuleiro(t):
        raise ValueError('tabuleiro_substitui_celula: argumentos invalidos.')
    else:
        t[coor[0]][coor[1]] = cel
        return t

def tabuleiro_inverte_estado(t, coor):
    '''tabuleiro_inverte_estado: tabuleiro x coordenada -> tabuleiro
    Esta funcao devolve o tabuleiro que resulta de inverter o estado da celula
    presente na coordenada coor do tabuleiro.'''
    if not eh_coordenada(coor) or not eh_tabuleiro(t):
        raise ValueError('tabuleiro_inverte_estado: argumentos invalidos.')
    else:
        t[coor[0]][coor[1]] = inverte_estado(t[coor[0]][coor[1]])
        return t

# RECONHECEDOR

def eh_tabuleiro(arg):
    '''eh_tabuleiro: universal -> logico
    Esta funcao devolve verdadeiro apenas no caso de arg ser do tipo tabuleiro.'''
    i = 0
    if not isinstance(arg, list) or len(arg) != 3 or not isinstance(arg[0], list) \
        or not isinstance(arg[1], list) or not isinstance(arg[2], list) \
        or len(arg[0]) != 3 or len(arg[1]) != 3 or len(arg[2]) != 3:
        return False
    while i <= 2:
        if not eh_celula(arg[0][i]) or not eh_celula(arg[1][i]) or not eh_celula(arg[2][i]):
            return False
        else:
            i += 1
    return True

# TESTE

def tabuleiros_iguais(t1, t2):
    '''tabuleiros_iguais: tabuleiro x tabuleiro -> logico
    Esta funcao devolve verdadeiro apenas no caso de t1 e t2 forem tabuleiros
    que contenham celulas iguais em cada uma das coordenadas.'''
    return eh_tabuleiro(t1) and eh_tabuleiro(t2) and t1 == t2

# TRANSFORMADOR

def tabuleiro_para_str(t):
    '''tabuleiro_para_str: tabuleiro -> cad. caracteres
    Esta funcao devolve a cadeia de caracteres que represente o seu argumento.'''
    if not eh_tabuleiro(t):
        raise ValueError('tabuleiro_str: argumento invalido.')
    else:
        return '+-------+\n|...' + celula_para_str(tabuleiro_celula(t, [0, 2])) \
        + '...|\n|..' + celula_para_str(tabuleiro_celula(t, [0, 1])) + '.' \
        + celula_para_str(tabuleiro_celula(t, [1, 2])) + '..|\n|.' \
        + celula_para_str(tabuleiro_celula(t, [0, 0])) + '.' \
        + celula_para_str(tabuleiro_celula(t, [1, 1])) + '.' \
        + celula_para_str(tabuleiro_celula(t, [2, 2])) + '.|\n|..' \
        + celula_para_str(tabuleiro_celula(t, [1, 0])) + '.' \
        + celula_para_str(tabuleiro_celula(t, [2, 1])) + '..|\n+-------+'

# ---------------------------------------------------------------------------- #
#                           OPERACOES DE ALTO NIVEL                            #
# ---------------------------------------------------------------------------- #

def porta_x(t, lado):
    '''porta_x: tabuleiro x {'E','D'} -> tabuleiro
    Esta funcao recebe um tabuleiro e um caracter (“E” ou “D”) e devolve um
    novo tabuleiro resultante de aplicar a porta X ao qubit esquerdo ou
    direito, conforme o caracter seja 'E' ou 'D', respetivamente.'''
    if not eh_tabuleiro(t):
        raise ValueError('porta_x: argumentos invalidos.')
    elif lado == 'E':
        t = tabuleiro_inverte_estado(t, [1, 0])
        t = tabuleiro_inverte_estado(t, [1, 1])
        t = tabuleiro_inverte_estado(t, [1, 2])
    elif lado == 'D':
        t = tabuleiro_inverte_estado(t, [0, 1])
        t = tabuleiro_inverte_estado(t, [1, 1])
        t = tabuleiro_inverte_estado(t, [2, 1])
    else:
        raise ValueError('porta_x: argumentos invalidos.')
    return t


def porta_z(t, lado):
    '''porta_z: tabuleiro x {'E','D'} -> tabuleiro
    Esta funcao recebe um tabuleiro e um caracter ('E' ou 'D') e devolve o
    tabuleiro resultante de aplicar a porta Z ao qubit da esquerda ou da
    direita, conforme o caracter seja 'E' ou 'D', respetivamente.'''
    if not eh_tabuleiro(t):
        raise ValueError('porta_z: argumentos invalidos.')
    elif lado == 'E':
        t = tabuleiro_inverte_estado(t, [0, 0])
        t = tabuleiro_inverte_estado(t, [0, 1])
        t = tabuleiro_inverte_estado(t, [0, 2])
    elif lado == 'D':
        t = tabuleiro_inverte_estado(t, [0, 2])
        t = tabuleiro_inverte_estado(t, [1, 2])
        t = tabuleiro_inverte_estado(t, [2, 2])
    else:
        raise ValueError('porta_z: argumentos invalidos.')
    return t


def porta_h(t, lado):
    '''porta_h: tabuleiro x {'E','D'} -> tabuleiro
    Esta funcao recebe um tabuleiro e um caracter ('E' ou 'D') e devolve o
    tabuleiro resultante de aplicar a porta H ao qubit da esquerda ou da
    direita, conforme o caracter seja 'E' ou 'D', respetivamente.'''
    if not eh_tabuleiro(t):
        raise ValueError('porta_h: argumentos invalidos.')
    elif lado == 'E':
        t = troca_celulas_tabuleiro(t, [1, 0], [0, 0])
        t = troca_celulas_tabuleiro(t, [1, 1], [0, 1])
        t = troca_celulas_tabuleiro(t, [1, 2], [0, 2])
    elif lado == 'D':
        t = troca_celulas_tabuleiro(t, [0, 1], [0, 2])
        t = troca_celulas_tabuleiro(t, [1, 1], [1, 2])
        t = troca_celulas_tabuleiro(t, [2, 1], [2, 2])
    else:
        raise ValueError('porta_h: argumentos invalidos.')
    return t

# ---------------------------------------------------------------------------- #
#                               FUNCAO PRINCIPAL                               #
# ---------------------------------------------------------------------------- #

def hello_quantum(s):
    '''hello_quantum: cad. caracteres -> logico
    Esta funcao recebe uma cadeia de caracteres contendo a descricao do
    tabuleiro objetivo e do numero maximo de jogadas. A funcao devolve
    verdadeiro se o jogador conseguir transformar o tabuleiro inicial no
    tabuleiro objetivo, nao ultrapassando o numero de jogadas indicado e
    devolve falso em caso contrario.'''
    s1 = ''
    for i in range(len(s) - 1):
        if s[i] != ':':
            s1 += str(s[i])
        else:
            jogadas = int(s[i + 1])
    j = 0
    tabuleiro = tabuleiro_inicial()
    objetivo = str_para_tabuleiro(s1)
    print('Bem-vindo ao Hello Quantum!\nO seu objetivo e chegar ao tabuleiro:')
    print(tabuleiro_para_str(objetivo))
    print('Comecando com o tabuleiro que se segue:')
    print(tabuleiro_para_str(tabuleiro))
    while j < jogadas:
            porta = input('Escolha uma porta para aplicar (X, Z ou H): ')
            lado = input('Escolha um qubit para analisar (E ou D): ')
            if porta == 'X':
                print(tabuleiro_para_str(porta_x(tabuleiro, lado)))
                if tabuleiros_iguais(tabuleiro, objetivo):
                    print('Parabens, conseguiu converter o tabuleiro em', j + 1, 'jogadas!')
                    return True
            elif porta == 'Z':
                print(tabuleiro_para_str(porta_z(tabuleiro, lado)))
                if tabuleiros_iguais(tabuleiro, objetivo):
                    print('Parabens, conseguiu converter o tabuleiro em', j + 1, 'jogadas!')
                    return True
            elif porta == 'H':
                print(tabuleiro_para_str(porta_h(tabuleiro, lado)))
                if tabuleiros_iguais(tabuleiro, objetivo):
                    print('Parabens, conseguiu converter o tabuleiro em', j + 1, 'jogadas!')
                    return True
            j += 1
    return False
