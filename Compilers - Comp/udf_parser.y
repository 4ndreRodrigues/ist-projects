%{
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <algorithm>
#include <memory>
#include <cstring>
#include <cdk/compiler.h>
#include <cdk/types/types.h>
#include ".auto/all_nodes.h"
#define LINE                         compiler->scanner()->lineno()
#define yylex()                      compiler->scanner()->scan()
#define yyerror(compiler, s)         compiler->scanner()->error(s)
//-- don't change *any* of these --- END!

#define NIL (new cdk::nil_node(LINE))
%}

%parse-param {std::shared_ptr<cdk::compiler> compiler}

%union {
  //--- don't change *any* of these: if you do, you'll break the compiler.
  YYSTYPE() : type(cdk::primitive_type::create(0, cdk::TYPE_VOID)) {}
  ~YYSTYPE() {}
  YYSTYPE(const YYSTYPE &other) { *this = other; }
  YYSTYPE& operator=(const YYSTYPE &other) { type = other.type; return *this; }

  std::shared_ptr<cdk::basic_type> type;        /* expression type */
  //-- don't change *any* of these --- END!

  int                   i;          /* integer value */
  double                d;            
  std::string          *s;          /* symbol name or string literal */
  cdk::basic_node      *node;       /* node pointer */
  cdk::sequence_node   *sequence;
  cdk::expression_node *expression; /* expression nodes */
  cdk::lvalue_node     *lvalue;
  udf::block_node      *block;
};

%token <i> tINTEGER
%token <s> tID tSTRING
%token <d> tREAL
%token tFOR tIF tTHEN tELSE tELIF
%token tCAPACITY tDIM tDIMS tRANK tCONTRACT
%token tTYPE_AUTO tTYPE_INT tTYPE_REAL tTYPE_STRING tTYPE_VOID tTYPE_TENSOR tTYPE_POINTER
%token tWRITE tWRITELN tINPUT
%token tRETURN tCONTINUE tBREAK tSIZEOF
%token tPUBLIC tPRIVATE tFORWARD tNULLPTR tOBJECTS


%nonassoc tIFX
%nonassoc tELIF tELSE


%right '='
%left tOR
%left tAND
%nonassoc '~'
%left tEQ tNE
%left tGE tLE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc tUNARY '?'
%left tCONTRACT


%type <node> instruction return iffalse
%type <sequence> file instructions opt_instructions
%type <sequence> expressions opt_expressions tensors
%type <expression> expression integer real opt_initializer tensor
%type <lvalue> lvalue
%type <block> block

%type <node>     declaration argdec fordec vardec fundec fundef
%type <sequence> declarations argdecs fordecs vardecs opt_vardecs
%type <node>     opt_forinit

%type<s> string
%type<type> data_type void_type

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%

file         : /* empty */              { compiler->ast($$ = new cdk::sequence_node(LINE)); }
             | declarations             { compiler->ast($$ = $1);                           }
             ;

declarations :              declaration { $$ = new cdk::sequence_node(LINE, $1);            }
             | declarations declaration { $$ = new cdk::sequence_node(LINE, $2, $1);        }
             ;

declaration  : vardec ';' { $$ = $1; }
             | fundec     { $$ = $1; }
             | fundef     { $$ = $1; }
             ;

vardec       : tFORWARD data_type  tID                 { $$ = new udf::variable_declaration_node(LINE, tPUBLIC,  $2, *$3, nullptr); delete $3; }
             | tPUBLIC  data_type  tID opt_initializer { $$ = new udf::variable_declaration_node(LINE, tPUBLIC,  $2, *$3, $4); delete $3;      }
             |          data_type  tID opt_initializer { $$ = new udf::variable_declaration_node(LINE, tPRIVATE, $1, *$2, $3); delete $2;      }
             | tPUBLIC  tTYPE_AUTO tID opt_initializer { $$ = new udf::variable_declaration_node(LINE, tPUBLIC,  nullptr, *$3, $4); delete $3; }
             |          tTYPE_AUTO tID opt_initializer { $$ = new udf::variable_declaration_node(LINE, tPUBLIC,  nullptr, *$2, $3); delete $2; }
             ;

vardecs      : vardec ';'                       { $$ = new cdk::sequence_node(LINE, $1);     }
             | vardecs vardec ';'               { $$ = new cdk::sequence_node(LINE, $2, $1); }
             ;

