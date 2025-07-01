Nome: André Jacinto de Sousa Rodrigues  Nº92421

;-------------------------------------------------------------------------------
;   CONSTANTES
;-------------------------------------------------------------------------------

SP_INICIAL	    EQU		FDFFh

DISPLAY         EQU     FFF0h
LCD_CONTROL     EQU     FFF4h
LCD_WRITE       EQU     FFF5h
TIMER_COUNTER   EQU     FFF6h
TIMER_CONTROL   EQU     FFF7h
BUTTONS         EQU     FFFAh
BUTTONS_MASK    EQU     1001001111111111b
JT_CURSOR       EQU     FFFCh
JT_WRITE        EQU     FFFEh
JT_CURSOR_MASK  EQU     FFFFh

ESCALA_Y        EQU     18
ESCALA_X        EQU     11

GRAVIDADE	    EQU 	0000000001001110b	; 4 bitas para a parte fraccionaria
PI              EQU 	0011001001000011b	; 12 bits para a
PI_QUADRADO 	EQU 	1001110111101001b	; parte fraccionaria


;-------------------------------------------------------------------------------
;   VARIAVEIS
;-------------------------------------------------------------------------------

                ORIG    8000h

JT_POSICAO      WORD    0
LCD_POSICAO     WORD    0

RANDOM_MASK     WORD    1000000000010110b
RANDOM_NI       WORD    0720h

CARACT_FINAL    STR     '@'

BORDER_1      	STR     '/--------- PLAYER 1:   ---------------------------------- PLAYER 2:   ---------\@'
BORDER_2        STR 	'|                                                                              |@'
BORDER_3        STR 	'\------------------------------ Press IC to exit ------------------------------/@'

LCD_ANGULO      STR     'Angulo:        @'
LCD_ANG_DFLT    WORD    800Eh
LCD_VELOCIDADE 	STR     'Velocidade:    @'
LCD_VEL_DFLT    WORD    801Dh

GORILA_1        STR     'd**b@'
GORILA_2        STR     '/||\@'
GORILA_3        STR     ' /\ @'
GORILA_4        STR     '    @'

GAMEOVER_1      STR     '/--------------------------------------------------------------\@'
GAMEOVER_2      STR     '| ###### ###### ##    ## #####   ###### ##     ## ##### ###### |@'
GAMEOVER_3      STR     '| ##     ##  ## ###  ### ##      ##  ##  ##   ##  ##    ##  ## |@'
GAMEOVER_4      STR     '| ## ### ###### ## ## ## #####   ##  ##   ## ##   ##### ####   |@'
GAMEOVER_5      STR     '| ##  ## ##  ## ##    ## ##      ##  ##    ###    ##    ## ##  |@'
GAMEOVER_6      STR     '| ###### ##  ## ##    ## #####   ######     #     ##### ##  ## |@'
GAMEOVER_7      STR     '\--------------------------------------------------------------/@'

POS_JOGADOR     WORD    0
POS_INI_PROJ    WORD    0
POS_ALVO        WORD    0

JOGADOR_ATUAL   WORD    2
PONTOS_JOG1     WORD    0
PONTOS_JOG2     WORD    0

ANGULO          WORD    0
ANG_RAD         WORD 	0
COS_ANG         WORD	0
SIN_ANG         WORD	0

VELOCIDADE      WORD    0

TEMPO           WORD    0

POS_PROJ        WORD    0
POS_ANT_PROJ    WORD    000Ah


; ------------------------------------------------------------------------------
;	Tabela de  Interrupcoes
; ------------------------------------------------------------------------------

                ORIG    FE00h

BUTAO_0         WORD    INATIVO
BUTAO_1         WORD    INATIVO
BUTAO_2         WORD    INATIVO
BUTAO_3         WORD    INATIVO
BUTAO_4         WORD    INATIVO
BUTAO_5         WORD    INATIVO
BUTAO_6         WORD    INATIVO
BUTAO_7         WORD    INATIVO
BUTAO_8         WORD    INATIVO
BUTAO_9         WORD    INATIVO


                ORIG    FE0Ch

BUTAO_C         WORD    EXIT


                ORIG    FE0Fh

