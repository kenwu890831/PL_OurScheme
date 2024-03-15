# include <string>
# include <iostream>
# include <vector>
# include <cstdlib>
# include <iomanip>
# include <stack>
# include <math.h>
# include <queue>
# include <stdio.h>
# include <queue>
# include <sstream>

using namespace std;

enum ErrorType {
  NOTCLOSE,
  UNEXPECTEDTOKEN1,
  UNEXPECTEDTOKEN2,
  NONLIST,
  LEVELERROR,
  FORMATERROR,
  UNBOUNDERROR,
  NONFUNCTION,
  ARGNUMERROR,
  ARGTYPEERROR,
  DIVBYZERO,
  NORETURNVALUE,
  ENDTHEFILE
} ;

enum Token {
  LEFT_PAREN = 87899,       // '('
  RIGHT_PAREN = 45645,      // ')'
  INT = 12354,          // e.g., '123', '+123', '-123'
  STRING = 89382,           // "string's (example)." (strings do not extend across lines)
  DOT = 89746,              // '.'
  FLOAT = 87960,            // '123.567', '123.', '.567', '+123.4', '-.123'
  NIL = 75134,              // 'nil' or '#f', but not 'NIL' nor 'nIL'
  T = 18674,                // 't' or '#t', but not 'T' nor '#T'
  QUOTE = 98721,            // '
  SYMBOL = 64843
};

struct Data {
  string token ;
  int token_type;
  int line ;
  int column ;
};

struct Node {
  bool isEmpty ;
  bool inQuote ;
  string token ;
  int type;
  Node *left ;
  Node * right ;
};

struct Ddefine {
  string token ;
  Node *define_list ;
};

class PrintSExp {
  private:
  int mcount ;
  Node *mroot ;
  int mplace ;
  int mprintCount ;
  bool mlastPrintLeftParen ;
  bool mlastPrintLeftDot ;
  bool mlastIsQuote ;
  bool mfirstIsQuote ;
  bool mlastPrintQuoute ;
  bool mQuoteOne ;
  int mParenCount ;
  bool mInQuote ;
  string mPrintTemp ;
 
  vector<string> *mProcedureList ;
  public:
  bool mIsError ;
  void CreateProcedure() {
    mProcedureList->push_back( "define" ) ;
    mProcedureList->push_back( "list" ) ;
    mProcedureList->push_back( "quote" ) ;
    mProcedureList->push_back( "cons" ) ;
    mProcedureList->push_back( "car" ) ;
    mProcedureList->push_back( "cdr" ) ;
    mProcedureList->push_back( "exit" ) ;
    mProcedureList->push_back( "clean-environment" ) ;
    mProcedureList->push_back( "pair?" ) ;
    mProcedureList->push_back( "null?" ) ;
    mProcedureList->push_back( "integer?" ) ;
    mProcedureList->push_back( "real?" ) ;
    mProcedureList->push_back( "number?" ) ;
    mProcedureList->push_back( "string?" ) ;
    mProcedureList->push_back( "boolean?" ) ;
    mProcedureList->push_back( "symbol?" ) ;
    mProcedureList->push_back( "+" ) ;
    mProcedureList->push_back( "-" ) ;
    mProcedureList->push_back( "*" ) ;
    mProcedureList->push_back( "/" ) ;
    mProcedureList->push_back( "not" ) ;
    mProcedureList->push_back( ">" ) ;
    mProcedureList->push_back( ">=" ) ;
    mProcedureList->push_back( "<" ) ;
    mProcedureList->push_back( "<=" ) ;
    mProcedureList->push_back( "=" ) ;
    mProcedureList->push_back( "string-append" ) ;
    mProcedureList->push_back( "string>?" ) ;
    mProcedureList->push_back( "string<?" ) ;
    mProcedureList->push_back( "string=?" ) ;
    mProcedureList->push_back( "string<=?" ) ;
    mProcedureList->push_back( "string>=?" ) ;
    mProcedureList->push_back( "integer>?" ) ;
    mProcedureList->push_back( "integer<?" ) ;
    mProcedureList->push_back( "integer=?" ) ;
    mProcedureList->push_back( "integer<=?" ) ;
    mProcedureList->push_back( "integer>=?" ) ;
    mProcedureList->push_back( "real>?" ) ;
    mProcedureList->push_back( "real<?" ) ;
    mProcedureList->push_back( "real=?" ) ;
    mProcedureList->push_back( "real<=?" ) ;
    mProcedureList->push_back( "real>=?" ) ;
    mProcedureList->push_back( "number>?" ) ;
    mProcedureList->push_back( "number<?" ) ;
    mProcedureList->push_back( "number=?" ) ;
    mProcedureList->push_back( "number<=?" ) ;
    mProcedureList->push_back( "number>=?" ) ;
    mProcedureList->push_back( "eqv?" ) ;
    mProcedureList->push_back( "equal?" ) ;
    mProcedureList->push_back( "if" ) ;
    mProcedureList->push_back( "cond" ) ;
    mProcedureList->push_back( "begin" ) ;
    mProcedureList->push_back( "and" ) ;
    mProcedureList->push_back( "or" ) ;
    mProcedureList->push_back( "atom?" ) ;
    mProcedureList->push_back( "list?" ) ;
  } // CreateProcedure()

  bool IsProcedure( string input ) {
    for ( int i = 0 ; i < mProcedureList->size() ; i++ ) {
      if ( mProcedureList->at( i ) == input ) 
        return true ;
    } // for

    return false ;
  } // IsProcedure()
  

  void Reset() {
    mProcedureList = new vector <string>() ;
    CreateProcedure() ;
  } // Reset()

  void PrintString( string input ) {
    bool slashCheck = false ;
    for ( int i = 0 ; i < input.size() ; i++ ) {
      if ( slashCheck ) {
        if ( input[i] == '\\' ) {
          // cout << '\\' ;
          mPrintTemp = mPrintTemp + "\\" ;
        } // if
        else if ( input[i] == 'n' ) {
          // cout << endl;
          mPrintTemp = mPrintTemp + "\n" ;
        } // else if
        else if ( input[i] == '\"' ) {
          // cout << "\"" ;
          mPrintTemp = mPrintTemp + "\"" ;
        } // else if
        else if ( input[i] == '\'' ) {
          // cout << "\'" ;
          mPrintTemp = mPrintTemp + "\'" ;
        } // else if
        else if ( input[i] == '\t' ) {
          // cout << "\t" ;
          mPrintTemp = mPrintTemp + "\t" ;
        } // else if
        else {
          // cout << "\\" << input[i] ;
          mPrintTemp = mPrintTemp + "\\" + input[i] ;
        } // else
        
        slashCheck = false ;
      } // if
      else {
        if ( input[i] == '\\' ) {
          slashCheck = true ;
        } // if
        else {
          mPrintTemp = mPrintTemp + input[i] ;
        } // else
      } // else
    } // for
  } // PrintString()

  void PrintNode( string inputSting, int inputType ) {
    // cout << "printf -------- " << endl ;
    if ( inputType == STRING ) {
      // cout << "\"" ;
      mPrintTemp = mPrintTemp + "\"" ;
      PrintString( inputSting ) ;
      // cout <<  "\"" ;
      mPrintTemp = mPrintTemp + "\"" ;
    } // if
    else if ( inputType == FLOAT ) {
      // cout << "F" << endl ;
      bool dotBack = false ;
      int dotBackCount = 0 ;
      string dotBackTemp = "" ;
      for ( int i = 0 ; i < inputSting.size() ; i++ ) {
        // cout <<"in" << endl ;
        if ( dotBack && dotBackCount < 4 ) {
                        // cout <<"in2" << endl ;
          dotBackTemp = dotBackTemp + inputSting[i] ;
          dotBackCount ++ ;
        } // if
        else if ( !dotBack ) {
          if ( inputSting[i] == '.' ) {
            dotBack = true ;
            if ( i == 0 ) 
              // cout << '0'  ;
              mPrintTemp = mPrintTemp + "0" ;
            else if ( inputSting[i-1] == '+' || inputSting[i-1] == '-' ) 
              // cout << '0'  ;
              mPrintTemp = mPrintTemp + "0" ;
          } // if

          if ( inputSting[i] != '+' )
            // cout << inputSting[i]  ;
            mPrintTemp = mPrintTemp + inputSting[i] ;
        } // else if
      } // for

      if ( dotBackCount > 3 ) {
        if ( dotBackTemp[3] >= '5' ) {
          dotBackTemp[2] = dotBackTemp[2] +1 ;
        } // if
      } // if 

      if ( dotBackCount < 3 ) {
        for ( int i = 3 ; dotBackCount < i ; dotBackCount++ ) {
          dotBackTemp = dotBackTemp + '0' ;
        } // for
      } // if

      for ( int i = 0 ; i < 3 ; i++ ) {
        // cout << dotBackTemp[i]  ;
        mPrintTemp = mPrintTemp + dotBackTemp[i] ;
      } // for

    } // else if
    else if ( inputType == NIL ) {
      
      // if ( !ghavecout )
      // cout << "nil"  ;
      mPrintTemp = mPrintTemp + "nil" ;
    } // else if
    else if ( inputType == T ) {
      // if ( !ghavecout )
      // cout << "#t"  ;
      mPrintTemp = mPrintTemp + "#t" ;
    } // else if
    else if ( inputType == INT ) {
      // cout << "INT" << endl ;
      for ( int i = 0 ; i < inputSting.size() ; i++ ) {
        if ( inputSting[i] == '.' )
          i = inputSting.size() ;
        if ( i == 0 && inputSting[i] == '+' ) ;
        else if ( i < inputSting.size() ) {
          // cout << inputSting[i]  ; 
          mPrintTemp = mPrintTemp + inputSting[i] ;
        } // else if
          
      } // for
    } // else if
    else if ( inputType == QUOTE ) {
      // cout  << "quote"  ;
      mPrintTemp = mPrintTemp + "quote" ;
    } // else if
    else {
    
      if ( !mInQuote && IsProcedure( inputSting ) && !mIsError ) {
        // cout << "#<procedure " << inputSting << ">"  ;
        mPrintTemp = mPrintTemp + "#<procedure " + inputSting + ">" ;
      } // if
      else {
        // cout  << inputSting  ;
        mPrintTemp = mPrintTemp + inputSting ;
      } // else
    } // else
            
    // cout << endl ;
    mPrintTemp = mPrintTemp + "\n" ;
  } // PrintNode()

  void PrintResult() {
    Node *walk = mroot ;
    if ( walk -> inQuote == true )
      mInQuote = true ;
    else
      mInQuote = false ; 
    // cout << mInQuote << endl ;
    PrintNode( walk->token, walk->type ) ;
  } // PrintResult()

  void PrintSpace() {
    for ( int i = 0 ; i < mprintCount *2 ; i++ )
      // cout << " " ;
      mPrintTemp = mPrintTemp + " " ;
  } // PrintSpace()

  void PrintResult1( Node * walk ) {
    if ( walk == NULL ) {
      return ;
    } // if

    
    if ( walk -> left != NULL && walk -> left -> isEmpty == true ) {
      
      if ( !mlastPrintLeftParen )
        PrintSpace() ;
      // cout << "( " ;
      mPrintTemp = mPrintTemp + "( " ;
      mprintCount++ ;
      mlastPrintLeftParen = true ;
    } // if

    PrintResult1( walk->left );
    if ( walk -> isEmpty == false ) {
      if ( mlastPrintLeftDot ) {
        mlastPrintLeftDot = false ;
      } // if
      else {
        if ( !mlastPrintLeftParen )
          PrintSpace() ;  
        if ( walk -> inQuote == true )
          mInQuote = true ;
        else
          mInQuote = false ;
        // cout << walk->token << endl ;
        // cout << "111" << walk->inQuote << endl ;
        PrintNode( walk->token, walk->type ) ;
        mlastPrintLeftParen = false ;
      } // else

    } // if  

    if ( walk -> left != NULL && walk -> left -> isEmpty == true ) {
      mprintCount-- ;
      PrintSpace() ;
      // cout << ")" << endl ;
      mPrintTemp = mPrintTemp + ")\n" ;
      mlastPrintLeftParen = false ;
    } // if

    if ( walk -> right != NULL && walk -> right -> isEmpty == false ) {
      if ( walk -> right -> type == NIL ) {
        mlastPrintLeftDot = true ;
      } // if
      else {
        PrintSpace() ;
        // cout << "." << endl ;
        mPrintTemp = mPrintTemp + ".\n" ;
        mlastPrintLeftParen = false ;
      } // else
      
    } // if

    

    PrintResult1( walk->right );
            
  } // PrintResult1()