opt_vardecs  : /* empty */ { $$ = NULL; }
             | vardecs     { $$ = $1; }
             ;

data_type    : tTYPE_STRING                     { $$ = cdk::primitive_type::create(4, cdk::TYPE_STRING);  }
             | tTYPE_INT                        { $$ = cdk::primitive_type::create(4, cdk::TYPE_INT);     }
             | tTYPE_REAL                       { $$ = cdk::primitive_type::create(8, cdk::TYPE_DOUBLE);  }
             | tTYPE_TENSOR                     { $$ = cdk::reference_type::create(4, nullptr);           }
             | tTYPE_POINTER '<' data_type '>'  { $$ = cdk::reference_type::create(4, $3);                }
             | tTYPE_POINTER '<' tTYPE_AUTO '>' { $$ = cdk::reference_type::create(4, nullptr);           }
             ;

opt_initializer  : /* empty */                  { $$ = nullptr; /* must be nullptr, not NIL */ }
                 | '=' expression               { $$ = $2; }
                 ;

void_type   : tTYPE_VOID { $$ = cdk::primitive_type::create(0, cdk::TYPE_VOID);  }
            ;

tensor      : '[' expressions ']'                {
                // tensor de 1 dimensão
                $$ = new udf::tensor_node(LINE, std::shared_ptr<cdk::sequence_node>($2));
              }/*
            | '[' tensors ']'                    {
                // tensor de N dimensões: extrair subdimensões e incrementar
                auto first_tensor = dynamic_cast<udf::tensor_node*>($2->node(0));
                std::vector<int> dims = { static_cast<int>($2->size()) };
                if (first_tensor)
                  dims.insert(dims.end(), first_tensor->dimensions().begin(), first_tensor->dimensions().end());

                // linearizar elementos
                cdk::sequence_node *flat = nullptr;
                for (size_t i = 0; i < $2->size(); ++i) {
                  auto sub = dynamic_cast<udf::tensor_node*>($2->node(i));
                  for (size_t j = 0; j < sub->elements()->size(); ++j)
                    flat = new cdk::sequence_node(LINE, sub->elements()->node(j), flat);
                }

                $$ = new udf::tensor_node(LINE, std::shared_ptr<cdk::sequence_node>(flat), dims);
              }*/
            ;

tensors     : tensor                            { $$ = new cdk::sequence_node(LINE, $1); }
            | tensors ',' tensor                { $$ = new cdk::sequence_node(LINE, $3, $1); }
            ;

fundec   :          data_type  tID '(' argdecs ')' { $$ = new udf::function_declaration_node(LINE, tPRIVATE, $1, *$2, $4); delete $2; }
         | tFORWARD data_type  tID '(' argdecs ')' { $$ = new udf::function_declaration_node(LINE, tFORWARD,  $2, *$3, $5); delete $3; }
         | tPUBLIC  data_type  tID '(' argdecs ')' { $$ = new udf::function_declaration_node(LINE, tPUBLIC,  $2, *$3, $5); delete $3; }
         |          tTYPE_AUTO tID '(' argdecs ')' { $$ = new udf::function_declaration_node(LINE, tPRIVATE, nullptr, *$2, $4); delete $2; }
         | tFORWARD tTYPE_AUTO tID '(' argdecs ')' { $$ = new udf::function_declaration_node(LINE, tFORWARD,  nullptr, *$3, $5); delete $3; }
         | tPUBLIC  tTYPE_AUTO tID '(' argdecs ')' { $$ = new udf::function_declaration_node(LINE, tPUBLIC,  nullptr, *$3, $5); delete $3; }
         |          void_type  tID '(' argdecs ')' { $$ = new udf::function_declaration_node(LINE, tPRIVATE, $1, *$2, $4); delete $2; }
         | tFORWARD void_type  tID '(' argdecs ')' { $$ = new udf::function_declaration_node(LINE, tFORWARD,  $2, *$3, $5); delete $3; }
         | tPUBLIC  void_type  tID '(' argdecs ')' { $$ = new udf::function_declaration_node(LINE, tPUBLIC,  $2, *$3, $5); delete $3; }
         ;