TIMER           WORD    INATIVO


; ############################################################################ ;


                ORIG    0000h

                MOV     R1, SP_INICIAL
                MOV     SP, R1

                MOV     R1, JT_CURSOR_MASK
                MOV     M[JT_CURSOR], R1

                MOV     R1, BUTTONS_MASK
                MOV     M[BUTTONS], R1
                ENI

                JMP     CicloJogo


;-------------------------------------------------------------------------------
;	INTERRUPCOES
;-------------------------------------------------------------------------------

BUTAO_0F:       MOV     R1, 0
                DEC     R2
                RTI

BUTAO_1F:       MOV     R1, 1
                DEC     R2
                RTI

BUTAO_2F:       MOV     R1, 2
                DEC     R2
                RTI

BUTAO_3F:       MOV     R1, 3
                DEC     R2
                RTI

BUTAO_4F:       MOV     R1, 4
                DEC     R2
                RTI

BUTAO_5F:       MOV     R1, 5
                DEC     R2
                RTI

BUTAO_6F:       MOV     R1, 6
                DEC     R2
                RTI

BUTAO_7F:       MOV     R1, 7
                DEC     R2
                RTI

BUTAO_8F:       MOV     R1, 8
                DEC     R2
                RTI

BUTAO_9F:       MOV     R1, 9
                DEC     R2
                RTI

EXIT:           JMP     FinalJogo
                RTI

TIMERF:         MOV     R3, 1
                MOV     M[TIMER_COUNTER], R3
                MOV     R3, 1
                MOV     M[TIMER_CONTROL], R3
                SHR     R4, 1
                RTI

INATIVO:        RTI


; ------------------------------------------------------------------------------
;	FUNCOES
; ------------------------------------------------------------------------------

; ------------------------------------------------------------------------------
;	Avanca uma posicao na janela de texto

ProxCaractIO:   PUSH    R1
                INC     M[JT_POSICAO]
                MOV     R1, M[JT_POSICAO]
                MOV     M[JT_CURSOR], R1
                POP     R1
                RET

; ------------------------------------------------------------------------------
;	Avanca uma linha na janela de texto
;		- recebe como argumento o numero de linhas
;		- que se deseja recuar na linha seguinte

ProxLinhaIO:    PUSH    R1
                MOV     R1, M[JT_POSICAO]
                ADD     R1, 0100h
                SUB     R1, M[SP+3]
                MOV     M[JT_POSICAO], R1
                MOV     M[JT_CURSOR], R1
                POP     R1
                RETN    1

; ------------------------------------------------------------------------------
;	Escreve uma string na janela de texto
;		- recebe como argumento a string que se deseja escrever
;		- e a posicao onde deve comecar a ser escrita

EscreveIO:      PUSH    R1
                PUSH    R2
                PUSH    R3
                PUSH    R4
                MOV     R1, M[SP+6]
                MOV     M[JT_POSICAO], R1
                MOV     M[JT_CURSOR], R1
                MOV     R2, M[CARACT_FINAL]
                MOV     R3, M[SP+7]
EscreveIOAux:   MOV     R4, M[R3]
                CMP     R2, R4
                BR.Z    EscreveIOFim
                MOV     M[JT_WRITE], R4
                CALL    ProxCaractIO
                INC     R3
                BR      EscreveIOAux

EscreveIOFim:   POP     R4
                POP     R3
                POP     R2
                POP     R1
                RETN    2

; ------------------------------------------------------------------------------

ProxCaractLCD:  PUSH    R1
                INC     M[LCD_POSICAO]
                MOV     R1, M[LCD_POSICAO]
                MOV     M[LCD_CONTROL], R1
                POP     R1
                RET

; ------------------------------------------------------------------------------

EscreveLCD:     PUSH    R1
                PUSH    R2
                PUSH    R3
                PUSH    R4
                MOV     R1, M[SP+6]
                MOV     M[LCD_POSICAO], R1
                MOV     M[LCD_CONTROL], R1
                MOV     R2, M[CARACT_FINAL]
                MOV     R3, M[SP+7]