  void PrintExp( Node* root2 ) {
    // cout << " in P exp1 " << endl  ;
    mPrintTemp = "" ;
    if ( root2 -> isEmpty == false ) {
      // cout << "single" << endl ;
      mroot = new Node() ;
      mroot-> token = root2 ->token ;
      mroot-> type = root2 -> type ;
      mroot ->inQuote = root2 ->inQuote ;
      mroot-> left = NULL ;
      mroot-> right = NULL ;
      PrintResult() ;
      // PrintNode( mroot-> token, mroot-> type) ;
      delete mroot;
    } // if
    else {

      mprintCount = 1 ;
      mlastPrintLeftParen = true ;
      mlastPrintLeftDot = false ;
      mlastIsQuote = false ;
      mlastPrintQuoute = false ;
      mQuoteOne = false ;
      // mroot = CreateTree( mreadList ) ;
      Node *walk = root2;
      // cout << "( " ;
      mPrintTemp = mPrintTemp + "( " ;
      PrintResult1( walk ) ;
      // cout << ")" << endl ;
      mPrintTemp = mPrintTemp + ")\n" ;
    } // else (have PAREN)
            
    
  } // PrintExp()

  void PrintLast() {
    cout << mPrintTemp ;
  } // PrintLast()

  string GetList() {
    return mPrintTemp ;
  } // GetList()
};

class ReadSExp {
  private:
  bool mIsExit ;
  bool mEOF ;
  Data mtemp ;
  bool mNeedReturn ;
  int mcolumn ;
  int mline ;
  char mpreinput ;
  bool misComment ;
  bool mstring_write ;
  string mErrorProcedure ;
  int mErrorLine ;
  int mErrorColumn ;
  bool mIsFirst ;
  bool mHaveToken ;
  bool mlastPrintQuoute ;
  bool mFirstIsQuote ;
  int mrun ;
  PrintSExp mp1 ;
  Node* mErrorNode ;
  vector<Ddefine> *mDefineList ;

  public :
  bool mlastIsError ;
  vector<Data> *mreadList ;
  Node* mTreeHead ;
  bool mNotPrint ;

  void Reset() {
    mp1.Reset() ;
    mlastPrintQuoute = false ;
    mIsExit = false ;
    mEOF = false ;
    mDefineList = new vector <Ddefine>() ;
    mreadList = new vector <Data>() ;
    mlastIsError = false ;
    mNeedReturn = false ;
    mstring_write = false ;
    misComment = false ;
    mline = 1 ;
    mcolumn = 1 ;
    mIsExit = false ;
    mEOF = false ;
    mpreinput = '\0' ;
    mIsFirst = false ;
  } // Reset()

  void LineRest() {
    mline = 1 ;
    mcolumn = 0 ;
    mIsFirst = true ;
    mHaveToken = false ;
    mreadList->clear() ;
  } // LineRest()
  
  bool Exit() {
    if ( mIsExit ) {
      return false ;
    } // if

    if ( mEOF ) {
      cout << "ERROR (no more input) : END-OF-FILE encountered" ;
      return false ;
    } // if


    return true ;
  } // Exit()

  Node* CreateTree( vector <Data> *tree ) { // create quote tree ex: ( 1 2 3 )
    bool rightCheck = false ;
    bool needReturn = false ;
    Node *temp = new Node() ;
    temp -> isEmpty = true ;
    temp -> inQuote = false ;
    temp -> type = 0 ;
    temp -> right = NULL ;
    temp ->left = NULL ;
    Node *walk = temp ;
    if ( mlastPrintQuoute ) {
      walk -> right = NULL ;
      walk -> left = new Node() ;
      walk -> left -> isEmpty = false ;
      walk -> left -> token = "quote" ;
      walk -> left -> type = QUOTE ;
      walk -> left -> inQuote = false ;
      walk -> left -> right = NULL ;
      walk -> left -> left = NULL ;
      mlastPrintQuoute = false ;
      mrun ++ ;
      needReturn = true ;
    } // if

    while ( mrun < mreadList ->size() ) {
      if ( mreadList -> at( mrun ).token_type == LEFT_PAREN ) {
        if ( rightCheck ) {
          // cout << "left paren right" << endl ;
          mrun++ ;
          walk -> right = CreateTree( tree ) ;
          rightCheck = false ;
        } // if
        else {
          if ( walk -> left == NULL ) {
            // cout << "left paren left" << endl ;
            mrun++ ;
            walk -> left = CreateTree( tree ) ;
          } // if
          else {
            // cout << "left paren left create" << endl ;
            walk -> right = new Node() ;
            walk = walk -> right ;
            walk -> isEmpty = true ;
            walk -> inQuote = false ;
            walk -> type = 0 ;
            walk -> right = NULL ;
            walk -> left = NULL ;
            mrun++ ;
            walk -> left = CreateTree( tree ) ;
          } // else
        } // else

        if ( needReturn ) {
          return temp ;
        } // if
      } // if
      else if ( mreadList -> at( mrun ).token_type == QUOTE  ) {
        if ( mFirstIsQuote ) {
          // cout << "quote first" << endl ;
          walk -> left = new Node() ;
          walk -> left -> isEmpty = false ;
          walk -> left -> token = "quote" ;
          walk -> left -> type = QUOTE ;
          walk -> left -> inQuote = false ;
          walk -> left -> right = NULL ;
          walk -> left -> left = NULL ;
          mFirstIsQuote = false ;
          needReturn = true ;
        } // if
        else {
          
          mlastPrintQuoute = true ;
          if ( rightCheck ) {
            // cout << "quote right" << endl ;
            walk -> right = CreateTree( tree ) ;
            rightCheck = false ;
          } // if
          else {
            if ( walk -> left == NULL ) {
              // cout << "quote left" << endl ;
              mlastPrintQuoute = true ;
              walk -> left = CreateTree( tree ) ;
            } // if
            else {
              // cout << "quote left create" << endl ;
              walk -> right = new Node() ;
              walk = walk -> right ;
              walk -> isEmpty = true ;
              walk -> inQuote = false ;
              walk -> type = 0 ;
              walk -> right = NULL ;
              walk -> left = NULL ;
              mlastPrintQuoute = true ;
              walk -> left = CreateTree( tree ) ;
            } // else
          } // else


          if ( needReturn ) {
            return temp ;
          } // if
        } // else


      } // else if
      else if ( mreadList -> at( mrun ).token_type == RIGHT_PAREN  ) {
        // cout << "right paren" << endl ;
        // cout << temp -> left -> token << endl ;
        return temp ;
      } // else if
      else if ( mreadList -> at( mrun ).token_type == DOT  ) {
        // cout << "dot" << endl ;
        rightCheck = true ;
      } // else if
      else {
        // cout << "qq" << endl ;
        if ( rightCheck ) {
          // cout << "symbol right" << endl ;
          if ( mreadList -> at( mrun ).token_type == NIL ) {
            walk -> right = NULL ;
          } // if 
          else {
            walk -> right = new Node() ;
            walk -> right -> token = mreadList -> at( mrun ).token ;
            walk -> right -> type = mreadList -> at( mrun ).token_type ;
            walk -> right -> isEmpty = false ;
            walk -> right -> inQuote = false ;
            walk -> right -> right = NULL ;
            walk -> right -> left = NULL ;
          } // else

          rightCheck = false ;
        } // if
        else {
          if ( walk -> left == NULL ) {
            // cout << "symbol left " << endl ;
            walk -> left = new Node() ;
            walk -> left -> token = mreadList -> at( mrun ).token ;
            walk -> left -> type = mreadList -> at( mrun ).token_type ;
            walk -> left -> isEmpty = false ;
            walk -> left -> inQuote = false ;
            walk -> left -> right = NULL ;
            walk -> left -> left = NULL ;
          } // if
          else {

            // cout << "symbol left create" << endl ;
            walk -> right = new Node() ;
            walk = walk -> right ;
            walk -> inQuote = false ;
            walk -> type = 0 ;
            walk -> right = NULL ;
            walk -> left = NULL ;
            walk -> isEmpty = true ;
            walk -> left = new Node() ;
            walk -> left -> token = mreadList -> at( mrun ).token ;
            walk -> left -> type = mreadList -> at( mrun ) .token_type ;
            walk -> left -> isEmpty = false ;
            walk -> left -> inQuote = false ;
            walk -> left -> right = NULL ;
            walk -> left -> left = NULL ;
          } // else
        } // else

        if ( needReturn ) {
          return temp ;
        } // if
      } // else

      mrun++ ;
    } // while

    return temp ;
  } // CreateTree() 

  void CreateMainTree() {
    // cout << "www" << endl ;
    if ( mreadList ->at( 0 ).token_type == LEFT_PAREN ) {
      // cout << "eft" << endl ;
      mrun = 1 ;
      mFirstIsQuote = false ;
    } // if
    else if ( mreadList ->at( 0 ).token_type == QUOTE ) {
      // cout << "quote " << endl ;
      mrun = 0 ;
      mFirstIsQuote = true ;
    } // else if
    else {
      mTreeHead = new Node() ;
      mTreeHead -> isEmpty = false ;
      mTreeHead -> right = NULL ;
      mTreeHead -> inQuote = false ;
      mTreeHead ->left = NULL ;
      mTreeHead -> type = mreadList ->at( 0 ).token_type ;
      mTreeHead -> token = mreadList ->at( 0 ).token ;
      return ;
    } // else

    mTreeHead = CreateTree( mreadList ) ;
    
  } // CreateMainTree()

  string IntRule( Data input ) {
    string retuneTemp = "" ;
    for ( int i = 0 ; i < input.token.size() ; i++ ) {
      if ( input.token[i] == '.' )
        i = input.token.size() ;
      else
        retuneTemp = retuneTemp + input.token[i] ;
    } // for


    return retuneTemp ;
  } // IntRule()

  string IntCal( Data input ) {
    bool dotBack = false ;
    int dotBackCount = 0 ;
    string retuneTemp = "" ;
    string dotBackTemp = "" ;
    for ( int i = 0 ; i < input.token.size() ; i++ ) {
      if ( dotBack && dotBackCount < 1 ) {
        dotBackTemp = dotBackTemp + input.token[i] ;
        dotBackCount ++ ;
      } // if
      else if ( !dotBack ) {
        if ( input.token[i] == '.' ) {
          dotBack = true ;
          if ( i == 0 ) 
            retuneTemp = retuneTemp + "0"  ;
          else if ( input.token[i-1] == '+' || input.token[i-1] == '-' ) 
            retuneTemp = retuneTemp + "0"  ;
        } // if

        if ( input.token[i] != '+' )
          retuneTemp = retuneTemp + input.token[i] ;
      } // else if 
    } // for

    bool needPlus = false ;
    if ( dotBackTemp[0] >= '5' ) {
      needPlus = true ;  
    } // if

    // cout << dotBackTemp << endl ;
    if ( needPlus ) {
      needPlus = false ;
      for ( int i = retuneTemp.size()-2 ; i >= 0 ; i-- ) {
        retuneTemp[i] = retuneTemp[i] + 1 ;
        if ( retuneTemp[i] > '9' ) {
          retuneTemp[i] = '0' ;
          if ( i == 0 )
            needPlus = true ;
        } // if
        else 
          i = -1 ;
      } // for

      if ( needPlus )
        retuneTemp = "1" + retuneTemp ;
    } // if

    // cout << retuneTemp << endl ;
    return retuneTemp ;
  } // IntCal()

