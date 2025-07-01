/* Nome: Andre Rodrigues   Numero: 92421 */

:- [codigo_comum].

%---------------------------------------------------------------------------

% Predicados Auxiliares

sublista(SL, L, N) :-
    append([_, SL, _], L),
    length(SL, N).

sublistas(L, N, SLs) :-
    findall(SL, sublista(SL, L, N), SLs).

nao_pertence(_, []).
nao_pertence(E, [P | R]) :-
E \== P,
nao_pertence(E, R).

junta([],L,L).
junta([P | R],L2,[P | J_R_L2]) :- junta(R,L2,J_R_L2).

pertence(P, [Q | _]) :- P == Q.
pertence(P, [_ | R]) :- pertence(P, R).

% membro(E,L) - E ´e membro de L.
membro(E, [E | _ ]).
membro(E, [_ | R]) :- membro(E, R).

intersection([], _, []).
intersection([Head|L1tail], L2, L3) :-
        memberchk(Head, L2),
        !,
        L3 = [Head|L3tail],
        intersection(L1tail, L2, L3tail).
intersection([_|L1tail], L2, L3) :-
        intersection(L1tail, L2, L3).


%---------------------------------------------------------------------------

% Predicados para inicializacao de puzzles 

obtem_letras_palavras(Lst_Pals, Letras) :- 
    maplist(atom_chars, Lst_Pals, Letras_Dsord),
    sort(Letras_Dsord, Letras).

espaco_fila(Fila, Espaco) :-
    append([Pref, Espaco, Suf], Fila),
    length(Espaco, Comp), Comp >= 3,
    nao_pertence(#, Espaco),
    (Pref = []
                           ;
    (last(Pref, X), X == #)),
    (Suf = []
                           ;
    ([P | R] = Suf, P == #)).

espacos_fila(Fila, Espacos) :-
    findall(Espaco, (espaco_fila(Fila, Espaco)), Espacos).

espacos_puzzle(Grelha, Espacos) :-
    maplist(espacos_fila, Grelha, Espacos_Temp),
    mat_transposta(Grelha, Transp),
    maplist(espacos_fila, Transp, Espacos_Temp_2),
    junta(Espacos_Temp, Espacos_Temp_2, Espacos_Temp_3),
    append(Espacos_Temp_3, Espacos).

espacos_com_posicoes_comuns(Espacos, Esp, Esps_com) :-
    findall(Espaco, (member(Espaco, Espacos), member(X, Espaco), member(Y, Esp), var(X), var(Y), X == Y), [Esps_com]).