EscreveLCDAux:  MOV     R4, M[R3]
                CMP     R2, R4
                BR.Z    EscreveLCDFim
                MOV     M[LCD_WRITE], R4
                CALL    ProxCaractLCD
                INC     R3
                BR      EscreveLCDAux

EscreveLCDFim:  POP     R4
                POP     R3
                POP     R2
                POP     R1
                RETN    2

;-------------------------------------------------------------------------------

AnguloLCD:      PUSH    R1
                PUSH    LCD_ANGULO
                PUSH    8000h
                CALL    EscreveLCD
                MOV     R1, 800Fh
                MOV     M[LCD_CONTROL], R1
                MOV     R1, 32
                MOV     M[LCD_WRITE], R1
                POP     R1
                RET

; ------------------------------------------------------------------------------

VelocidadeLCD:  PUSH    R1
                PUSH    LCD_VELOCIDADE
                PUSH    8010h
                CALL    EscreveLCD
                MOV     R1, 801Fh
                MOV     M[LCD_CONTROL], R1
                MOV     R1, 32
                MOV     M[LCD_WRITE], R1
                POP     R1
                RET

; ------------------------------------------------------------------------------

EscreveNumLCD:  PUSH    R1
                PUSH    R2
                MOV     R1, M[SP+4]
                MOV     M[LCD_CONTROL], R1
                MOV     R2, M[SP+5]
                ADD     R2, 48
                MOV     M[LCD_WRITE], R2
                POP     R2
                POP     R1
                RETN    2

; ------------------------------------------------------------------------------

DesenhaGorila:  PUSH    R1
                MOV     R1, M[SP+3]
                PUSH    GORILA_1
                PUSH    R1
                CALL    EscreveIO
                PUSH    4
                CALL    ProxLinhaIO
                PUSH    GORILA_2
                PUSH    M[JT_POSICAO]
                CALL    EscreveIO
                PUSH    4
                CALL    ProxLinhaIO
                PUSH    GORILA_3
                PUSH    M[JT_POSICAO]
                CALL    EscreveIO
                POP     R1
                RETN    1

; ------------------------------------------------------------------------------

Aleatoria:      PUSH    R1
                MOV     R1, M[RANDOM_NI]
                AND     R1, FFFEh
                CMP     R1, R0
                BR.NZ   AleatoriaElse
                MOV     R1, M[RANDOM_NI]
                ROR     R1, 1
                BR      AleatoriaFim

AleatoriaElse:  MOV     R1, M[RANDOM_NI]
                XOR     R1, M[RANDOM_MASK]
                ROR     R1, 1

AleatoriaFim:   MOV     M[RANDOM_NI], R1
                POP     R1
                RET

; ------------------------------------------------------------------------------

GeraPos:        PUSH    R1
                PUSH    R2

GeraPosY:       CALL    Aleatoria
                MOV     R1, M[RANDOM_NI]
                MOV     R2, 20
                DIV     R1, R2
                INC     R2
                SHL     R2, 8
                PUSH    R2

GeraPosX:       CALL    Aleatoria
                MOV     R1, M[RANDOM_NI]
                MOV     R2, 15
                DIV     R1, R2
                INC     R2

                MOV     R1, M[SP+5]
                CMP     R1, 1
                BR.Z    GeraPosJogador

GeraPosAlvo:    ADD     R2, 60
                POP     R1
                ADD     R1, R2
                MOV     M[POS_ALVO], R1
                PUSH    R1
                CALL    DesenhaGorila
                BR      GeraPosFim

GeraPosJogador: POP     R1
                ADD     R1, R2
                MOV     M[POS_JOGADOR], R1
                PUSH    R1
                CALL    DesenhaGorila
                ADD     R1, 0004h
                MOV     M[POS_INI_PROJ], R1

GeraPosFim:     POP     R2
                POP     R1
                RETN    1

; ------------------------------------------------------------------------------

GeraGorilas:    PUSH    1
                CALL    GeraPos
                PUSH    2
                CALL    GeraPos
                RET

; ------------------------------------------------------------------------------

