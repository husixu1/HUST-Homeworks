 /**
  * @file parser.y
  * @brief The yacc file to generate the parser
  *
  * @details
  * build AST from tokens
  *
  * @date 2018-06-11
  * @author Sixu Hu
 **/

%{
#include "scanner.h"
#include "parser.h"
#include "errors.h"
%}

 /* the token type union*/
%union {
    int              integerConstant;
    bool             boolConstant;
    char             *stringConstant;
    double           doubleConstant;
    char             identifier[MAX_IDENTIFIER_LENGTH + 1];
    Decl             *declaration;
    List<Decl*>      *declarationList;
    Type             *type;
    NamedType        *namedType;
    List<NamedType*> *namedTypeList;
    FnDecl           *functoinDeclaration;
    VarDecl          *variableDeclaration;
    List<VarDecl*>   *variableList;
    Expr             *expression;
    List<Expr*>      *expressionList;
    Stmt             *statement;
    List<Stmt*>      *statementList;
    LValue           *lvalue;
}


/* terminal token types without value */
%token tokenVoid tokenBool tokenInt tokenDouble tokenString tokenClass
%token tokenLessEqual tokenGreaterEqual tokenEqual tokenNotEqual tokenDims
%token tokenAnd tokenOr tokenNull tokenExtends tokenThis tokenInterface tokenImplements
%token tokenWhile tokenFor tokenIf tokenElse tokenReturn tokenBreak
%token tokenNew tokenNewArray tokenPrint tokenReadInteger tokenReadLine

/* terminal token types with value */
%token <identifier>      tokenIdentifier
%token <stringConstant>  tokenStringConstant
%token <integerConstant> tokenIntConstant
%token <doubleConstant>  tokenDoubleConstant
%token <boolConstant>    tokenBoolConstant

/* non-terminal token types */
%type <expression>          Constant Expr Call OptExpr
%type <lvalue>              LValue
%type <type>                Type
%type <namedType>           OptExt
%type <namedTypeList>       OptImpl ImpList
%type <declaration>         ClassDecl Decl Field IntfDecl
%type <functoinDeclaration> FnDecl FnHeader
%type <declarationList>     FieldList DeclList IntfList
%type <variableDeclaration> Variable VarDecl
%type <variableList>        Formals FormalList VarDecls
%type <expressionList>      Actuals ExprList
%type <statement>           Stmt StmtBlock OptElse
%type <statementList>       StmtList

/* precedence and associativity */
%left     '='
%left      tokenOr
%left      tokenAnd
%nonassoc  tokenEqual tokenNotEqual
%nonassoc  '<' '>' tokenLessEqual tokenGreaterEqual
%left      '+' '-'
%left      '*' '/' '%'
%nonassoc  tokenUnaryMinus '!'
%nonassoc  '.' '['
%nonassoc  tokenLower_Than_Else
%nonassoc  tokenElse

 /* body ------------------------------------------------------------------- */

%%
Program     : DeclList {
                Program *program = new Program($1);
                if (ReportError::NumErrors() == 0)
                    program->check();
                if (ReportError::NumErrors() == 0)
                    program->generate();
            }
            ;

DeclList    : DeclList Decl {
                ($$=$1)->Append($2);
            }
            | Decl {
                ($$ = new List<Decl*>)->Append($1);
            }
            ;

Decl        : ClassDecl
            | FnDecl {
                $$=$1;
            }
            | VarDecl {
                $$=$1;
            }
            | IntfDecl
            ;

VarDecl     : Variable ';'
            ;

Variable    : Type tokenIdentifier {
                $$ = new VarDecl(new Identifier(@2, $2), $1);
            }
            ;

Type        : tokenInt {
                $$ = Type::intType;
            }
            | tokenBool {
                $$ = Type::boolType;
            }
            | tokenString {
                $$ = Type::stringType;
            }
            | tokenDouble {
                ReportError::Formatted(&@1, "No code gen for doubles");
                $$ = Type::errorType;
            }
            | tokenIdentifier {
                $$ = new NamedType(new Identifier(@1,$1));
            }
            | Type tokenDims {
                $$ = new ArrayType(concatLocation(@1, @2), $1);
            }
            ;

