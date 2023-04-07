%option main
newline 		\n
tab			\t		
letter			[a-zA-Z]|\.|\&|\=|\?|\!|\%|\+|\-|\_
sign        		[+-]
digit			[0-9]
int			{sign}?{digit}+
float			{int}*(\.){digit}+
bool			True|False
alphanumeric		{letter}+({digit}|{letter})*
id			{alphanumeric}	
char			(\'[^\']*\'|\'{letter}\'|\'{digit}\')
string			(\"[^\"]*\"|\"({letter}|{digit})+{alphanumeric}\")		
unsecuredUrl		("http://")+{alphanumeric}
securedUrl		("https://")+{alphanumeric}
sensorType		TEMPERATURE|LIGHT|HUMIDITY|AIR_PRESSURE|AIR_QUALITY|SOUND_LEVEL
switch			("switch")
switchOn 		open|true|on|1
switchOff 		close|false|off|0
%%
{newline}		printf("\n");
{tab}			printf("\t");
int			printf("DATA_TYPE_INT ");
float			printf("DATA_TYPE_FLOAT ");
bool			printf("DATA_TYPE_BOOL ");
char			printf("DATA_TYPE_CHAR ");	
string			printf("DATA_TYPE_STRING ");
{int}			printf("CONST_INT ");
{float}			printf("CONST_FLOAT ");
{bool}			printf("CONST_BOOL ");
{char}			printf("CONST_CHAR ");
{string}		printf("CONST_STRING ");
"//".*			printf("COMMENT ");
\.			printf("DOT ");
\!			printf("OP_NOT ");
\=			printf("OP_ASSIGN ");
\+                  	printf("OP_ADD ");
\-                  	printf("OP_SUB ");
\*                  	printf("OP_MUL ");
\*\*			printf("OP_POW ");
\/                  	printf("OP_DIV ");
\%                  	printf("OP_MOD ");
\?			printf("OP_TERNARY ");
\=\=                	printf("OP_EQUAL ");
\<                  	printf("OP_LESS_THAN ");
\<\=                	printf("OP_LESS_THAN_EQUAL ");
\>                  	printf("OP_GREATER_THAN ");
\>\=                	printf("OP_GREATER_THAN_EQUAL ");
\&\&			printf("OP_AND ");
\|\| 			printf("OP_OR ");
\(                  	printf("LEFT_PAR ");
\)                  	printf("RIGHT_PAR ");
\{                  	printf("LEFT_CURLY_BRACKETS ");
\}                  	printf("RIGHT_CURLY_BRACKETS ");
\,                  	printf("COMMA ");
\:                  	printf("COLON ");
\;                  	printf("SEMI_COLON ");
for                 	printf("FOR ");
if                  	printf("IF ");
else                	printf("ELSE ");
func		      	printf("FUNCTION "); 
return              	printf("RETURN ");
{unsecuredUrl}		printf("UNSECURED_URL ");
{securedUrl}		printf("SECURED_URL ");
getTime			printf("GET_TIME ");
connect			printf("CONNECT TO ");
getDataOf		printf("GET_DATA_FROM ");
setDataOf		printf("SET_DATA_OF ");
{sensorType}		printf("SENSOR.%s ", yytext);
sendIntegerToUrl	printf("SEND_INTEGER_TO ");
receiveIntegerFromUrl 	printf("RECEIVE_INTEGER_FROM ");
{id}			printf("IDENTIFIER ");
[ \t\n]             	;
%%