AtivaButoes:    PUSH    R1
                MOV     R1, BUTAO_0F
                MOV     M[BUTAO_0], R1
                MOV     R1, BUTAO_1F
                MOV     M[BUTAO_1], R1
                MOV     R1, BUTAO_2F
                MOV     M[BUTAO_2], R1
                MOV     R1, BUTAO_3F
                MOV     M[BUTAO_3], R1
                MOV     R1, BUTAO_4F
                MOV     M[BUTAO_4], R1
                MOV     R1, BUTAO_5F
                MOV     M[BUTAO_5], R1
                MOV     R1, BUTAO_6F
                MOV     M[BUTAO_6], R1
                MOV     R1, BUTAO_7F
                MOV     M[BUTAO_7], R1
                MOV     R1, BUTAO_8F
                MOV     M[BUTAO_8], R1
                MOV     R1, BUTAO_9F
                MOV     M[BUTAO_9], R1
                POP     R1
                RET

; ------------------------------------------------------------------------------

DesativaButoes: PUSH    R1
                MOV     R1, INATIVO
                MOV     M[BUTAO_0], R1             ; Desativa as BUTAO_erupcoes desnecessarias
                MOV     M[BUTAO_1], R1             ; para nao BUTAO_erferirem com o funcionamento
                MOV     M[BUTAO_2], R1             ; do programa
                MOV     M[BUTAO_3], R1
                MOV     M[BUTAO_4], R1
                MOV     M[BUTAO_5], R1
                MOV     M[BUTAO_6], R1
                MOV     M[BUTAO_7], R1
                MOV     M[BUTAO_8], R1
                MOV     M[BUTAO_9], R1
                POP     R1
                RET

; ------------------------------------------------------------------------------

InsereAng:      PUSH    R1
                PUSH    R2
                PUSH    R3
                PUSH    R4
                PUSH    R5
                CALL    AtivaButoes

                MOV     R2, 2
                MOV     R3, R0
                MOV     R5, M[LCD_ANG_DFLT]
InsereAngCiclo: MOV     R4, 10
                CMP     R2, R0
                BR.Z    InsereAngFim

                MOV     R1, -1
InputAng:       CMP     R1, R0					; O programa fica num loop enquanto nenhuma
                BR.N    InputAng  					; interrupcao de I1 a I6 é ativa
                MUL     R4, R3
                PUSH    R1
                PUSH    R5
                CALL    EscreveNumLCD
                ADD     R3, R1
                INC     R5
                BR      InsereAngCiclo

InsereAngFim:   CALL    DesativaButoes
                MOV     M[ANGULO], R3
                POP     R5
                POP     R4
                POP     R3
                POP     R2
                POP     R1
                RET

; ------------------------------------------------------------------------------

InsereVel:      PUSH    R1
                PUSH    R2
                PUSH    R3
                PUSH    R4
                PUSH    R5
                CALL    AtivaButoes

                MOV     R2, 3
                MOV     R3, R0
                MOV     R5, M[LCD_VEL_DFLT]
InsereVelCiclo: MOV     R4, 10
                CMP     R2, R0
                BR.Z    InsereVelFim

                MOV     R1, -1
InputVel:       CMP     R1, R0					; O programa fica num loop enquanto nenhuma
                BR.N    InputVel  					; interrupcao de I1 a I6 é ativa
                MUL     R4, R3
                PUSH    R1
                PUSH    R5
                CALL    EscreveNumLCD
                ADD     R3, R1
                INC     R5
                BR      InsereVelCiclo

InsereVelFim:   CALL    DesativaButoes
                MOV     M[VELOCIDADE], R3
                POP     R5
                POP     R4
                POP     R3
                POP     R2
                POP     R1
                RET

; ------------------------------------------------------------------------------

InsereJogada:   CALL    AnguloLCD
                CALL    InsereAng
                MOV     R1, M[ANGULO]
                CMP     R1, 90
                BR.P    InsereJogada

InsereJogada1:  CALL    VelocidadeLCD
                CALL    InsereVel
                MOV     R1, M[VELOCIDADE]
                CMP     R1, 120
                BR.P    InsereJogada1

InsereJogFim:   RET

; ------------------------------------------------------------------------------

DispJogAtual:   PUSH    R1
                MOV     R1, M[JOGADOR_ATUAL]
                MOV     M[DISPLAY], R1
                POP     R1
                RET