fundef   :         data_type  tID '(' argdecs ')' block { $$ = new udf::function_definition_node(LINE, tPRIVATE, $1, *$2, $4, $6); delete $2; }
         | tPUBLIC data_type  tID '(' argdecs ')' block { $$ = new udf::function_definition_node(LINE, tPUBLIC,  $2, *$3, $5, $7); delete $3; }
         |         tTYPE_AUTO tID '(' argdecs ')' block { $$ = new udf::function_definition_node(LINE, tPRIVATE, nullptr, *$2, $4, $6); delete $2; }
         | tPUBLIC tTYPE_AUTO tID '(' argdecs ')' block { $$ = new udf::function_definition_node(LINE, tPUBLIC,  nullptr, *$3, $5, $7); delete $3; }
         |         void_type  tID '(' argdecs ')' block { $$ = new udf::function_definition_node(LINE, tPRIVATE, $1, *$2, $4, $6); delete $2; }
         | tPUBLIC void_type  tID '(' argdecs ')' block { $$ = new udf::function_definition_node(LINE, tPUBLIC,  $2, *$3, $5, $7); delete $3; }
         ;

argdecs  : /* empty */         { $$ = new cdk::sequence_node(LINE);         }
         |             argdec  { $$ = new cdk::sequence_node(LINE, $1);     }
         | argdecs ',' argdec  { $$ = new cdk::sequence_node(LINE, $3, $1); }
         ;

argdec   : data_type tID { $$ = new udf::variable_declaration_node(LINE, tPRIVATE, $1, *$2, nullptr); delete $2; }
         ;

block    : '{' opt_vardecs opt_instructions '}'  { $$ = new udf::block_node(LINE, $2, $3);    }
         ;

fordec          : data_type tID '=' expression   { $$ = new udf::variable_declaration_node(LINE, tPRIVATE,  $1, *$2, $4); delete $2; }
                ;
              
fordecs         :             fordec             { $$ = new cdk::sequence_node(LINE, $1);     }
                | fordecs ',' fordec             { $$ = new cdk::sequence_node(LINE, $3, $1); }

opt_forinit     : /**/                           { $$ = new cdk::sequence_node(LINE, NIL);    }
                | fordecs                        { $$ = $1; }
                ;
               
return          : tRETURN            ';'         { $$ = new udf::return_node(LINE, nullptr);  }
                | tRETURN expression ';'         { $$ = new udf::return_node(LINE, $2);       }
                ;

instructions    : instruction                    { $$ = new cdk::sequence_node(LINE, $1);     }
                | instructions instruction       { $$ = new cdk::sequence_node(LINE, $2, $1); }
                ;

opt_instructions: /* empty */                    { $$ = new cdk::sequence_node(LINE);         }
                | instructions                   { $$ = $1;                                   }
                ;

instruction     : tIF '(' expression ')' instruction %prec tIFX                           { $$ = new udf::if_node(LINE, $3, $5); }
                | tIF '(' expression ')' instruction iffalse                              { $$ = new udf::if_else_node(LINE, $3, $5, $6); }
                | tFOR '(' opt_forinit ';' opt_expressions ';' opt_expressions ')' instruction  { $$ = new udf::for_node(LINE, $3, $5, $7, $9); }
                | expression ';'                                                          { $$ = new udf::evaluation_node(LINE, $1); }
                | tWRITE   expressions ';'                                                { $$ = new udf::write_node(LINE, $2, false); }
                | tWRITELN expressions ';'                                                { $$ = new udf::write_node(LINE, $2, true); }
                | tBREAK                                                                  { $$ = new udf::break_node(LINE); }
                | tCONTINUE                                                               { $$ = new udf::continue_node(LINE); }
                | return                                                                  { $$ = $1; }
                | block                                                                   { $$ = $1; }
                ;

iffalse         : tELSE instruction                                                       { $$ = $2; }
                | tELIF expression instruction %prec tIFX                                 { $$ = new udf::if_node(LINE, $2, $3); }
                | tELIF expression instruction iffalse                                    { $$ = new udf::if_else_node(LINE, $2, $3, $4); }