  string FloatRule( Data input ) {
    bool dotBack = false ;
    int dotBackCount = 0 ;
    string retuneTemp = "" ;
    string dotBackTemp = "" ;
    for ( int i = 0 ; i < input.token.size() ; i++ ) {
      if ( dotBack && dotBackCount < 4 ) {
        dotBackTemp = dotBackTemp + input.token[i] ;
        dotBackCount ++ ;
      } // if
      else if ( !dotBack ) {
        if ( input.token[i] == '.' ) {
          dotBack = true ;
          if ( i == 0 ) 
            retuneTemp = retuneTemp + "0"  ;
          else if ( input.token[i-1] == '+' || input.token[i-1] == '-' ) 
            retuneTemp = retuneTemp + "0"  ;
        } // if

        if ( input.token[i] != '+' )
          retuneTemp = retuneTemp + input.token[i] ;
      } // else if 
    } // for

    bool needPlus = false ;
    if ( dotBackCount > 3 ) {
      if ( dotBackTemp[3] >= '5' ) {
        for ( int i = 2 ; i >= 0 ; i-- ) {
          dotBackTemp[i] = dotBackTemp[i] + 1 ;
          if ( dotBackTemp[i] > '9' ) {
            dotBackTemp[i] = '0' ;
            if ( i == 0 )
              needPlus = true ;
          } // if
          else 
            i = -1 ;
            
        } // for
        
      } // if
    } // if 

    // cout << dotBackTemp << endl ;
    if ( needPlus ) {
      needPlus = false ;
      for ( int i = retuneTemp.size()-2 ; i >= 0 ; i-- ) {
        retuneTemp[i] = retuneTemp[i] + 1 ;
        if ( retuneTemp[i] > '9' ) {
          retuneTemp[i] = '0' ;
          if ( i == 0 )
            needPlus = true ;
        } // if
        else 
          i = -1 ;
      } // for

      if ( needPlus )
        retuneTemp = "1" + retuneTemp ;
    } // if

    if ( dotBackCount < 3 ) {
      for ( int i = 3 ; dotBackCount < i ; dotBackCount++ ) {
        dotBackTemp = dotBackTemp + '0' ;
      } // for
    } // if

    for ( int i = 0 ; i < 3 ; i++ ) {
      retuneTemp = retuneTemp + dotBackTemp[i]  ;
    } // for

    // cout << retuneTemp << endl ;
    return retuneTemp ;
  } // FloatRule() 
  
  Token CheckType( Data input ) {
    bool haveInt = false ;
    if ( input.token == "." )
      return DOT ;
    bool isNum = true ;
    int dotCount = 0 ;
    if ( input.token == "nil" || input.token == "#f" )
      return NIL ;
    if ( input.token == "t" || input.token == "#t" )
      return T ;
    for ( int i = 0 ; i < input.token.size() ; i++ ) {
      if ( !haveInt ) {
        if ( input.token[i] <= '9' &&  input.token[i] >= '0' )
          haveInt = true ;
      } // if

      if ( i == 0 ) {
        if ( ( input.token[i] > '9' || input.token[i] < '0' )
             && input.token[i] != '.' && input.token[i] != '+' && input.token[i] != '-' ) {
          isNum = false;
        } // if 
                        
      } // if
      else if ( ( input.token[i] > '9' ||  input.token[i] < '0' ) && input.token[i] != '.' )
        isNum = false ;

      if ( input.token[i] == '.' )
        dotCount++ ;
    } // for

    if ( haveInt && isNum && dotCount <= 1 ) {
      if ( dotCount == 1 ) {
        mtemp.token = FloatRule( mtemp ) ;
        return FLOAT ;
      } // if
        
      return INT ;
    } // if

    return SYMBOL ;
  } // CheckType()

  Data GetToken() {
    char ch = '\0' ;
    mtemp.token = "" ;
    int tempLine ;
    int tempColumn ;
    if ( mNeedReturn ) {
      if ( mcolumn == 0 )
        mcolumn++ ;
      mNeedReturn = false ;
      if ( mpreinput == '(' ) {
        mtemp.token = "(" ;
        mtemp.token_type = LEFT_PAREN ;
        mtemp.line = mline ;
        mtemp.column = mcolumn ;
        // CheckToken( mtemp ) ;
        return mtemp ;
      } // if
      else if ( mpreinput == ')' ) {
        mtemp.token = ")" ;
        mtemp.token_type = RIGHT_PAREN ;
        mtemp.line = mline ;
        mtemp.column = mcolumn ;
        // CheckToken( mtemp ) ;
        return mtemp ;
      } // else if
      else if ( mpreinput == '\'' ) {
        mtemp.token = "\'" ;
        mtemp.token_type = QUOTE ;
        mtemp.line = mline ;
        mtemp.column = mcolumn ;
        // CheckToken( mtemp ) ;
        return mtemp ;
      } // else if
    } // if 

    mlastIsError = false ;
    mIsFirst = false ;
    mNeedReturn = false ;

    if ( mpreinput == ' ' && mcolumn == 0 && !mIsFirst ) 
      mcolumn++ ;
    else
      mlastIsError = false ;

    while ( scanf( "%c", &ch ) == true ) {
      if ( ch != ' ' && ch != '\n' && ch != ';' && !misComment )
        mHaveToken = true ;
      // the first input
      mcolumn ++ ;
      // cout << ch << endl ;
      if ( mstring_write ) {
        if ( ch == '\"' && mpreinput != '\\' ) {
          mstring_write = false ;
          mtemp.token_type = STRING ;
          // CheckToken( mtemp ) ;
          return mtemp ;
        } // if
        else if ( ch == '\n' ) {
          // cout << "ERROR (no closing quote)" << endl ;
          mstring_write = false ;
          mpreinput = ch ;
          throw NOTCLOSE ;
        } // else if
        else {
          mtemp.token = mtemp.token + ch ;
        } // else
      } // if
      else if ( misComment ) {
        if ( ch == '\n' ) {
          if ( mHaveToken )
            mline++ ;
          else {
            mHaveToken = true ;
            mIsFirst = true ;
          } // else
            
          mcolumn = 0 ;
          misComment = false ;
          
        } // if
          
      } // else if
      else if ( ch == '(' || ch == ')' || ch == '\'' ) {
        if ( mtemp.token != "" ) {
          mNeedReturn = true ;
          mtemp.token_type = CheckType( mtemp ) ;
          mpreinput = ch ;
          mtemp.line = tempLine ;
          mtemp.column = tempColumn ;
          // CheckToken( mtemp ) ;
          return mtemp ;
        } // if
        else if ( ch == '(' ) {
          mtemp.token = "(" ;
          mtemp.token_type = LEFT_PAREN ;
          mpreinput = ch ;
          mtemp.line = mline ;
          mtemp.column = mcolumn ;
          // CheckToken( mtemp ) ;
          return mtemp ;
        } // else if
        else if ( ch == ')' ) {
          mtemp.token = ")" ;
          mtemp.token_type = RIGHT_PAREN ;
          mpreinput = ch ;
          mtemp.line = mline ;
          mtemp.column = mcolumn ;
          // CheckToken( mtemp ) ;
          return mtemp ;
        } // else if
        else if ( ch == '\'' ) {
          mtemp.token = "\'" ;
          mtemp.token_type = QUOTE ;
          mpreinput = ch ;
          mtemp.line = mline ;
          mtemp.column = mcolumn ;
          // CheckToken( mtemp ) ;
          return mtemp ;
        } // else if

      } // if
      else if ( ch == ';' ) {
        misComment = true ;
        if ( mtemp.token != "" ) {
          mtemp.token_type = CheckType( mtemp ) ;
          mpreinput = ch ;
          mtemp.line = tempLine ;
          mtemp.column = tempColumn ;
          // CheckToken( mtemp ) ;
          return mtemp ;
        } // if
      } // if
      else if ( ch == '\"' ) {
        mstring_write = true ;
        if ( mtemp.token != "" ) {
          mtemp.token_type = CheckType( mtemp ) ;
          mpreinput = ch ;
          mtemp.line = tempLine ;
          mtemp.column = tempColumn ;
          // CheckToken( mtemp ) ;
          return mtemp ;
        } // if
      } // if
      else if ( ch == ' ' ) {
        if ( mtemp.token != "" ) {
          mtemp.token_type = CheckType( mtemp ) ;
          mpreinput = ch ;
          mtemp.line = tempLine ;
          mtemp.column = tempColumn ;
          // cout<< mtemp.token << "   " << mtemp.line << "   " << mtemp.column << endl;
          // CheckToken( mtemp ) ;
          return mtemp ;
        } // if
      } // if
      else if ( ch == '\n' ) {
        if ( mHaveToken )
          mline++ ;
        else
          mHaveToken = true ;
        mcolumn = 0 ;
        if ( mtemp.token != "" ) {
          mtemp.token_type = CheckType( mtemp ) ;
          mpreinput = ch ;
          // cout << " enter" << endl ;
          mtemp.line = tempLine ;
          mtemp.column = tempColumn ;
          // CheckToken( mtemp ) ;
          return mtemp ;
        } // if
      } // if
      else {
        if ( mtemp.token == "" ) {
          tempLine = mline ;
          tempColumn = mcolumn ;
        } // if

        mtemp.token = mtemp.token + ch ;
      } // else

      // get token



      mpreinput = ch ;
    } // while

    mEOF = true ;
    throw ENDTHEFILE ;
    // CheckToken( mtemp ) ;
    return mtemp ;
  } // GetToken()


  bool GetExp( int i ) {
    if ( i != 1 ) {
      mtemp = GetToken() ;
      // cout<< mtemp.token << "   " << mtemp.line << "   " << mtemp.column << endl;
      mreadList->push_back( mtemp ) ;
    } // if

    if ( IsAtom( mtemp.token_type ) ) {
      return true ;
    } // if
    else if ( mtemp.token_type == LEFT_PAREN ) {
      mtemp = GetToken() ;
      // cout<< mtemp.token << "   " << mtemp.line << "   " << mtemp.column << endl;
      mreadList->push_back( mtemp ) ;
      if ( mtemp.token_type == RIGHT_PAREN ) {
        mreadList->pop_back() ;
        mreadList->pop_back() ;
        Data temp ;
        temp.token = "()" ;
        temp.token_type = NIL ;
        mreadList->push_back( temp ) ;
        return true ;
      } // if
      else if ( IsAtom( mtemp.token_type ) ) {   
      } // else if
      else if ( mtemp.token_type == QUOTE ) {
        GetExp( 0 ) ;
      } // else if
      else if ( mtemp.token_type == LEFT_PAREN ) {
        GetExp( 1 ) ;
      } // else if
      else {
        mErrorProcedure = mtemp.token ;
        mErrorLine = mtemp.line ;
        mErrorColumn = mtemp.column ;
        throw UNEXPECTEDTOKEN2 ;
      } // else

      GetExpWhile() ;

      if (  mtemp.token_type == DOT ) {
        GetExp( 0 ) ;
        mtemp = GetToken() ;
        // cout<< mtemp.token << "   " << mtemp.line << "   " << mtemp.column << endl;
        mreadList->push_back( mtemp ) ;
        if ( mtemp.token_type == RIGHT_PAREN ) {
          return true ;
        } // if
        else {
          mErrorProcedure = mtemp.token ;
          mErrorLine = mtemp.line ;
          mErrorColumn = mtemp.column ;
          throw UNEXPECTEDTOKEN1 ;
        } // else
      } // if

      else if ( mtemp.token_type == RIGHT_PAREN ) {
        return true ;
      } // else if
      else {
        mErrorProcedure = mtemp.token ;
        mErrorLine = mtemp.line ;
        mErrorColumn = mtemp.column ;
        throw UNEXPECTEDTOKEN2 ;
      } // else

    } // else if
    else if ( mtemp.token_type == QUOTE ) {
      GetExp( 0 ) ;
      return true ;
    } // else if

    if ( i != 2 ) {
      mErrorProcedure = mtemp.token ;
      mErrorLine = mtemp.line ;
      mErrorColumn = mtemp.column ;
      throw UNEXPECTEDTOKEN2 ;
    } // if
      
    return false ;
  } // GetExp()