; ------------------------------------------------------------------------------

TrocaJogador:   PUSH    R1
                MOV     R1, M[JOGADOR_ATUAL]
                CMP     R1, 1
                BR.Z    TrocaJogElse
                MOV     R1, 1
                BR      TrocaJogFim

TrocaJogElse:   MOV     R1, 2

TrocaJogFim:    MOV     M[JOGADOR_ATUAL], R1
                POP     R1
                RET

; ------------------------------------------------------------------------------

CalcAngulares:  PUSH    R1
                PUSH    R2
                PUSH    R3
                PUSH    R4

ConverteAng:	MOV 	R1, M[ANGULO]
                MOV 	R2, 180
                SHL 	R1, 8
                DIV 	R1, R2
                MOV 	R2, PI
                MUL 	R2, R1
                SHR 	R1, 8
                SHL 	R2, 8
                ADD 	R1, R2
                MOV 	M[ANG_RAD], R1

Sin:            MOV 	R2, M[ANG_RAD]
                MOV 	R1, PI
                SUB 	R1, R2				; R1 -> (pi - x) 12 bits fraccionaria
                PUSH 	R1
                MOV 	R3, 16
                MUL 	R3, R2
                SHL 	R3, 12
                SHR 	R2, 4
                ADD 	R2, R3 				; R2 -> (16 * x) 8 bits fraccionaria
                MUL 	R1, R2
                SHL 	R1, 4
                SHR 	R2, 12
                ADD 	R1, R2 				; R1 -> 16 * x * (pi - x)
                MOV 	R2, 5
                MOV 	R3, PI_QUADRADO
                MUL  	R2, R3
                SHL 	R2, 12
                SHR 	R3, 4
                ADD 	R2, R3 				; R2 -> (5 * pi²) 8 bits
                MOV 	R3, 4
                MOV 	R4, M[ANG_RAD]
                MUL 	R4, R3				; R3 -> (4 * x) 12 bits
                POP 	R4
                MUL 	R3, R4				; R3 -> 4 * x * (pi - x) 8 bits
                SUB 	R2, R3				; R2 -> 5 * pi² - 4 * x * (pi - x) 8 bits
                SHR 	R2, 8
                DIV 	R1, R2
                MOV 	M[SIN_ANG], R1

Cos:            MOV 	R1, M[ANG_RAD]
                MOV 	R2, M[ANG_RAD]
                MUL 	R1, R2
                SHL     R1, 4
                SHR 	R2, 12
                ADD     R1, R2 				; R1 -> (x²) 12 bits
                PUSH  	R1
                MOV 	R2, 4
                MUL 	R1, R2
                MOV 	R1, PI_QUADRADO
                SUB 	R1, R2				; R1 -> (pi² - 4x²) 12 bits
                MOV 	R2, PI_QUADRADO
                POP 	R3
                ADD 	R2, R3				; R2 -> (pi² + x²) 12 bits
                SHR 	R2, 8
                DIV 	R1, R2
                MOV 	M[COS_ANG], R1		; Cosseno com 8 bits fraccionaria

CalcAngFim:     POP     R4
                POP     R3
                POP     R2
                POP     R1
                RET

; ------------------------------------------------------------------------------

CalcPos:        PUSH    R1
                PUSH    R2
                PUSH    R3

MovimentoY:     MOV 	R1, M[VELOCIDADE]
                MOV 	R2, M[SIN_ANG]
                MUL 	R2, R1
                SHR     R1, 4
                SHL     R2, 12
                ADD 	R1, R2
                MOV 	R2, M[TEMPO]
                MUL 	R2, R1
                SHR     R1, 4
                SHL     R2, 12
                ADD 	R1, R2
                MOV 	R2, M[TEMPO]
                MOV 	R3, M[TEMPO]
                MUL 	R3, R2
                SHR     R2, 4
                SHL 	R3, 12
                ADD     R2, R3
                MOV 	R3, GRAVIDADE  	  ; Calculei previamente o meio da
                MUL 	R3, R2            ; forca gravitica para facilitar as contas
                SHR     R2, 4
                SHL 	R3, 12
                ADD     R2, R3
                PUSH    R2
