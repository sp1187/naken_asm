#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/assembler.h"
#include "common/ifdef_expression.h"
#include "common/macros.h"
#include "common/tokens.h"

int errors = 0;

const char *answer_1[] = { "one", "two", "three", NULL };
const char *answer_2[] = { "ten", "two", "three", NULL };

void test_macro(const char *macro, const char **answer)
{
  struct _asm_context asm_context = { 0 };
  char token[TOKENLEN];
  int token_type;
  int index = 0;
  //int num;

  printf("Testing: %s ... ", macro);

  tokens_open_buffer(&asm_context, macro);
  tokens_reset(&asm_context);

  while(1)
  {
    token_type = tokens_get(&asm_context, token, TOKENLEN);

    if (token_type == TOKEN_EOF) { break; }
    if (token_type == TOKEN_EOL) { continue; }

    // CAREFUL HERE
    if (strcmp(token, ".") == 0) { continue; }

    if (strcasecmp(token, "macro") == 0)
    {
      if (macros_parse(&asm_context, IS_MACRO) != 0)
      {
        errors++;
        return;
      }

//macros_print(&asm_context.macros, stdout);

      printf("\n");
    }
      else
    {
      if (answer[index] == NULL)
      {
        printf("Error: Macro should be finished %s:%d\n", __FILE__, __LINE__);
        errors++;
        return;
      }

      printf("'%s' -> '%s'\n", token, answer[index]);

      if (strcmp(token, answer[index]) != 0)
      {
        printf("Error: Macro doesn't match %s:%d\n", __FILE__, __LINE__);
        errors++;
        return;
      }

      index++;
    }
  }

  tokens_close(&asm_context);
}

void test_if(const char *statement, int answer)
{
  char token[TOKENLEN];
  int token_type;
  struct _asm_context asm_context = { 0 };

  printf("Testing: %s ... ", statement);

  tokens_open_buffer(&asm_context, statement);
  tokens_reset(&asm_context);

  while(1)
  {
    token_type = tokens_get(&asm_context, token, TOKENLEN);

    if (token_type == TOKEN_EOF) { break; }
    if (token_type == TOKEN_EOL) { continue; }

    if (strcmp(token, ".") == 0) { continue; }

    if (strcasecmp(token, "if") == 0)
    {
      int value = eval_ifdef_expression(&asm_context);

      if (value != answer)
      {
        printf("FAIL\n");
        errors++;
      }
        else
      {
        printf("PASS\n");
      }

      return;
    }
  }
}

int main(int argc, char *argv[])
{
  printf("macros.o test\n");

  test_macro(".macro blah\n"
       "one\n"
       "two\n"
       "three\n"
       ".endm\n"
       "blah\n", answer_1);

  test_macro(".macro blah(param_underscore)\n"
       "param_underscore\n"
       "two\n"
       "three\n"
       ".endm\n"
       "blah(ten)\n", answer_2);

  test_macro(".macro blah (param_underscore)\n"
       "param_underscore\n"
       "two\n"
       "three\n"
       ".endm\n"
       "blah (ten)\n", answer_2);

  test_macro(".macro blah\t( param_underscore ) \n"
       "param_underscore\n"
       "two\n"
       "three\n"
       ".endm\n"
       "blah ( ten )\n", answer_2);

  test_macro(".macro blah\t( param_underscore ) \n"
       "param_underscore\n"
       "two\n"
       "three\n"
       ".endm\n"
       "blah ( ten )\n", answer_2);

  test_if(".if (15 < 256)\n", 1);
  test_if(".if (257 < 256)\n", 0);
  test_if(".if 15 < 256\n", 1);
  test_if(".if 257 < 256\n", 0);

  printf("Total errors: %d\n", errors);
  printf("%s\n", errors == 0 ? "PASSED." : "FAILED.");

  if (errors != 0) { return -1; }

  return 0;
}

