
/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

int solved;

/**
 * Pomocná funkce untilLeftPar.
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
 * znaků postfixExpression.
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
 * nadeklarovat a používat pomocnou proměnnou typu char.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void untilLeftPar( Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength ) 
{
    // If stack is empty there is nothing to do
    if (Stack_IsEmpty(stack)) return;

    // Get top value from stack
    char tmp;
    Stack_Top(stack, &tmp);

    // Iterate until left bracket is found or until we empty whole stack (to be sure we dont try to underflow stack - in this case stuck in infinite loop)
    while (!Stack_IsEmpty(stack) && tmp != '(')
    {
        // Append value from stack to postfix expression string
        postfixExpression[*postfixExpressionLength] = tmp;
        (*postfixExpressionLength)++;

        // Pop old top value from stack and get new one
        Stack_Pop(stack);

        Stack_Top(stack, &tmp);
    }

    // Pop the left bracket from stack
    Stack_Pop(stack);
}

/**
 * Get value of operator based on mathematical operation priority
 * 
 * @param c Character of operator we want evaluate
 * @return Relative weight of operator
 */
int eval(char c)
{
    // Lowest priority
    if(c == '+' || c == '-') 
    {
      return 1;
    }
    // Higher priority
    else if (c == '*' || c == '/') 
    {
      return 2;
    }
    // Unknown operators have zero priority
    else return 0;  
}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void doOperation( Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength )
{
    char tmp;
    // When stack is empty we only append current operator (this is to take care of unitialized state of tmp variable when there is no top of stack)
    if (Stack_IsEmpty(stack)) 
    {
        Stack_Push(stack, c);
    }
    else
    {
        // Get operator from stack
        Stack_Top(stack, &tmp);
        // If current operator have higher weight than operator on stack then we will only add it to stack
        // Higher priority operators go first to postfix expression
        if (eval(c) > eval(tmp))
        {
            Stack_Push(stack, c);
        }
        else
        {
            // If current operator have lower or equal weight as operator from stack then we will start adding higher priority operators
            // from stack to postfix expression until we empty whole stack or we get to operator with lower weight
            // NOTE: equal is there because when there is same weight then we will perform operations from right to left (operators added earlier have priority)
            while(!Stack_IsEmpty(stack) && eval(c) <= eval(tmp))
            {
                postfixExpression[*postfixExpressionLength] = tmp;
                (*postfixExpressionLength)++;
                // Pop operator from stack and get new one
                Stack_Pop(stack);

                if (Stack_IsEmpty(stack)) break;
                Stack_Top(stack, &tmp);
            }
            // Put current operator to stack
            Stack_Push(stack, c);
        }
    }
}

/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků Stack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression Znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns Znakový řetězec obsahující výsledný postfixový výraz
 */
char *infix2postfix( const char *infixExpression )
{
    // Init variables
    char tmp;
    unsigned postfixExpressionLength = 0;

    char *postfixExpression = (char*)malloc(MAX_LEN * sizeof(char));
    if (postfixExpression == NULL) return NULL;

    Stack stack;
    Stack_Init(&stack);

    // Loop until end of string or = (= is end of expression so we dont need to take care of it by algorithm)
    while (*infixExpression != '\0' && *infixExpression != '=')
    {
        // if (isalnum(*infixExpression))
        // I didnt know if ctypes header is allowed so this if statement is replacement for function isalnum from it
        if (('0' <= *infixExpression && *infixExpression <= '9') ||
            ('a' <= *infixExpression && *infixExpression <= 'z') ||
            ('A' <= *infixExpression && *infixExpression <= 'Z'))
        {
            // If character is alphanumeric value then add it to output expression
            postfixExpression[postfixExpressionLength++] = *infixExpression;
        }
        else if (*infixExpression == '(')
        {
            // If character is left bracket then add it to stack (will be used for extracting operators from stack in brackets)
            Stack_Push(&stack, *infixExpression);
        }
        else if (*infixExpression == ')')
        {
            // If right bracket is found then add operators from inside of brackets to output expression
            untilLeftPar(&stack, postfixExpression, &postfixExpressionLength);
        }
        else
        {
            // Handle operators
            doOperation(&stack, *infixExpression, postfixExpression, &postfixExpressionLength);
        }

        infixExpression++;
    }

    // We are on the end of expression so we add rest of operators from stack
    while (!Stack_IsEmpty(&stack))
    {
        Stack_Top(&stack, &tmp);
        Stack_Pop(&stack);

        postfixExpression[postfixExpressionLength++] = tmp;
    }

    // Every expression needs to end with =, so we add it to the end
    postfixExpression[postfixExpressionLength++] = '=';
    postfixExpression[postfixExpressionLength++] = '\0';

    return postfixExpression;
}

/* Konec c204.c */