AddPosIniY:     MOV     R2, M[POS_INI_PROJ]
                AND     R2, FF00h
                MOV     R3, 1700h
                SUB     R3, R2
                SHR     R3, 8
                MOV     R2, ESCALA_Y
                MUL     R2, R3
                SHL     R3, 4
                ADD     R1, R3            ; POS_INI_PROJ + parte inicial da funcionamento
                POP     R2                ; para o projetil ir abaixo do y da POS_INI_PROJ
                SUB     R1, R2            ; E so depois a subtracao
ConvertePosY:   MOV     R2, ESCALA_Y
                DIV     R1, R2
                SHR     R1, 4
                MOV     R2, 24
                SUB     R2, R1
                SHL     R2, 8

                PUSH    R2

MovimentoX:     MOV 	R1, M[VELOCIDADE]
                MOV 	R2, M[COS_ANG]
                MUL 	R2, R1
                SHR     R1, 4
                SHL     R2, 12
                ADD 	R1, R2
                MOV 	R2, M[TEMPO]
                MUL 	R2, R1
                SHR     R1, 4
                SHL     R2, 12
                ADD     R1, R2
AddPosIniX:     MOV     R2, M[POS_INI_PROJ]
                AND     R2, 00FFh
                MOV     R3, ESCALA_X
                MUL     R3, R2
                SHL     R2, 4
                ADD     R1, R2
ConvertePosX:   MOV     R2, ESCALA_X
                DIV     R1, R2
                SHR     R1, 4

                POP     R2
                ADD     R1, R2
                MOV     M[POS_PROJ], R1

CalcPosFim:     POP     R3
                POP     R2
                POP     R1
                RET

; ------------------------------------------------------------------------------

DesenhaProj:	PUSH 	R1
                PUSH 	R2
                PUSH 	R3
                MOV     R1, M[POS_PROJ]
                MOV     R2, M[POS_ANT_PROJ]
                CMP     R1, R2
                JMP.Z   DesProjFim
                AND     R1, 00FFh
                CMP     R1, 004Fh
                BR.Z    LimpaAnt
                MOV     R1, M[POS_PROJ]
                AND     R1, FF00h
                CMP     R1, 1700h
                BR.Z    LimpaAnt
                CMP     R1, R0
                BR.Z    DesProjFim
                MOV     R1, M[POS_PROJ]
                MOV 	M[JT_CURSOR], R1
                MOV 	R3, '('
                MOV 	M[JT_WRITE], R3
LimpaAnt:       MOV 	M[JT_CURSOR], R2
                MOV 	R3, ' '
                MOV 	M[JT_WRITE], R3
                MOV 	M[POS_ANT_PROJ], R1

DesProjFim:	    POP 	R3
                POP 	R2
                POP 	R1
                RET

; ------------------------------------------------------------------------------


Hit:            PUSH    R1
                PUSH    R2
                PUSH    R3
                PUSH    R4
                MOV     R1, M[POS_PROJ]
                MOV     R2, M[POS_ALVO]

HitVerifica:    CMP     R1, R2
                JMP.N   HitFim
                MOV     R3, R2
                ADD     R3, 0203h
                CMP     R1, R3
                BR.P    HitFim
                MOV     R3, R1
                AND     R3, 00FFh
                MOV     R4, R2
                AND     R4, 00FFh
                CMP     R3, R4
                BR.N    HitFim
                ADD     R4, 0003h
                CMP     R3, R4
                BR.P    HitFim
                MOV     R1, 1
                MOV     M[SP+6], R1

HitFim:         POP     R4
                POP     R3
                POP     R2
                POP     R1
                RET

; ------------------------------------------------------------------------------

CalcProjetil:   PUSH    R1
                PUSH    R2
                PUSH	R3
                PUSH	R4
                MOV     R1, TIMERF
                MOV     M[TIMER], R1
                MOV     R1, 1
                MOV     M[TIMER_COUNTER], R1
                MOV     R1, 1
                MOV     M[TIMER_CONTROL], R1

                MOV     M[TEMPO], R0