  void GetExpWhile() {
    
    while ( GetExp( 2 ) ) {
    } // while

  } // GetExpWhile()


  bool IsAtom( int type ) {
    // cout << "in" << endl ;
    if ( type == SYMBOL || type == INT 
         || type == FLOAT || type == STRING || type == NIL || type == T ) {
      return true;
    } // if 
                
    return false ;
  } // IsAtom()

  void ClearLine() {
    // cout << "\'" << mpreinput << "\'" << endl ;
    char ch ;
    if ( mpreinput != '\n' ) {
      while ( scanf( "%c", &ch ) == true ) {
        mpreinput = '\n' ;
        if ( ch == '\n' )
          return ;
      } // while
    } // if
  } // ClearLine()

  void PrintError( ErrorType errorType ) {
    if ( errorType ==  NOTCLOSE ) {
      cout << "ERROR (no closing quote) : END-OF-LINE encountered at Line " << mline ;
      cout << " Column " << mcolumn << endl ;
      ClearLine() ;
    } // if
    else if ( errorType ==  UNEXPECTEDTOKEN1 ) {
      cout << "ERROR (unexpected token) : ')' expected when token at Line " << mErrorLine ;
      cout << " Column " << mErrorColumn << " is >>" << mErrorProcedure << "<<" << endl ;
      ClearLine() ;
    } // else if
    else if ( errorType ==  UNEXPECTEDTOKEN2 ) {
      cout << "ERROR (unexpected token) : atom or '(' expected when token at Line " << mErrorLine ;
      cout << " Column " << mErrorColumn << " is >>" << mErrorProcedure << "<<" << endl ;
      ClearLine() ;
    } // else if
    else if ( errorType ==  NONLIST ) {
      mp1.mIsError = true ;
      cout << "ERROR (non-list) : " ;
      mp1.PrintExp( mErrorNode ) ;
      mp1.PrintLast() ;
      
    } // else if
    else if ( errorType ==  LEVELERROR ) {
      mp1.mIsError = true ;
      cout << "ERROR (level of " << mErrorProcedure << ")" << endl ;
    } // else if
    else if ( errorType ==  FORMATERROR ) {
      mp1.mIsError = true ;
      cout << "ERROR (" << mErrorProcedure << " format) : " ;
      mp1.PrintExp( mErrorNode ) ;
      mp1.PrintLast() ;
      mp1.mIsError = false ;
    } // else if
    else if ( errorType ==  UNBOUNDERROR ) {
      cout << "ERROR (unbound symbol) : " << mErrorProcedure << endl ;
    } // else if
    else if ( errorType ==  NONFUNCTION ) {
      // mp1.mIsError = true ;
      mp1.mIsError = false ;
      cout << "ERROR (attempt to apply non-function) : " ;
      mp1.PrintExp( mErrorNode ) ;
      mp1.PrintLast() ;
      // mp1.mIsError = false ;
      
    } // else if
    else if ( errorType == ARGNUMERROR ) {
      cout << "ERROR (incorrect number of arguments) : " ;
      cout  << mErrorProcedure << endl;
    } // else if
    else if ( errorType ==  ARGTYPEERROR ) {
      cout << "ERROR (" ;
      cout << mErrorProcedure ;
      cout << " with incorrect argument type) : "  ;
      mp1.PrintExp( mErrorNode ) ;
      mp1.PrintLast() ;
    } // else if
    else if ( errorType ==  DIVBYZERO ) {
      mp1.mIsError = true ;
      cout << "ERROR (division by zero) : /" << endl ;
    } // else if
    else if ( errorType ==  NORETURNVALUE ) {
      mp1.mIsError = true ;
      cout << "ERROR (no return value) : " ;
      mp1.PrintExp( mErrorNode ) ;
      mp1.PrintLast() ;
      mp1.mIsError = false ;
    } // else if

    mErrorProcedure = "";
    
  } // PrintError()

  void CheckList() {
    // cout << "check list" << endl << endl ;
    // cout << mreadList -> size() << endl; 
    for ( int i = 0 ; i < mreadList -> size() ; i++ ) {
      cout << mreadList -> at( i ).token << "   " ;
    } // for

  } // CheckList()


  bool CheckIsList( Node* head, int & count ) {
    Node* walk = head ;
    walk = walk -> right ;
    while ( walk != NULL ) {
      if ( walk -> isEmpty == false  )
        return false ;
      walk = walk -> right ;
      count++ ;
    } // while
    

    return true ;
  } // CheckIsList()

  void GetArg( Node* head, vector <Node*> *argList ) {
    Node* walk = head ;
    Node* argTemp ;
    while ( walk != NULL && walk -> type != NIL ) {
      // cout << "get arg " << endl ;
      argTemp = Run( walk-> left ) ;
      argList->push_back( argTemp ) ;
      walk = walk -> right ;
    } // while

    // cout << "get arg out " << endl ;
  } // GetArg()

  void GetOneArg( Node* head, vector <Node*> *argList ) {
    Node* walk = head ;
    Node* argTemp ;
    argTemp = Run( walk-> left ) ;
    argList->push_back( argTemp ) ;
  } // GetOneArg()

  void PutInDeineList( string name, Node* argList ) {
    int alreadyHave = false ;
    for ( int i = 0 ; i < mDefineList -> size() ; i++ ) {
      if ( mDefineList -> at( i ).token == name ) {
        mDefineList -> at( i ).define_list = argList ;
        alreadyHave = true ;
      } // if
    } // for

    if ( !alreadyHave ) {
      // cout << "new Define" << endl ;
      Ddefine defineNode ; 
      defineNode.token = name ;
      defineNode.define_list = argList ;
      mDefineList->push_back( defineNode ) ;
    } // if
    
  } // PutInDeineList()

  bool CheckDefine( string name ) {
    for ( int i = 0 ; i < mDefineList->size() ; i++ ) {
      if ( mDefineList -> at( i ).token == name ) {
        return true ;
      } // if
    } // for

    return false ;
  } // CheckDefine()

  Node* GetDefine( string name ) {
    for ( int i = 0 ; i < mDefineList->size() ; i++ ) {
      if ( mDefineList -> at( i ).token == name ) {
        // cout << "get " << mDefineList -> at(i).token << endl ;
        return mDefineList -> at( i ).define_list ;
      } // if
    } // for
 
    // return false ;
    return mDefineList -> at( 0 ).define_list ;
  } // GetDefine()

  void InQuote( Node* head ) {
    // cout << " run " << endl ;
    if ( head == NULL )
      return ;
    InQuote( head -> left ) ;
    head->inQuote = true ;
    InQuote( head -> right ) ;
  } // InQuote()

  Node* QuoteRun( Node* head ) {
    InQuote( head -> left ) ;
    return head -> left ;
  } // QuoteRun()

  Node* ConsRun( Node* head ) {
    Node* walk = head ;
    vector <Node*> *argList = new vector <Node*>() ;
    Node* argTemp = new Node;
    argTemp -> isEmpty = true ;
    GetArg( walk, argList ) ;
    argTemp -> left = argList ->at( 0 ) ;
    argTemp -> right = argList ->at( 1 ) ;
    argList ->clear() ;
    return argTemp ;
  } // ConsRun()

  Node* ListRun( Node* head ) {
    Node* walk = head ;
    vector <Node*> *argList = new vector <Node*>() ;
    Node* argTemp = new Node;
    argTemp -> isEmpty = true ;
    argTemp -> left = NULL ;
    argTemp -> right = NULL ;
    GetArg( walk, argList ) ;
    walk = argTemp ;
    for ( int i = 0 ; i < argList->size() ; i++ ) {
      if ( i != 0 ) {
        argTemp -> right = new Node() ;
        argTemp = argTemp -> right ;
        argTemp -> isEmpty = true ;
        argTemp -> right = NULL ;
      } // if

      argTemp -> left = argList ->at( i ) ;
    } // for

    argList ->clear() ;
    return walk ;
  } // ListRun()


  Node* CarRun( Node* head ) {
    Node* walk = head ;
    vector <Node*> *argList = new vector <Node*>() ;
    Node* argTemp ;
    GetArg( walk, argList ) ;
    argTemp = argList ->at( 0 ) ;
    if ( argTemp -> isEmpty == false ) {
      mErrorProcedure = "car" ;
      mErrorNode = argTemp ;
      throw ARGTYPEERROR ;
    } // if

    argTemp = argTemp -> left ;
    argList ->clear() ;
    return argTemp ;
  } // CarRun()

  Node* CdrRun( Node* head ) {
    Node* walk = head ;
    vector <Node*> *argList = new vector <Node*>() ;
    Node* argTemp ;
    GetArg( walk, argList ) ;
    argTemp = argList ->at( 0 ) ;
    if ( argTemp -> isEmpty == false ) {
      mErrorProcedure = "cdr" ;
      mErrorNode = argTemp ;
      throw ARGTYPEERROR ;
    } // if

    argTemp = argTemp -> right ;
    if ( argTemp == NULL ) {
      argTemp = new Node() ;
      argTemp -> isEmpty = false ;
      argTemp -> inQuote = false ;
      argTemp -> right = NULL ;
      argTemp -> left = NULL ;
      argTemp -> type = NIL ;
      argTemp -> token = "#f" ;      
    } // if

    argList ->clear() ;
    return argTemp ;
  } // CdrRun()

  Node* IsAtomRun( Node* head ) {
    Node* walk = head ;
    vector <Node*> *argList = new vector <Node*>() ;
    Node* argTemp ;
    GetArg( walk, argList ) ;
    // cout << argList ->at(0)-> type << endl ;
    if ( argList ->at( 0 )->isEmpty == false && argList ->at( 0 )-> type != SYMBOL 
         && IsAtom( argList ->at( 0 )-> type ) ) {
      argTemp = new Node() ;
      argTemp -> isEmpty = false ;
      argTemp -> inQuote = false ;
      argTemp -> right = NULL ;
      argTemp -> left = NULL ;
      argTemp -> type = T ;
      argTemp -> token = "#t" ; 
    } // if
    else {
      argTemp = new Node() ;
      argTemp -> isEmpty = false ;
      argTemp -> inQuote = false ;
      argTemp -> right = NULL ;
      argTemp -> left = NULL ;
      argTemp -> type = NIL ;
      argTemp -> token = "#f" ; 
    } // else


    argList ->clear() ;
    return argTemp ;
  } // IsAtomRun()

  Node* IsPairRun( Node* head ) {
    Node* walk = head ;
    vector <Node*> *argList = new vector <Node*>() ;
    Node* argTemp ;
    GetArg( walk, argList ) ;
    if ( argList ->at( 0 )->isEmpty == true ) {
      argTemp = new Node() ;
      argTemp -> isEmpty = false ;
      argTemp -> inQuote = false ;
      argTemp -> right = NULL ;
      argTemp -> left = NULL ;
      argTemp -> type = T ;
      argTemp -> token = "#t" ; 
    } // if
    else {
      argTemp = new Node() ;
      argTemp -> isEmpty = false ;
      argTemp -> inQuote = false ;
      argTemp -> right = NULL ;
      argTemp -> left = NULL ;
      argTemp -> type = NIL ;
      argTemp -> token = "#f" ; 
    } // else


    argList ->clear() ;
    return argTemp ;
  } // IsPairRun()


  Node* IsListRun( Node* head ) {
    Node* walk = head ;
    vector <Node*> *argList = new vector <Node*>() ;
    Node* argTemp ;
    GetArg( walk, argList ) ;
    bool isList = true ;
    for ( walk = argList ->at( 0 ) ; walk != NULL ; walk = walk -> right ) {
      if ( walk -> isEmpty == false ) 
        isList = false ;  
    } // for

    if ( isList ) {
      argTemp = new Node() ;
      argTemp -> isEmpty = false ;
      argTemp -> inQuote = false ;
      argTemp -> right = NULL ;
      argTemp -> left = NULL ;
      argTemp -> type = T ;
      argTemp -> token = "#t" ; 
    } // if
    else {
      argTemp = new Node() ;
      argTemp -> isEmpty = false ;
      argTemp -> inQuote = false ;
      argTemp -> right = NULL ;
      argTemp -> left = NULL ;
      argTemp -> type = NIL ;
      argTemp -> token = "#f" ; 
    } // else


    argList ->clear() ;
    return argTemp ;
  } // IsListRun()


