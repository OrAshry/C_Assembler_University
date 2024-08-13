#include "stringSplit.h"

struct string_split split_string(char *str, const char *delimiter)
{
    int strings_count = 0, in_quotes = 0;
    struct string_split split_result = {0};

    /** Skip leading whitespaces **/
    while (isspace((unsigned char)*str))
        str++;

    /** If the string is empty after removing whitespaces **/
    if (*str == NULL_BYTE)
        return split_result;

    /*while (str && *str != '\0')*/
    while (str)
    {
        /** If string final char is enter **/
        if (*str == '\n')
        {
            *str = NULL_BYTE;
            break;
        }

        /** Check for the start or end of a quoted substring **/
        if (*str == '\"')
            in_quotes = !in_quotes; /** Toggle in_quotes flag **/

        /** Store current string in the list **/
        if (*str != NULL_BYTE)
        {
            split_result.string[strings_count++] = str;
        }

        if (in_quotes)
        {
            /** Find the closing quote **/
            str = strchr(str + 1, '\"');
            if (str)
            {
                str++; /** Move past the closing quote **/
            }
            else
            {
                break; /** No closing quote found **/
            }
        }
        else
        {
            /** Move to the next string **/
            str = strpbrk(str, delimiter);

            if (str)
            {
                *str = NULL_BYTE; /** Null terminate **/
                str++;

                /** Skip additional whitespaces **/
                while (isspace((unsigned char)*str))
                    str++;
            }
        }
    }

    /** Update strings counter in the list **/
    split_result.size = strings_count;

    /* Remove '\n' from end of string if exists */
    if (split_result.string[strings_count - 1][strlen(split_result.string[strings_count - 1]) - 1] == '\n')
    {
        split_result.string[strings_count - 1][strlen(split_result.string[strings_count - 1]) - 1] = '\0';
    }

    /** Return string_split structure **/
    return split_result;
}