CalcProjCiclo:  MOV 	R4, 0001h
CalcProjAux:	CMP 	R4, R0
                BR.NZ	CalcProjAux
                MOV 	R4, 100b
                ADD     M[TEMPO], R4
                CALL 	CalcPos
                MOV 	R1, M[POS_PROJ]
                AND 	R1, 00FFh
                MOV 	R2, 004Fh
                CMP 	R1, R2
                MOV     R1, 0
                BR.P 	CalcProjFim
                MOV 	R1, M[POS_PROJ]
                AND 	R1, FF00h
                MOV 	R2, 1700h
                CMP 	R1, R2
                MOV     R1, 0
                BR.P 	CalcProjFim
                CALL 	DesenhaProj
                PUSH    R0
                CALL    Hit
                POP     R1
                CMP     R1, 1
                BR.Z    CalcProjFim
                JMP     CalcProjCiclo

CalcProjFim:    MOV     M[SP+6], R1
                MOV     R1, INATIVO
                MOV     M[TIMER], R1
                POP 	R4
                POP 	R3
                POP     R2
                POP     R1
                RET

; ##############################################################################

; ------------------------------------------------------------------------------
;	Imprime uma borda na janela de texto
;		- (efeito estetico)

Border:         PUSH    R1
                MOV     R1, 22
                PUSH    BORDER_1
                PUSH    0000h
                CALL    EscreveIO

BorderCiclo:    PUSH    80
                CALL    ProxLinhaIO
                CMP     R1, R0
                BR.Z    BorderFim
                PUSH    BORDER_2
                PUSH    M[JT_POSICAO]
                CALL    EscreveIO
                DEC     R1
                BR      BorderCiclo

BorderFim:      PUSH 	BORDER_3
                PUSH	M[JT_POSICAO]
                CALL 	EscreveIO
                POP     R1
                RET

; ------------------------------------------------------------------------------

Score:          PUSH    R1
                MOV     R1, 0015h
                MOV     M[JT_CURSOR], R1
                MOV     R1, M[PONTOS_JOG1]
                ADD     R1, 48
                MOV     M[JT_WRITE], R1
                MOV     R1, 0044h
                MOV     M[JT_CURSOR], R1
                MOV     R1, M[PONTOS_JOG2]
                ADD     R1, 48
                MOV     M[JT_WRITE], R1
                POP     R1
                RET

; ------------------------------------------------------------------------------
;	Imprime uma mensagem de GAME OVER
;		- (efeito estetico)

Gameover:       PUSH    GAMEOVER_1
                PUSH    0708h
                CALL    EscreveIO
                PUSH    GAMEOVER_2
                PUSH    0808h
                CALL    EscreveIO
                PUSH    GAMEOVER_3
                PUSH    0908h
                CALL    EscreveIO
                PUSH    GAMEOVER_4
                PUSH    0A08h
                CALL    EscreveIO
                PUSH    GAMEOVER_5
                PUSH    0B08h
                CALL    EscreveIO
                PUSH    GAMEOVER_6
                PUSH    0C08h
                CALL    EscreveIO
                PUSH    GAMEOVER_7
                PUSH    0D08h
                CALL    EscreveIO
                RET

; ------------------------------------------------------------------------------
;    INICIO DO JOGO
; ------------------------------------------------------------------------------


CicloJogo:      CALL    Border
                CALL    Score
                CALL    GeraGorilas
                CALL    TrocaJogador
NovaTentativa:  CALL    AnguloLCD
                CALL    VelocidadeLCD
                CALL    DispJogAtual
                CALL    InsereJogada
                CALL    CalcAngulares
                PUSH    R0
                CALL    CalcProjetil
                POP     R1
                CMP     R1, 1
                BR.Z    AdicionaPonto
                CALL    TrocaJogador
                BR      NovaTentativa

AdicionaPonto:  MOV     R1, M[JOGADOR_ATUAL]
                CMP     R1, 1
                BR.Z    AdicionaPonto1

AdicionaPonto2: INC     M[PONTOS_JOG2]
                JMP     CicloJogo

AdicionaPonto1: INC     M[PONTOS_JOG1]
                JMP     CicloJogo

FinalJogo:      CALL    Gameover

Fim:            BR      Fim