  Node* IsNullRun( Node* head ) {
    Node* walk = head ;
    vector <Node*> *argList = new vector <Node*>() ;
    Node* argTemp ;
    GetArg( walk, argList ) ;
    if ( argList ->at( 0 )->isEmpty == false && argList ->at( 0 )-> type == NIL ) {
      argTemp = new Node() ;
      argTemp -> isEmpty = false ;
      argTemp -> inQuote = false ;
      argTemp -> right = NULL ;
      argTemp -> left = NULL ;
      argTemp -> type = T ;
      argTemp -> token = "#t" ; 
    } // if
    else {
      argTemp = new Node() ;
      argTemp -> isEmpty = false ;
      argTemp -> inQuote = false ;
      argTemp -> right = NULL ;
      argTemp -> left = NULL ;
      argTemp -> type = NIL ;
      argTemp -> token = "#f" ; 
    } // else


    argList ->clear() ;
    return argTemp ;
  } // IsNullRun()

  Node* IsIntRun( Node* head ) {
    Node* walk = head ;
    vector <Node*> *argList = new vector <Node*>() ;
    Node* argTemp ;
    GetArg( walk, argList ) ;
    if ( argList ->at( 0 )->isEmpty == false && argList ->at( 0 )-> type == INT ) {
      argTemp = new Node() ;
      argTemp -> isEmpty = false ;
      argTemp -> inQuote = false ;
      argTemp -> right = NULL ;
      argTemp -> left = NULL ;
      argTemp -> type = T ;
      argTemp -> token = "#t" ; 
    } // if
    else {
      argTemp = new Node() ;
      argTemp -> isEmpty = false ;
      argTemp -> inQuote = false ;
      argTemp -> right = NULL ;
      argTemp -> left = NULL ;
      argTemp -> type = NIL ;
      argTemp -> token = "#f" ; 
    } // else


    argList ->clear() ;
    return argTemp ;
  } // IsIntRun()

  Node* IsRealRun( Node* head ) {
    Node* walk = head ;
    vector <Node*> *argList = new vector <Node*>() ;
    Node* argTemp ;
    GetArg( walk, argList ) ;
    if ( argList ->at( 0 )->isEmpty == false 
         && ( argList ->at( 0 )-> type == INT || argList ->at( 0 )-> type == FLOAT ) ) {
      argTemp = new Node() ;
      argTemp -> isEmpty = false ;
      argTemp -> inQuote = false ;
      argTemp -> right = NULL ;
      argTemp -> left = NULL ;
      argTemp -> type = T ;
      argTemp -> token = "#t" ; 
    } // if
    else {
      argTemp = new Node() ;
      argTemp -> isEmpty = false ;
      argTemp -> inQuote = false ;
      argTemp -> right = NULL ;
      argTemp -> left = NULL ;
      argTemp -> type = NIL ;
      argTemp -> token = "#f" ; 
    } // else


    argList ->clear() ;
    return argTemp ;
  } // IsRealRun()

  Node* IsStringRun( Node* head ) {
    Node* walk = head ;
    vector <Node*> *argList = new vector <Node*>() ;
    Node* argTemp ;
    GetArg( walk, argList ) ;
    if ( argList ->at( 0 )->isEmpty == false && argList ->at( 0 )-> type == STRING ) {
      argTemp = new Node() ;
      argTemp -> isEmpty = false ;
      argTemp -> inQuote = false ;
      argTemp -> right = NULL ;
      argTemp -> left = NULL ;
      argTemp -> type = T ;
      argTemp -> token = "#t" ; 
    } // if
    else {
      argTemp = new Node() ;
      argTemp -> isEmpty = false ;
      argTemp -> inQuote = false ;
      argTemp -> right = NULL ;
      argTemp -> left = NULL ;
      argTemp -> type = NIL ;
      argTemp -> token = "#f" ; 
    } // else


    argList ->clear() ;
    return argTemp ;
  } // IsStringRun()

  Node* IsBoolRun( Node* head ) {
    Node* walk = head ;
    vector <Node*> *argList = new vector <Node*>() ;
    Node* argTemp ;
    GetArg( walk, argList ) ;
    if ( argList ->at( 0 )->isEmpty == false 
         && ( argList ->at( 0 )-> type == T || argList ->at( 0 )-> type == NIL ) ) {
      argTemp = new Node() ;
      argTemp -> isEmpty = false ;
      argTemp -> inQuote = false ;
      argTemp -> right = NULL ;
      argTemp -> left = NULL ;
      argTemp -> type = T ;
      argTemp -> token = "#t" ; 
    } // if
    else {
      argTemp = new Node() ;
      argTemp -> isEmpty = false ;
      argTemp -> inQuote = false ;
      argTemp -> right = NULL ;
      argTemp -> left = NULL ;
      argTemp -> type = NIL ;
      argTemp -> token = "#f" ; 
    } // else


    argList ->clear() ;
    return argTemp ;
  } // IsBoolRun()

  Node* IsSymbolRun( Node* head ) {
    Node* walk = head ;
    vector <Node*> *argList = new vector <Node*>() ;
    Node* argTemp ;
    GetArg( walk, argList ) ;
    if ( argList ->at( 0 )->isEmpty == false && argList ->at( 0 )-> type == SYMBOL ) {
      argTemp = new Node() ;
      argTemp -> isEmpty = false ;
      argTemp -> inQuote = false ;
      argTemp -> right = NULL ;
      argTemp -> left = NULL ;
      argTemp -> type = T ;
      argTemp -> token = "#t" ; 
    } // if
    else {
      argTemp = new Node() ;
      argTemp -> isEmpty = false ;
      argTemp -> inQuote = false ;
      argTemp -> right = NULL ;
      argTemp -> left = NULL ;
      argTemp -> type = NIL ;
      argTemp -> token = "#f" ; 
    } // else


    argList ->clear() ;
    return argTemp ;
  } // IsSymbolRun()

  Node* AndRun( Node* head ) {
    Node* walk = head ;
    vector <Node*> *argList = new vector <Node*>() ;
    Node* argTemp ;
    while ( walk != NULL ) {
      argList ->clear() ;
      GetOneArg( walk, argList ) ;
      if ( argList ->at( 0 )->isEmpty == false && argList ->at( 0 )-> type == NIL ) {
        argTemp = new Node() ;
        argTemp -> isEmpty = false ;
        argTemp -> inQuote = false ;
        argTemp -> right = NULL ;
        argTemp -> left = NULL ;
        argTemp -> type = NIL ;
        argTemp -> token = "#f" ; 
        return argTemp ;
      } // if

      walk = walk -> right ;
    } // while


    argTemp = argList ->at( 0 ) ;

    return argTemp ;
  } // AndRun()

  Node* OrRun( Node* head ) {
    Node* walk = head ;
    vector <Node*> *argList = new vector <Node*>() ;
    Node* argTemp ;
    while ( walk != NULL ) {
      argList ->clear() ;
      GetOneArg( walk, argList ) ;
      if ( argList ->at( 0 )->isEmpty == true || argList ->at( 0 )-> type != NIL ) {
        argTemp = argList ->at( 0 ) ;
        return argTemp ;
      } // if

      walk = walk -> right ;
    } // while

    argTemp = new Node() ;
    argTemp -> isEmpty = false ;
    argTemp -> inQuote = false ;
    argTemp -> right = NULL ;
    argTemp -> left = NULL ;
    argTemp -> type = NIL ;
    argTemp -> token = "#f" ; 

    return argTemp ;
  } // OrRun()

  Node* PlusRun( Node* head ) {
    Node* walk = head ;
    vector <Node*> *argList = new vector <Node*>() ;
    Node* argTemp ;
    float sum = 0 ;
    bool haveFloat = false ;
    while ( walk != NULL ) {
      argList ->clear() ;
      GetOneArg( walk, argList ) ;
      if ( argList->at( 0 )-> isEmpty == true 
           || ( argList->at( 0 )-> type != INT && argList->at( 0 )-> type != FLOAT ) ) {
        mErrorNode = argList ->at( 0 ) ;
        mErrorProcedure = "+" ;
        throw ARGTYPEERROR ;
      } // if

      sum = sum + atof( argList->at( 0 )->token.c_str() ) ;
      if ( argList->at( 0 )->type == FLOAT )
        haveFloat = true ;
      walk = walk -> right ;
    } // while

    stringstream ss;
    ss << fixed << setprecision( 5 ) << sum;
    argTemp = new Node() ;
    argTemp -> isEmpty = false ;
    argTemp -> inQuote = false ;
    argTemp -> right = NULL ;
    argTemp -> left = NULL ;
    argTemp -> token = ss.str() ;
    if ( haveFloat ) {
      argTemp -> type = FLOAT ;
      Data calTemp ;
      calTemp.token = ss.str() ;
      argTemp -> token = FloatRule( calTemp ) ;
    } // if 
    else {
      argTemp -> type = INT ;
      Data calTemp ;
      calTemp.token = ss.str() ;
      argTemp -> token = IntRule( calTemp ) ;
    } // else
      
    // cout << argTemp ->token << endl ; 

    return argTemp ;
  } // PlusRun()

  Node* MinusRun( Node* head ) {
    Node* walk = head ;
    vector <Node*> *argList = new vector <Node*>() ;
    Node* argTemp ;
    float sum = 0 ;
    bool haveFloat = false ;
    bool theFirst = true ;
    while ( walk != NULL ) {
      argList ->clear() ;
      GetOneArg( walk, argList ) ;
      if ( argList->at( 0 )-> isEmpty == true 
           || ( argList->at( 0 )-> type != INT && argList->at( 0 )-> type != FLOAT ) ) {
        mErrorNode = argList ->at( 0 ) ;
        mErrorProcedure = "-" ;
        throw ARGTYPEERROR ;
      } // if
      
      if ( theFirst ) {
        sum = sum + atof( argList->at( 0 )->token.c_str() ) ;
        theFirst = false ;
      } // if
      else
        sum = sum - atof( argList->at( 0 )->token.c_str() ) ;

      if ( argList->at( 0 )->type == FLOAT ) 
        haveFloat = true ;

      if ( haveFloat ) {
        stringstream ss;
        ss << fixed << setprecision( 5 ) << sum;
        Data calTemp ;
        calTemp.token = ss.str() ;
        string floatTemp = FloatRule( calTemp ) ;
        // cout << floatTemp << endl ; 
        sum = atof( floatTemp.c_str() ) ;
      } // if


      walk = walk -> right ;
    } // while

    stringstream ss;
    ss << fixed << setprecision( 5 ) << sum;
    argTemp = new Node() ;
    argTemp -> isEmpty = false ;
    argTemp -> inQuote = false ;
    argTemp -> right = NULL ;
    argTemp -> left = NULL ;
    argTemp -> token = ss.str() ;
    if ( haveFloat ) {
      argTemp -> type = FLOAT ;
      Data calTemp ;
      calTemp.token = ss.str() ;
      argTemp -> token = FloatRule( calTemp ) ;
    } // if 
    else {
      argTemp -> type = INT ;
      Data calTemp ;
      calTemp.token = ss.str() ;
      argTemp -> token = IntRule( calTemp ) ;
    } // else
      
    // cout << argTemp ->token << endl ; 

    return argTemp ;
  } // MinusRun()

