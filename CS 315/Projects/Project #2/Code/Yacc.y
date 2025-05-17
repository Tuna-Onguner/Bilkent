%{
#include <stdio.h>
#include <stdlib.h>
int yylex(void);
void yyerror(char *s);
extern int yylineno;
%}

%token COMMENT
%token LP RP LCB RCB CM CL SC NL
%token TYPE_INT TYPE_FLOAT TYPE_BOOL TYPE_CHAR TYPE_STRING
%token CONST_INT CONST_FLOAT CONST_BOOL CONST_CHAR CONST_STRING
%token FOR IF ELSE RETURN FUNCTION FUNC DOT IDENTIFIER
%token UNSECURED_URL SECURED_URL GET_TIME 
%token CONNECT_TO GET_DATA_FROM SEND_INTEGER_TO RECEIVE_INTEGER_FROM 
%token SET_SWITCH ON OFF
%token SENSOR_TEMPERATURE SENSOR_LIGHT SENSOR_HUMIDITY 
%token SENSOR_AIR_PRESSURE SENSOR_AIR_QUALITY SENSOR_SOUND_LEVEL

%right OP_ASSIGN OP_TERNARY

%left OP_ADD OP_SUB
%left OP_MUL OP_DIV OP_MOD
%left OP_POW
%left OP_OR
%left OP_AND
%left OP_NOT
%left OP_EQUAL OP_NOT_EQUAL
%left OP_LT OP_LTE OP_GT OP_GTE

%%

program: stmts

stmts: if_stmt | for_stmt | declaration_stmt | function_call | stmts

if_stmt:  IF LP bool_expr RP LCB stmts RCB
	| IF LP bool_expr RP LCB stmts RCB ELSE LCB stmts RCB
	| IF LP bool_expr RP LCB stmts RCB ELSE if_stmt

for_stmt:  FOR LP bool_expr RP LCB stmts RCB

declaration_stmt:  data_types IDENTIFIER OP_ASSIGN IDENTIFIER SC
                 | data_types IDENTIFIER OP_ASSIGN constant_value SC
                 | data_types IDENTIFIER OP_ASSIGN arithmetic_operations SC

data_types: TYPE_INT | TYPE_FLOAT  | TYPE_BOOL | TYPE_CHAR | TYPE_STRING

constant_value: CONST_INT | CONST_FLOAT | CONST_STRING | CONST_CHAR | CONST_BOOL

arithmetic_operations:  arithmetic_operants arithmetic_op_symbols arithmetic_operants SC
                      | arithmetic_operations arithmetic_op_symbols arithmetic_operants SC
                      | arithmetic_operations arithmetic_op_symbols arithmetic_operations SC

arithmetic_operants: IDENTIFIER | constant_value

arithmetic_op_symbols: OP_ADD | OP_DIV | OP_MUL | OP_SUB | OP_POW | OP_MOD

bool_expr:  CONST_BOOL | comp_expr

comp_expr: comp_operations | comp_expr loop_op comp_expr

comp_operations:  OP_NOT comp_operations SC
                | IDENTIFIER comp_op_symbols IDENTIFIER SC
                | IDENTIFIER comp_op_symbols constant_value SC
                | constant_value comp_op_symbols IDENTIFIER SC
                | constant_value comp_op_symbols constant_value SC
                | LP CONST_BOOL RP SC

comp_op_symbols: OP_EQUAL | OP_GT | OP_GTE | OP_LT | OP_LTE
loop_op: OP_AND | OP_OR
loop_types: IF | FOR 

function_create:  FUNCTION IDENTIFIER LP parameter_list RP LCB stmts RCB SC
                | FUNCTION IDENTIFIER LP RP LCB stmts RCB SC

function_call:  FUNC IDENTIFIER LP parameter_list RP SC
              | FUNC IDENTIFIER LP RP SC

parameter_list: IDENTIFIER | constant_value | arithmetic_operations | parameter_list CM parameter_list

given_url: SECURED_URL | UNSECURED_URL
set_switch: SET_SWITCH CONST_INT ON | SET_SWITCH CONST_INT OFF
exist_function: GET_TIME | CONNECT_TO given_url | SEND_INTEGER_TO given_url | RECEIVE_INTEGER_FROM given_url | set_switch

device: GET_DATA_FROM sensor 
sensor: SENSOR_TEMPERATURE | SENSOR_LIGHT | SENSOR_HUMIDITY | SENSOR_AIR_PRESSURE | SENSOR_AIR_QUALITY | SENSOR_SOUND_LEVEL

%%

#include "lex.yy.c"

int yyerror(char *s){
  fprintf(stderr, "%s on line %d\n",s, yylineno);
  return 1;
}

int main(){
  yyparse();
  return 0;
}