IntfDecl    : tokenInterface tokenIdentifier '{' IntfList '}' {
                $$ = new InterfaceDecl(new Identifier(@2, $2), $4);
            }
            ;

IntfList    : IntfList FnHeader ';' {
                ($$=$1)->Append($2);
            }
            | /* empty */ {
                $$ = new List<Decl*>();
            }
            ;

ClassDecl   : tokenClass tokenIdentifier OptExt OptImpl '{' FieldList '}' {
                $$ = new ClassDecl(new Identifier(@2, $2), $3, $4, $6);
            }
            ;

OptExt      : tokenExtends tokenIdentifier {
                $$ = new NamedType(new Identifier(@2, $2));
            }
            | /* empty */ {
                $$ = NULL;
            }
            ;

OptImpl     : tokenImplements ImpList {
                $$ = $2;
            }
            | /* empty */ {
                $$ = new List<NamedType*>;
            }
            ;

ImpList     : ImpList ',' tokenIdentifier {
                ($$=$1)->Append(new NamedType(new Identifier(@3, $3)));
            }
            | tokenIdentifier      {
                ($$=new List<NamedType*>)->Append(new NamedType(new Identifier(@1, $1)));
            }
            ;

FieldList   : FieldList Field      {
                ($$=$1)->Append($2);
            }
            | /* empty */          {
                $$ = new List<Decl*>();
            }
            ;

Field       : VarDecl {
                $$ = $1;
            }
            | FnDecl {
                $$ = $1;
            }
            ;


FnHeader    : Type tokenIdentifier '(' Formals ')' {
                $$ = new FnDecl(new Identifier(@2, $2), $1, $4);
            }
            | tokenVoid tokenIdentifier '(' Formals ')' {
                $$ = new FnDecl(new Identifier(@2, $2), Type::voidType, $4);
            }
            ;

Formals     : FormalList {
                            $$ = $1;
            }
            | /* empty */ {
                $$ = new List<VarDecl*>;
            }
            ;

FormalList  : FormalList ',' Variable {
                ($$=$1)->Append($3);
            }
            | Variable {
                ($$ = new List<VarDecl*>)->Append($1);
            }
            ;

FnDecl      : FnHeader StmtBlock {
                            ($$=$1)->SetFunctionBody($2);
            }
            ;

StmtBlock   : '{' VarDecls StmtList '}' {
                $$ = new StmtBlock($2, $3);
            }
            ;

VarDecls    : VarDecls VarDecl {
                ($$=$1)->Append($2);
            }
            | /* empty */ {
                $$ = new List<VarDecl*>;
            }
            ;

StmtList    : Stmt StmtList {
                $$ = $2; $$->InsertAt($1, 0);
            }
            | /* empty */ {
                $$ = new List<Stmt*>;
            }
            ;

Stmt        : OptExpr ';' {
                $$ = $1;
            }
            | StmtBlock
            | tokenIf '(' Expr ')' Stmt OptElse {
                $$ = new IfStmt($3, $5, $6);
            }
            | tokenWhile '(' Expr ')' Stmt {
                $$ = new WhileStmt($3, $5);
            }
            | tokenFor '(' OptExpr ';' Expr ';' OptExpr ')' Stmt {
                $$ = new ForStmt($3, $5, $7, $9);
            }
            | tokenReturn Expr ';' {
                $$ = new ReturnStmt(@2, $2);
            }
            | tokenReturn ';' {
                $$ = new ReturnStmt(@1, new EmptyExpr());
            }
            | tokenPrint '(' ExprList ')' ';' {
                $$ = new PrintStmt($3);
            }
            | tokenBreak ';' {
                $$ = new BreakStmt(@1);
            }
            ;

LValue      : tokenIdentifier {
                $$ = new FieldAccess(NULL, new Identifier(@1, $1));
            }
            | Expr '.' tokenIdentifier {
                $$ = new FieldAccess($1, new Identifier(@3, $3));
            }
            | Expr '[' Expr ']' {
                $$ = new ArrayAccess(concatLocation(@1, @4), $1, $3);
            }
            ;

Call        : tokenIdentifier '(' Actuals ')' {
                $$ = new Call(concatLocation(@1,@4), NULL, new Identifier(@1,$1), $3);
            }
            | Expr '.' tokenIdentifier '(' Actuals ')' {
                $$ = new Call(concatLocation(@1,@6), $1, new Identifier(@3,$3), $5);
            }
            ;