  Node* MulRun( Node* head ) {
    Node* walk = head ;
    vector <Node*> *argList = new vector <Node*>() ;
    Node* argTemp ;
    float sum = 0 ;
    bool haveFloat = false ;
    bool theFirst = true ;
    while ( walk != NULL ) {
      argList ->clear() ;
      GetOneArg( walk, argList ) ;
      if ( argList->at( 0 )-> isEmpty == true 
           || ( argList->at( 0 )-> type != INT && argList->at( 0 )-> type != FLOAT ) ) {
        mErrorNode = argList ->at( 0 ) ;
        mErrorProcedure = "*" ;
        throw ARGTYPEERROR ;
      } // if
      
      if ( theFirst ) {
        sum = sum + atof( argList->at( 0 )->token.c_str() ) ;
        theFirst = false ;
      } // if
      else
        sum = sum * atof( argList->at( 0 )->token.c_str() ) ;

      if ( argList->at( 0 )->type == FLOAT ) 
        haveFloat = true ;

      if ( haveFloat ) {
        stringstream ss;
        ss << fixed << setprecision( 5 ) << sum;
        Data calTemp ;
        calTemp.token = ss.str() ;
        string floatTemp = FloatRule( calTemp ) ;
        // cout << floatTemp << endl ; 
        sum = atof( floatTemp.c_str() ) ;
      } // if
      else {
        stringstream ss;
        ss << fixed << setprecision( 5 ) << sum;
        Data calTemp ;
        calTemp.token = ss.str() ;
        string floatTemp = IntRule( calTemp ) ;
        sum = atof( floatTemp.c_str() ) ;
      } // else


      walk = walk -> right ;
    } // while

    stringstream ss;
    ss << fixed << setprecision( 5 ) << sum;
    argTemp = new Node() ;
    argTemp -> isEmpty = false ;
    argTemp -> inQuote = false ;
    argTemp -> right = NULL ;
    argTemp -> left = NULL ;
    argTemp -> token = ss.str() ;
    if ( haveFloat ) {
      argTemp -> type = FLOAT ;
      Data calTemp ;
      calTemp.token = ss.str() ;
      argTemp -> token = FloatRule( calTemp ) ;
    } // if 
    else {
      argTemp -> type = INT ;
      Data calTemp ;
      calTemp.token = ss.str() ;
      argTemp -> token = IntRule( calTemp ) ;
    } // else
      
    // cout << argTemp ->token << endl ; 

    return argTemp ;
  } // MulRun()

  Node* DivRun( Node* head ) {
    Node* walk = head ;
    vector <Node*> *argList = new vector <Node*>() ;
    Node* argTemp ;
    float sum = 0 ;
    bool haveFloat = false ;
    bool theFirst = true ;
    while ( walk != NULL ) {
      argList ->clear() ;
      GetOneArg( walk, argList ) ;
      if ( argList->at( 0 )-> isEmpty == true 
           || ( argList->at( 0 )-> type != INT && argList->at( 0 )-> type != FLOAT ) ) {
        mErrorNode = argList ->at( 0 ) ;
        mErrorProcedure = "/" ;
        throw ARGTYPEERROR ;
      } // if
      
      if ( theFirst ) {
        sum = sum + atof( argList->at( 0 )->token.c_str() ) ;
        theFirst = false ;
      } // if
      else {
        if ( atof( argList->at( 0 )->token.c_str() ) == 0 )
          throw DIVBYZERO ;
        sum = sum / atof( argList->at( 0 )->token.c_str() ) ;
      } // else
        

      if ( argList->at( 0 )->type == FLOAT ) 
        haveFloat = true ;

      if ( haveFloat ) {
        stringstream ss;
        ss << fixed << setprecision( 5 ) << sum;
        Data calTemp ;
        calTemp.token = ss.str() ;
        string floatTemp = FloatRule( calTemp ) ;
        // cout << floatTemp << endl ; 
        sum = atof( floatTemp.c_str() ) ;
      } // if
      else {
        stringstream ss;
        ss << fixed << setprecision( 5 ) << sum;
        Data calTemp ;
        calTemp.token = ss.str() ;
        string floatTemp = IntRule( calTemp ) ;
        sum = atof( floatTemp.c_str() ) ;
      } // else


      walk = walk -> right ;
    } // while

    stringstream ss;
    ss << fixed << setprecision( 5 ) << sum;
    argTemp = new Node() ;
    argTemp -> isEmpty = false ;
    argTemp -> inQuote = false ;
    argTemp -> right = NULL ;
    argTemp -> left = NULL ;
    argTemp -> token = ss.str() ;
    if ( haveFloat ) {
      argTemp -> type = FLOAT ;
      Data calTemp ;
      calTemp.token = ss.str() ;
      argTemp -> token = FloatRule( calTemp ) ;
    } // if 
    else {
      argTemp -> type = INT ;
      Data calTemp ;
      calTemp.token = ss.str() ;
      argTemp -> token = IntRule( calTemp ) ;
    } // else
      
    // cout << argTemp ->token << endl ; 

    return argTemp ;
  } // DivRun()

  Node* GreatRun( Node* head ) {
    Node* walk = head ;
    vector <Node*> *argList = new vector <Node*>() ;
    Node* argTemp ;
    GetArg( walk, argList ) ;
    // check arg type
    for ( int i = 0 ; i < argList ->size() ; i++ ) {
      if ( argList ->at( i )->type != INT && argList ->at( i )->type != FLOAT ) {
        mErrorProcedure = ">" ;
        mErrorNode = argList ->at( i ) ;
        throw ARGTYPEERROR ;
      } // if 
    } // for

    float temp1 ;
    float temp2 ;
    for ( int i = 1 ; i < argList ->size() ; i++ ) {
      temp1 = atof( argList->at( i - 1 )->token.c_str() ) ;
      temp2 = atof( argList->at( i )->token.c_str() ) ;
      if ( temp1 <= temp2 ) {
        argTemp = new Node() ;
        argTemp -> isEmpty = false ;
        argTemp -> inQuote = false ;
        argTemp -> right = NULL ;
        argTemp -> left = NULL ;
        argTemp -> type = NIL ;
        argTemp -> token = "#f" ;  
        return argTemp ;
      } // if
    } // for

    argTemp = new Node() ;
    argTemp -> isEmpty = false ;
    argTemp -> inQuote = false ;
    argTemp -> right = NULL ;
    argTemp -> left = NULL ;
    argTemp -> type = T ;
    argTemp -> token = "#t" ; 
    return argTemp ;
  } // GreatRun()

  Node* GreatOrEquRun( Node* head ) {
    Node* walk = head ;
    vector <Node*> *argList = new vector <Node*>() ;
    Node* argTemp ;
    GetArg( walk, argList ) ;
    // check arg type
    for ( int i = 0 ; i < argList ->size() ; i++ ) {
      if ( argList ->at( i )->type != INT && argList ->at( i )->type != FLOAT ) {
        mErrorProcedure = ">=" ;
        mErrorNode = argList ->at( i ) ;
        throw ARGTYPEERROR ;
      } // if 
    } // for

    float temp1 ;
    float temp2 ;
    for ( int i = 1 ; i < argList ->size() ; i++ ) {
      temp1 = atof( argList->at( i - 1 )->token.c_str() ) ;
      temp2 = atof( argList->at( i )->token.c_str() ) ;
      if ( temp1 < temp2 ) {
        argTemp = new Node() ;
        argTemp -> isEmpty = false ;
        argTemp -> inQuote = false ;
        argTemp -> right = NULL ;
        argTemp -> left = NULL ;
        argTemp -> type = NIL ;
        argTemp -> token = "#f" ;  
        return argTemp ;
      } // if
    } // for

    argTemp = new Node() ;
    argTemp -> isEmpty = false ;
    argTemp -> inQuote = false ;
    argTemp -> right = NULL ;
    argTemp -> left = NULL ;
    argTemp -> type = T ;
    argTemp -> token = "#t" ; 
    return argTemp ;
  } // GreatOrEquRun()

  Node* LessRun( Node* head ) {
    Node* walk = head ;
    vector <Node*> *argList = new vector <Node*>() ;
    Node* argTemp ;
    GetArg( walk, argList ) ;
    // check arg type
    for ( int i = 0 ; i < argList ->size() ; i++ ) {
      if ( argList ->at( i )->type != INT && argList ->at( i )->type != FLOAT ) {
        mErrorProcedure = "<" ;
        mErrorNode = argList ->at( i ) ;
        throw ARGTYPEERROR ;
      } // if 
    } // for

    float temp1 ;
    float temp2 ;
    for ( int i = 1 ; i < argList ->size() ; i++ ) {
      temp1 = atof( argList->at( i - 1 )->token.c_str() ) ;
      temp2 = atof( argList->at( i )->token.c_str() ) ;
      if ( temp1 >= temp2 ) {
        argTemp = new Node() ;
        argTemp -> isEmpty = false ;
        argTemp -> inQuote = false ;
        argTemp -> right = NULL ;
        argTemp -> left = NULL ;
        argTemp -> type = NIL ;
        argTemp -> token = "#f" ;  
        return argTemp ;
      } // if
    } // for

    argTemp = new Node() ;
    argTemp -> isEmpty = false ;
    argTemp -> inQuote = false ;
    argTemp -> right = NULL ;
    argTemp -> left = NULL ;
    argTemp -> type = T ;
    argTemp -> token = "#t" ; 
    return argTemp ;
  } // LessRun()

  Node* LessOrEquRun( Node* head ) {
    Node* walk = head ;
    vector <Node*> *argList = new vector <Node*>() ;
    Node* argTemp ;
    GetArg( walk, argList ) ;
    // check arg type
    for ( int i = 0 ; i < argList ->size() ; i++ ) {
      if ( argList ->at( i )->type != INT && argList ->at( i )->type != FLOAT ) {
        mErrorProcedure = "<=" ;
        mErrorNode = argList ->at( i ) ;
        throw ARGTYPEERROR ;
      } // if 
    } // for

    float temp1 ;
    float temp2 ;
    for ( int i = 1 ; i < argList ->size() ; i++ ) {
      temp1 = atof( argList->at( i - 1 )->token.c_str() ) ;
      temp2 = atof( argList->at( i )->token.c_str() ) ;
      if ( temp1 > temp2 ) {
        argTemp = new Node() ;
        argTemp -> isEmpty = false ;
        argTemp -> inQuote = false ;
        argTemp -> right = NULL ;
        argTemp -> left = NULL ;
        argTemp -> type = NIL ;
        argTemp -> token = "#f" ;  
        return argTemp ;
      } // if
    } // for

    argTemp = new Node() ;
    argTemp -> isEmpty = false ;
    argTemp -> inQuote = false ;
    argTemp -> right = NULL ;
    argTemp -> left = NULL ;
    argTemp -> type = T ;
    argTemp -> token = "#t" ; 
    return argTemp ;
  } // LessOrEquRun()



  Node* IntEquRun( Node* head ) {
    Node* walk = head ;
    vector <Node*> *argList = new vector <Node*>() ;
    Node* argTemp ;
    GetArg( walk, argList ) ;
    // check arg type
    for ( int i = 0 ; i < argList ->size() ; i++ ) {
      if ( argList ->at( i )->type != INT && argList ->at( i )->type != FLOAT ) {
        mErrorProcedure = "=" ;
        mErrorNode = argList ->at( i ) ;
        throw ARGTYPEERROR ;
      } // if 
    } // for

    float temp1 ;
    float temp2 ;
    for ( int i = 1 ; i < argList ->size() ; i++ ) {
      temp1 = atof( argList->at( i - 1 )->token.c_str() ) ;
      temp2 = atof( argList->at( i )->token.c_str() ) ;
      if ( temp1 != temp2 ) {
        argTemp = new Node() ;
        argTemp -> isEmpty = false ;
        argTemp -> inQuote = false ;
        argTemp -> right = NULL ;
        argTemp -> left = NULL ;
        argTemp -> type = NIL ;
        argTemp -> token = "#f" ;  
        return argTemp ;
      } // if
    } // for

    argTemp = new Node() ;
    argTemp -> isEmpty = false ;
    argTemp -> inQuote = false ;
    argTemp -> right = NULL ;
    argTemp -> left = NULL ;
    argTemp -> type = T ;
    argTemp -> token = "#t" ; 
    return argTemp ;
  } // IntEquRun()

