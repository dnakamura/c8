changequote(`[[',`]]')[[]]dnl
divert([[-1]])

// Strip out the quotation marks
define([["]],[[]])

// common code to push a token

// Ignore token defs
define([[TOK]],[[]])
define([[KEYWORD]],[['$1' { push_token(tok::kw_[[]]$1);};]])
define([[PUNCTUATOR]],[['$2' { push_token(tok::$1);};]])
divert[[]]dnl