OptExpr     : Expr {
                $$ = $1;
            }
            | /* empty */ {
                $$ = new EmptyExpr();
            }
            ;

Expr        : LValue {
                $$ = $1;
            }
            | Call
            | Constant
            | LValue '=' Expr {
                $$ = new AssignExpr($1, new Operator(@2,"="), $3);
            }
            | Expr '+' Expr {
                $$ = new ArithmeticExpr($1, new Operator(@2, "+"), $3);
            }
            | Expr '-' Expr {
                $$ = new ArithmeticExpr($1, new Operator(@2, "-"), $3);
            }
            | Expr '/' Expr {
                $$ = new ArithmeticExpr($1, new Operator(@2,"/"), $3);
            }
            | Expr '*' Expr {
                $$ = new ArithmeticExpr($1, new Operator(@2,"*"), $3);
            }
            | Expr '%' Expr {
                $$ = new ArithmeticExpr($1, new Operator(@2,"%"), $3);
            }
            | Expr tokenEqual Expr {
                $$ = new EqualityExpr($1, new Operator(@2,"=="), $3);
            }
            | Expr tokenNotEqual Expr {
                $$ = new EqualityExpr($1, new Operator(@2,"!="), $3);
            }
            | Expr '<' Expr {
                $$ = new RelationalExpr($1, new Operator(@2,"<"), $3);
            }
            | Expr '>' Expr {
                $$ = new RelationalExpr($1, new Operator(@2,">"), $3);
            }
            | Expr tokenLessEqual Expr {
                $$ = new RelationalExpr($1, new Operator(@2,"<="), $3);
            }
            | Expr tokenGreaterEqual Expr {
                $$ = new RelationalExpr($1, new Operator(@2,">="), $3);
            }
            | Expr tokenAnd Expr {
                $$ = new LogicalExpr($1, new Operator(@2,"&&"), $3);
            }
            | Expr tokenOr Expr {
                $$ = new LogicalExpr($1, new Operator(@2,"||"), $3);
            }
            | '(' Expr ')' {
                $$ = $2;
            }
            | '-' Expr  %prec tokenUnaryMinus {
                $$ = new ArithmeticExpr(new Operator(@1,"-"), $2);
            }
            | '!' Expr {
                $$ = new LogicalExpr(new Operator(@1,"!"), $2);
            }
            | tokenReadInteger '(' ')' {
                $$ = new ReadIntegerExpr(concatLocation(@1,@3));
            }
            | tokenReadLine '(' ')' {
                $$ = new ReadLineExpr(concatLocation(@1,@3));
            }
            | tokenNew '(' tokenIdentifier ')' {
                $$ = new NewExpr(concatLocation(@1,@4),new NamedType(new Identifier(@3,$3)));
            }
            | tokenNewArray '(' Expr ',' Type ')' {
                $$ = new NewArrayExpr(concatLocation(@1,@6),$3, $5);
            }
            | tokenThis {
                $$ = new This(@1);
            }
            ;

Constant    : tokenIntConstant {
                            $$ = new IntConstant(@1,$1);
            }
            | tokenBoolConstant {
                $$ = new BoolConstant(@1,$1);
            }
            | tokenDoubleConstant {
                ReportError::Formatted(&@1, "No code gen for doubles");
                $$ = new IntConstant(@1,$1);
            }
            | tokenStringConstant {
                $$ = new StringConstant(@1,$1);
            }
            | tokenNull {
                $$ = new NullConstant(@1);
            }
            ;

Actuals     : ExprList {
                $$ = $1;
            }
            | /* empty */ {
                $$ = new List<Expr*>;
            }
            ;

ExprList    : ExprList ',' Expr {
                ($$=$1)->Append($3);
            }
            | Expr {
                ($$ = new List<Expr*>)->Append($1);
            }
            ;

OptElse     : tokenElse Stmt {
                $$ = $2;
            }
            | /* empty */ %prec tokenLower_Than_Else {
                $$ = NULL;
            }
            ;

%%

void initParser()
{
   debug("parser", "Initializing parser");
   yydebug = false;
}