  Node* StringAppendRun( Node* head ) {
    Node* walk = head ;
    vector <Node*> *argList = new vector <Node*>() ;
    Node* argTemp ;
    GetArg( walk, argList ) ;
    // check arg type
    for ( int i = 0 ; i < argList ->size() ; i++ ) {
      if ( argList ->at( i )->type != STRING ) {
        mErrorProcedure = "string-append" ;
        mErrorNode = argList ->at( i ) ;
        throw ARGTYPEERROR ;
      } // if 
    } // for

    argTemp = new Node() ;
    argTemp -> isEmpty = false ;
    argTemp -> inQuote = false ;
    argTemp -> right = NULL ;
    argTemp -> left = NULL ;
    argTemp -> type = STRING ;
    argTemp -> token = "" ; 

    for ( int i = 0 ; i < argList ->size() ; i++ ) {
      argTemp -> token = argTemp -> token + argList->at( i )->token ;
    } // for


    return argTemp ;
  } // StringAppendRun()

  Node* StringGreat( Node* head ) {
    Node* walk = head ;
    vector <Node*> *argList = new vector <Node*>() ;
    Node* argTemp ;
    GetArg( walk, argList ) ;
    // check arg type
    for ( int i = 0 ; i < argList ->size() ; i++ ) {
      if ( argList ->at( i )->type != STRING ) {
        mErrorProcedure = "string>?" ;
        mErrorNode = argList ->at( i ) ;
        throw ARGTYPEERROR ;
      } // if 
    } // for
    
    string temp1 ;
    string temp2 ;
    for ( int i = 1 ; i < argList ->size() ; i++ ) {
      temp1 = argList ->at( i - 1 )->token.c_str() ;
      temp2 = argList ->at( i )->token.c_str() ;
      if ( temp1 <= temp2 ) {
        argTemp = new Node() ;
        argTemp -> isEmpty = false ;
        argTemp -> inQuote = false ;
        argTemp -> right = NULL ;
        argTemp -> left = NULL ;
        argTemp -> type = NIL ;
        argTemp -> token = "#f" ;  
        return argTemp ;
      } // if
    } // for

    argTemp = new Node() ;
    argTemp -> isEmpty = false ;
    argTemp -> inQuote = false ;
    argTemp -> right = NULL ;
    argTemp -> left = NULL ;
    argTemp -> type = T ;
    argTemp -> token = "#t" ; 
    return argTemp ;
  } // StringGreat()

  Node* StringLess( Node* head ) {
    Node* walk = head ;
    vector <Node*> *argList = new vector <Node*>() ;
    Node* argTemp ;
    GetArg( walk, argList ) ;
    // check arg type
    for ( int i = 0 ; i < argList ->size() ; i++ ) {
      if ( argList ->at( i )->type != STRING ) {
        mErrorProcedure = "string<?" ;
        mErrorNode = argList ->at( i ) ;
        throw ARGTYPEERROR ;
      } // if 
    } // for
    
    string temp1 ;
    string temp2 ;
    for ( int i = 1 ; i < argList ->size() ; i++ ) {
      temp1 = argList ->at( i - 1 )->token.c_str() ;
      temp2 = argList ->at( i )->token.c_str() ;
      if ( temp1 >= temp2 ) {
        argTemp = new Node() ;
        argTemp -> isEmpty = false ;
        argTemp -> inQuote = false ;
        argTemp -> right = NULL ;
        argTemp -> left = NULL ;
        argTemp -> type = NIL ;
        argTemp -> token = "#f" ;  
        return argTemp ;
      } // if
    } // for

    argTemp = new Node() ;
    argTemp -> isEmpty = false ;
    argTemp -> inQuote = false ;
    argTemp -> right = NULL ;
    argTemp -> left = NULL ;
    argTemp -> type = T ;
    argTemp -> token = "#t" ; 
    return argTemp ;
  } // StringLess()

  Node* StringEqu( Node* head ) {
    Node* walk = head ;
    vector <Node*> *argList = new vector <Node*>() ;
    Node* argTemp ;
    GetArg( walk, argList ) ;
    // check arg type
    for ( int i = 0 ; i < argList ->size() ; i++ ) {
      if ( argList ->at( i )->type != STRING ) {
        mErrorProcedure = "string=?" ;
        mErrorNode = argList ->at( i ) ;
        throw ARGTYPEERROR ;
      } // if 
    } // for
    
    string temp1 ;
    string temp2 ;
    for ( int i = 1 ; i < argList ->size() ; i++ ) {
      temp1 = argList ->at( i - 1 )->token.c_str() ;
      temp2 = argList ->at( i )->token.c_str() ;
      if ( temp1 != temp2 ) {
        argTemp = new Node() ;
        argTemp -> isEmpty = false ;
        argTemp -> inQuote = false ;
        argTemp -> right = NULL ;
        argTemp -> left = NULL ;
        argTemp -> type = NIL ;
        argTemp -> token = "#f" ;  
        return argTemp ;
      } // if
    } // for

    argTemp = new Node() ;
    argTemp -> isEmpty = false ;
    argTemp -> inQuote = false ;
    argTemp -> right = NULL ;
    argTemp -> left = NULL ;
    argTemp -> type = T ;
    argTemp -> token = "#t" ; 
    return argTemp ;
  } // StringEqu()

  Node* Eqv( Node* head ) {
    Node* walk = head ;
    vector <Node*> *argList = new vector <Node*>() ;
    Node* argTemp ;
    bool isEqu = false ;
    GetArg( walk, argList ) ;
    if ( argList ->at( 0 ) == argList ->at( 1 ) )
      isEqu = true ;
    else if ( ( argList ->at( 0 )->type == INT || argList ->at( 0 )->type == FLOAT ) &&
              ( argList ->at( 1 )->type == INT || argList ->at( 1 )->type == FLOAT ) ) {
      float temp1 ;
      float temp2 ;
      temp1 = atof( argList->at( 0 )->token.c_str() ) ;
      temp2 = atof( argList->at( 1 )->token.c_str() ) ;
      if ( temp1 == temp2 )
        isEqu = true ;
    } // else if
    else if ( argList ->at( 0 )->type == T && argList ->at( 1 )->type == T ) {
      isEqu = true ;
    } // else if
    else if ( argList ->at( 0 )->type == NIL && argList ->at( 1 )->type == NIL ) {
      isEqu = true ;
    } // else if
  
    if ( isEqu ) {
      argTemp = new Node() ;
      argTemp -> isEmpty = false ;
      argTemp -> inQuote = false ;
      argTemp -> right = NULL ;
      argTemp -> left = NULL ;
      argTemp -> type = T ;
      argTemp -> token = "#t" ; 
    } // if
    else {
      argTemp = new Node() ;
      argTemp -> isEmpty = false ;
      argTemp -> inQuote = false ;
      argTemp -> right = NULL ;
      argTemp -> left = NULL ;
      argTemp -> type = NIL ;
      argTemp -> token = "nil" ; 
    } // else

    return argTemp ;
  } // Eqv()

  Node* Equal( Node* head ) {
    Node* walk = head ;
    vector <Node*> *argList = new vector <Node*>() ;
    Node* argTemp ;
    bool isEqu = false ;
    GetArg( walk, argList ) ;
    mp1.PrintExp( argList ->at( 0 ) ) ;
    string temp1 = mp1.GetList() ;
    mp1.PrintExp( argList ->at( 1 ) ) ;
    string temp2 = mp1.GetList() ;
    if ( temp1 == temp2 ) {
      argTemp = new Node() ;
      argTemp -> isEmpty = false ;
      argTemp -> inQuote = false ;
      argTemp -> right = NULL ;
      argTemp -> left = NULL ;
      argTemp -> type = T ;
      argTemp -> token = "#t" ; 
    } // if
    else {
      argTemp = new Node() ;
      argTemp -> isEmpty = false ;
      argTemp -> inQuote = false ;
      argTemp -> right = NULL ;
      argTemp -> left = NULL ;
      argTemp -> type = NIL ;
      argTemp -> token = "nil" ; 
    } // else

    return argTemp ;
  } // Equal()

  Node* BeginRun( Node* head ) {
    Node* walk = head ;
    vector <Node*> *argList = new vector <Node*>() ;
    Node* argTemp ;
    GetArg( walk, argList ) ;
    argTemp = argList ->at( argList ->size() -1 ) ;

    return argTemp ;
  } // BeginRun()

  Node* IfRun( Node* head ) {
    Node* walk = head ;
    vector <Node*> *argList = new vector <Node*>() ;
    Node* argTemp ;
    walk = walk -> right ;
    GetOneArg( walk, argList ) ;
    if ( argList ->at( 0 )->isEmpty == true || argList ->at( 0 )->type != NIL ) {
      GetOneArg( walk -> right, argList ) ;
    } // if
    else {
      walk = walk -> right ;
      walk = walk -> right ;
      if ( walk == NULL ) {
        mErrorNode = head ;
        throw NORETURNVALUE ;
      } // if
      else {
        GetOneArg( walk, argList ) ;
      } // else
    } // else

    argTemp = argList ->at( 1 ) ;
    return argTemp ;
  } // IfRun()

  Node* CondRun( Node* head ) {
    Node* walk = head ;
    vector <Node*> *argList = new vector <Node*>() ;
    Node* argTemp ;
    walk = walk -> right ;
    Node* walk2 ;
    // check format
    for ( int i = 0 ; walk != NULL ; walk = walk -> right ) {
      walk2 = walk -> left ;
      if ( walk2 -> isEmpty == false ) {
        mErrorProcedure = "COND" ;
        mErrorNode = head ;
        throw FORMATERROR ;
      } // if
      else if ( walk2 -> right == NULL ) {
        mErrorProcedure = "COND" ;
        mErrorNode = head ;
        throw FORMATERROR ;
      } // else if
    } // for

    // find the return valus
    // need check all exp
    bool haveResult = false ;
    walk = head -> right ;
    for ( int i = 0 ; walk != NULL ; walk = walk -> right ) {
      walk2 = walk -> left ;
      if ( walk -> right == NULL && walk -> left -> left -> token == "else" ) {
        haveResult = true ;
        GetArg( walk2 -> right, argList ) ;
        argTemp = argList ->at( argList ->size() - 1 ) ;
        return argTemp ;
      } // if
      else {
        GetOneArg( walk2, argList ) ;
        if ( argList ->at( 0 )->isEmpty == false && argList ->at( 0 )->type == NIL ) ;
        else {
          GetArg( walk2 -> right, argList ) ;
          argTemp = argList ->at( argList ->size() - 1 ) ;
          return argTemp ;
        } // else
      } // else
      
      argList ->clear() ;

    } // for

    if ( haveResult ) {
      return argTemp ;
    } // if
    else {
      mErrorNode = head ;
      throw NORETURNVALUE ;
    } // else


    return argTemp ;
  } // CondRun()