expression : integer                             { $$ = $1;                             }
           | real                                { $$ = $1;                             }
           | string                              { $$ = new cdk::string_node(LINE, $1); }
           | tensor                              { $$ = $1;                             }
           | tNULLPTR                            { $$ = new udf::nullptr_node(LINE);    }
           /* LEFT VALUES */
           | lvalue                              { $$ = new cdk::rvalue_node(LINE, $1); }
           /* ASSIGNMENTS */
           | lvalue '=' expression               { $$ = new cdk::assignment_node(LINE, $1, $3); }
           /* ARITHMETIC EXPRESSIONS */
           | expression '+' expression           { $$ = new cdk::add_node(LINE, $1, $3); }
           | expression '-' expression           { $$ = new cdk::sub_node(LINE, $1, $3); }
           | expression '*' expression           { $$ = new cdk::mul_node(LINE, $1, $3); }
           | expression '/' expression           { $$ = new cdk::div_node(LINE, $1, $3); }
           | expression '%' expression           { $$ = new cdk::mod_node(LINE, $1, $3); }
           /* LOGICAL EXPRESSIONS */
           | expression  '<' expression          { $$ = new cdk::lt_node(LINE, $1, $3); }
           | expression  '>' expression          { $$ = new cdk::gt_node(LINE, $1, $3); }
           | expression tLE  expression          { $$ = new cdk::le_node(LINE, $1, $3); }
           | expression tEQ  expression          { $$ = new cdk::eq_node(LINE, $1, $3); }
           | expression tGE  expression          { $$ = new cdk::ge_node(LINE, $1, $3); }
           | expression tNE  expression          { $$ = new cdk::ne_node(LINE, $1, $3); }
           /* LOGICAL EXPRESSIONS */
           | expression tAND  expression         { $$ = new cdk::and_node(LINE, $1, $3); }
           | expression tOR   expression         { $$ = new cdk::or_node (LINE, $1, $3); }
           /* UNARY EXPRESSION */
           | '-' expression %prec tUNARY         { $$ = new cdk::unary_minus_node(LINE, $2); }
           | '+' expression %prec tUNARY         { $$ = new cdk::unary_plus_node(LINE, $2);  }
           | '~' expression                      { $$ = new cdk::not_node(LINE, $2);         }
           /* TENSOR EXPRESSIONS
           | expression '.' tDIMS                { $$ = new udf::tensor_dims_node(LINE, $1);         }
           | expression '.' tCAPACITY            { $$ = new udf::tensor_capacity_node(LINE, $1);     }
           | expression '.' tRANK                { $$ = new udf::tensor_rank_node(LINE, $1);         }
           | expression '.' tDIM '(' integer ')' { $$ = new udf::tensor_dim_node(LINE, $1, $5);      }
           | expression tCONTRACT expression     { $$ = new udf::tensor_contract_node(LINE, $1, $3); }
           /* OTHER EXPRESSION */
           | tINPUT                              { $$ = new udf::input_node(LINE); }
           /* OTHER EXPRESSION */
           | tID '(' opt_expressions ')'         { $$ = new udf::function_call_node(LINE, *$1, $3); delete $1; }
           | tSIZEOF '(' expression ')'          { $$ = new udf::sizeof_node(LINE, $3);                        }
           /* OTHER EXPRESSION */
           | '(' expression ')'                  { $$ = $2;                                  }
           | '[' expression ']'                  { $$ = new udf::stack_alloc_node(LINE, $2); }
           | lvalue '?'                          { $$ = new udf::address_of_node(LINE, $1);  }
           ; 

expressions     : expression                   { $$ = new cdk::sequence_node(LINE, $1);     }
                | expressions ',' expression   { $$ = new cdk::sequence_node(LINE, $3, $1); }
                ;

opt_expressions : /* empty */                  { $$ = new cdk::sequence_node(LINE); }
                | expressions                  { $$ = $1;                           }
                ;

lvalue          : tID                                            { $$ = new cdk::variable_node(LINE, *$1); delete $1; }
                | '(' expression ')' '[' expression ']'          { $$ = new udf::left_index_node(LINE, $2, $5); }
                | '[' expression ']' '[' expression ']'          { $$ = new udf::left_index_node(LINE, new udf::stack_alloc_node(LINE, $2), $5); }
                | tID '(' opt_expressions ')' '[' expression ']' { $$ = new udf::left_index_node(LINE, new udf::function_call_node(LINE, *$1, $3), $6); }
                | lvalue             '[' expression ']'          { $$ = new udf::left_index_node(LINE, new cdk::rvalue_node(LINE, $1), $3); }
                ;

integer         : tINTEGER                     { $$ = new cdk::integer_node(LINE, $1); };
real            : tREAL                        { $$ = new cdk::double_node(LINE, $1);  };
string          : tSTRING                      { $$ = $1;                              }
                | string tSTRING               { $$ = $1; $$->append(*$2); delete $2;  }
                ;

%%
