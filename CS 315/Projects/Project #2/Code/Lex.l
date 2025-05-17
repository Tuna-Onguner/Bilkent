%option yylineno
newline 		\n	
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
sensorTemp		TEMPERATURE
sensorLight		LIGHT
sensorHum		HUMIDITY
sensorAirPress		AIR_PRESSURE
sensorAirQual		AIR_QUALITY
sensorSound		SOUND_LEVEL
switch			("switch")
switchOn 		open|true|on|1
switchOff 		close|false|off|0
ws			[ \t\r\s]+$
%%
{newline}		return NL;
int			return TYPE_INT;
float			return TYPE_FLOAT;
bool			return TYPE_BOOL;
char			return TYPE_CHAR;	
string			return TYPE_STRING;
{int}			return CONST_INT;
{float}			return CONST_FLOAT;
{bool}			return CONST_BOOL;
{char}			return CONST_CHAR;
{string}		return CONST_STRING;
"//".*			return COMMENT;
\!\>			return FUNC;
\.			return DOT;
\!			return OP_NOT;
\=			return OP_ASSIGN;
\+                  	return OP_ADD;
\-                  	return OP_SUB;
\*                  	return OP_MUL;
\*\*			return OP_POW;
\/                  	return OP_DIV;
\%                  	return OP_MOD;
\?			return OP_TERNARY;
\=\=                	return OP_EQUAL;
\!\=			return OP_NOT_EQUAL;
\<                  	return OP_LT;
\<\=                	return OP_LTE;
\>                  	return OP_GT;
\>\=                	return OP_GTE;
\&\&			return OP_AND;
\|\| 			return OP_OR;
\(                  	return LP;
\)                  	return RP;
\{                  	return LCB;
\}                  	return RCB;
\,                  	return CM;
\:                  	return CL;
\;                  	return SC;
for                 	return FOR;
if                  	return IF;
else                	return ELSE;
func		      	return FUNCTION; 
return              	return RETURN;
{unsecuredUrl}		return UNSECURED_URL;
{securedUrl}		return SECURED_URL;
getTime			return GET_TIME;
connect			return CONNECT_TO;
getDataOf		return GET_DATA_FROM;
sendIntegerToUrl	return SEND_INTEGER_TO;
receiveIntegerFromUrl 	return RECEIVE_INTEGER_FROM;
{sensorTemp}		return SENSOR_TEMPERATURE;
{sensorLight}		return SENSOR_LIGHT;
{sensorHum}		return SENSOR_HUMIDITY;
{sensorAirPress}	return SENSOR_AIR_PRESSURE;
{sensorAirQual}		return SENSOR_AIR_QUALITY;
{sensorSound}		return SENSOR_SOUND_LEVEL;
{id}			return IDENTIFIER;
{ws}           		{};
%%
int yywrap() { return 1; }