  Node* Run( Node* head ) {
    Node* temp ;
    Node* walk = head ;
    if ( walk -> type == SYMBOL ) {
      // if is symbol check define or function  
      if ( mp1.IsProcedure( walk -> token ) ) {
        temp = walk ;
        return temp ;
      } // if
      else if ( CheckDefine( walk -> token )  ) {
        // define
        temp = GetDefine( walk -> token ) ;
        return temp ;
      } // else if
      else {
        // unbound
        mErrorProcedure = walk -> token ;
        throw UNBOUNDERROR ;
      } // else
    } // if
    else if ( IsAtom( walk -> type ) ) {
      temp = walk ;
      return temp ;
    } // else if
    else {
      int argCount = 0 ;
      vector <Node*> *argList = new vector <Node*>() ;
      Node* argTemp ;
      if ( CheckIsList( walk, argCount ) == false ) {
        mErrorNode = head ;
        throw NONLIST ;
      } // if

      if ( walk -> left -> isEmpty == true ) {
        walk -> left = Run( walk -> left ) ;
      } // if
      else if ( !mp1.IsProcedure( walk -> left -> token ) && walk -> left -> type == SYMBOL ) {
        walk -> left = Run( walk -> left ) ;
      } // else if

      if ( mp1.IsProcedure( walk -> left -> token ) && walk -> left -> inQuote == false ) {
        if ( walk -> left -> token == "define" ) {
          mNotPrint = true ;
          if ( walk -> left != mTreeHead -> left ) {
            // level error
            mErrorProcedure = "DEFINE" ;
            throw LEVELERROR ;
          } // if

          walk = walk -> right ;
          if ( walk == NULL ) {
            // format error
            mErrorProcedure = "DEFINE" ;
            mErrorNode = head ;
            throw FORMATERROR ;
          } // if

          string defineName ;
          if ( mp1.IsProcedure( walk -> left -> token ) ||
               walk -> left -> type != SYMBOL || argCount != 2 ) {
            mErrorProcedure = "DEFINE" ;
            mErrorNode = head ;
            throw FORMATERROR ;
          } // if

          defineName = walk -> left -> token ;
          GetArg( walk -> right, argList ) ;
          PutInDeineList( defineName, argList ->at( 0 ) ) ;
          cout << defineName << " defined" << endl ;
          argList ->clear() ;
          return temp ;
        } // if ( function define )
        else if ( walk -> left -> token == "exit" ) {
          mNotPrint = true ;
          if ( walk -> left != mTreeHead -> left ) {
            // level error
            mErrorProcedure = "EXIT" ;
            throw LEVELERROR ;
          } // if

          if ( argCount != 0 ) {
            mErrorProcedure = "exit" ;
            throw ARGNUMERROR ;
          } // if

          mIsExit = true ;
          return temp ;
        } // else if
        else if ( walk -> left -> token == "clean-environment" ) {
          mNotPrint = true ;
          if ( walk -> left != mTreeHead -> left ) {
            // level error
            mErrorProcedure = "CLEAN-ENVIRONMENT" ;
            throw LEVELERROR ;
          } // if

          if ( argCount != 0 ) {
            mErrorProcedure = "clean-environment" ;
            throw ARGNUMERROR ;
          } // if

          mDefineList ->clear() ;
          cout << "environment cleaned" << endl ;
          return temp ;
        } // else if
        else if ( walk -> left -> token == "quote" ) {
          if ( argCount != 1 ) {
            // arg num error
            mErrorProcedure = "quote" ;
            throw ARGNUMERROR ;
          } // if

          return QuoteRun( walk -> right ) ;
        } // else if
        else if ( walk -> left -> token == "cons" ) {
          if ( argCount != 2 ) {
            mErrorProcedure = "cons" ;
            throw ARGNUMERROR ;
          } // if

          return ConsRun( walk -> right ) ;
        } // else if
        else if ( walk -> left -> token == "list" ) {
          if ( argCount == 0 ) {
            argTemp = new Node() ;
            argTemp -> isEmpty = false ;
            argTemp -> inQuote = false ;
            argTemp -> right = NULL ;
            argTemp -> left = NULL ;
            argTemp -> type = NIL ;
            argTemp -> token = "#f" ;
            return argTemp ;
          } // if

          return ListRun( walk -> right ) ;
        } // else if
        else if ( walk -> left -> token == "car" ) {
          if ( argCount != 1 ) {
            mErrorProcedure = "car" ;
            throw ARGNUMERROR ;
          } // if

          return CarRun( walk -> right ) ;
        } // else if
        else if ( walk -> left -> token == "cdr" ) {
          if ( argCount != 1 ) {
            mErrorProcedure = "cdr" ;
            throw ARGNUMERROR ;
          } // if

          return CdrRun( walk -> right ) ;
        } // else if
        else if ( walk -> left -> token == "atom?" ) {
          if ( argCount != 1 ) {
            mErrorProcedure = "atom?" ;
            throw ARGNUMERROR ;
          } // if

          return IsAtomRun( walk -> right ) ;
        } // else if
        else if ( walk -> left -> token == "pair?" ) {
          if ( argCount != 1 ) {
            mErrorProcedure = "pair?" ;
            throw ARGNUMERROR ;
          } // if

          return IsPairRun( walk -> right ) ;
        } // else if
        else if ( walk -> left -> token == "list?" ) {
          if ( argCount != 1 ) {
            mErrorProcedure = "list?" ;
            throw ARGNUMERROR ;
          } // if

          return IsListRun( walk -> right ) ;
        } // else if
        else if ( walk -> left -> token == "null?" ) {
          if ( argCount != 1 ) {
            mErrorProcedure = "null?" ;
            throw ARGNUMERROR ;
          } // if

          return IsNullRun( walk -> right ) ;
        } // else if
        else if ( walk -> left -> token == "integer?" ) {
          if ( argCount != 1 ) {
            mErrorProcedure = "integer?" ;
            throw ARGNUMERROR ;
          } // if

          return IsIntRun( walk -> right ) ;
        } // else if
        else if ( walk -> left -> token == "real?" ) {
          if ( argCount != 1 ) {
            mErrorProcedure = "real?" ;
            throw ARGNUMERROR ;
          } // if

          return IsRealRun( walk -> right ) ;
        } // else if
        else if ( walk -> left -> token == "number?" ) {
          if ( argCount != 1 ) {
            mErrorProcedure = "number?" ;
            throw ARGNUMERROR ;
          } // if

          return IsRealRun( walk -> right ) ;
        } // else if
        else if ( walk -> left -> token == "string?" ) {
          if ( argCount != 1 ) {
            mErrorProcedure = "string?" ;
            throw ARGNUMERROR ;
          } // if

          return IsStringRun( walk -> right ) ;
        } // else if
        else if ( walk -> left -> token == "boolean?" ) {
          if ( argCount != 1 ) {
            mErrorProcedure = "boolean?" ;
            throw ARGNUMERROR ;
          } // if

          return IsBoolRun( walk -> right ) ;
        } // else if
        else if ( walk -> left -> token == "symbol?" ) {
          if ( argCount != 1 ) {
            mErrorProcedure = "symbol?" ;
            throw ARGNUMERROR ;
          } // if

          return IsSymbolRun( walk -> right ) ;
        } // else if
        else if ( walk -> left -> token == "not" ) {
          if ( argCount != 1 ) {
            mErrorProcedure = "not" ;
            throw ARGNUMERROR ;
          } // if

          return IsNullRun( walk -> right ) ;
        } // else if
        else if ( walk -> left -> token == "and" ) {
          if ( argCount < 2 ) {
            mErrorProcedure = "and" ;
            throw ARGNUMERROR ;
          } // if

          return AndRun( walk -> right ) ;
        } // else if
        else if ( walk -> left -> token == "or" ) {
          if ( argCount < 2 ) {
            mErrorProcedure = "or" ;
            throw ARGNUMERROR ;
          } // if

          return OrRun( walk -> right ) ;
        } // else if
        else if ( walk -> left -> token == "+" ) {
          if ( argCount < 2 ) {
            mErrorProcedure = "+" ;
            throw ARGNUMERROR ;
          } // if

          return PlusRun( walk -> right ) ;
        } // else if
        else if ( walk -> left -> token == "-" ) {
          if ( argCount < 2 ) {
            mErrorProcedure = "-" ;
            throw ARGNUMERROR ;
          } // if

          return MinusRun( walk -> right ) ;
        } // else if
        else if ( walk -> left -> token == "*" ) {
          if ( argCount < 2 ) {
            mErrorProcedure = "*" ;
            throw ARGNUMERROR ;
          } // if

          return MulRun( walk -> right ) ;
        } // else if
        else if ( walk -> left -> token == "/" ) {
          if ( argCount < 2 ) {
            mErrorProcedure = "/" ;
            throw ARGNUMERROR ;
          } // if

          return DivRun( walk -> right ) ;
        } // else if
        else if ( walk -> left -> token == ">" ) {
          if ( argCount < 2 ) {
            mErrorProcedure = ">" ;
            throw ARGNUMERROR ;
          } // if

          return GreatRun( walk -> right ) ;
        } // else if
        else if ( walk -> left -> token == ">=" ) {
          if ( argCount < 2 ) {
            mErrorProcedure = ">=" ;
            throw ARGNUMERROR ;
          } // if

          return GreatOrEquRun( walk -> right ) ;
        } // else if
        else if ( walk -> left -> token == "<" ) {
          if ( argCount < 2 ) {
            mErrorProcedure = "<" ;
            throw ARGNUMERROR ;
          } // if

          return LessRun( walk -> right ) ;
        } // else if
        else if ( walk -> left -> token == "<=" ) {
          if ( argCount < 2 ) {
            mErrorProcedure = "<=" ;
            throw ARGNUMERROR ;
          } // if

          return LessOrEquRun( walk -> right ) ;
        } // else if
        else if ( walk -> left -> token == "=" ) {
          if ( argCount < 2 ) {
            mErrorProcedure = "=" ;
            throw ARGNUMERROR ;
          } // if

          return IntEquRun( walk -> right ) ;
        } // else if
        else if ( walk -> left -> token == "string-append" ) {
          if ( argCount < 2 ) {
            mErrorProcedure = "string-append" ;
            throw ARGNUMERROR ;
          } // if

          return StringAppendRun( walk -> right ) ;
        } // else if
        else if ( walk -> left -> token == "string>?" ) {
          if ( argCount < 2 ) {
            mErrorProcedure = "string>?" ;
            throw ARGNUMERROR ;
          } // if

          return StringGreat( walk -> right ) ;
        } // else if
        else if ( walk -> left -> token == "string<?" ) {
          if ( argCount < 2 ) {
            mErrorProcedure = "string<?" ;
            throw ARGNUMERROR ;
          } // if

          return StringLess( walk -> right ) ;
        } // else if
        else if ( walk -> left -> token == "string=?" ) {
          if ( argCount < 2 ) {
            mErrorProcedure = "string=?" ;
            throw ARGNUMERROR ;
          } // if

          return StringEqu( walk -> right ) ;
        } // else if
        else if ( walk -> left -> token == "eqv?" ) {
          if ( argCount != 2 ) {
            mErrorProcedure = "eqv?" ;
            throw ARGNUMERROR ;
          } // if

          return Eqv( walk -> right ) ;
        } // else if
        else if ( walk -> left -> token == "equal?" ) {
          if ( argCount != 2 ) {
            mErrorProcedure = "equal?" ;
            throw ARGNUMERROR ;
          } // if

          return Equal( walk -> right ) ;
        } // else if
        else if ( walk -> left -> token == "begin" ) {
          if ( argCount < 1 ) {
            mErrorProcedure = "begin" ;
            throw ARGNUMERROR ;
          } // if

          return BeginRun( walk -> right ) ;
        } // else if
        else if ( walk -> left -> token == "if" ) {
          if ( argCount != 2 && argCount != 3 ) {
            mErrorProcedure = "if" ;
            throw ARGNUMERROR ;
          } // if

          return IfRun( walk ) ;
        } // else if
        else if ( walk -> left -> token == "cond" ) {
          if ( argCount < 1 ) {
            mErrorProcedure = "COND" ;
            mErrorNode = head ;
            throw FORMATERROR ;
          } // if

          return CondRun( walk ) ;
        } // else if

      } // if
      else {
        // non function error
        mErrorProcedure = walk -> left -> token ;
        mErrorNode = walk -> left ;
        throw NONFUNCTION ;
      } // else
    } // else


    return temp ;
  } // Run()
} ;


int main( void ) {
  int uTestNum = -1;
  char ccc ; // \n
  ReadSExp a ;
  PrintSExp b ;
  Node* returnTemp ;
  bool run_check = true ; // Check whether to continue
  scanf( "%d", &uTestNum ) ;
  scanf( "%c", &ccc ) ;
  printf( "Welcome to OurScheme!\n" ) ;
  a.Reset() ;
  b.Reset() ;
  while ( a.Exit() ) {
    printf( "\n> " ) ;
    try {
      b.mIsError = false ;
      a.LineRest() ;
      a.GetExp( 8 ) ;
      a.CreateMainTree() ;
      // a.CheckList() ;
      a.mNotPrint = false ;
      returnTemp = a.Run( a.mTreeHead ) ;
      b.PrintExp( a.mTreeHead ) ;
      if ( !a.mNotPrint ) {
        b.PrintExp( returnTemp ) ;
        b.PrintLast() ;
      } // if
          
    }
    catch ( ErrorType errorType ) {
      a.PrintError( errorType ) ;
      a.mlastIsError = true ;
    } // catch


  } // while

  cout << "\nThanks for using OurScheme!" << endl;
} // main()