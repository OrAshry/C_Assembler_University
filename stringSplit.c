#include "stringSplit.h"

/**
 * @brief Split a string into substrings using a delimiter
 * 
 * @param str The string to split
 * @param delimiter The delimiter to split the string by
 * @return struct string_split A structure containing the split strings
 */
struct string_split split_string(char *str, const char *delimiter)
{
    int strings_count = 0, in_quotes = 0;
    struct string_split split_result = {0};
    char *temp_str = (char *)allocateMemory(strlen(str) + 1, sizeof(char), MALLOC_ID);
    strcpy(temp_str, str);

    /** Skip leading whitespaces **/
    while (isspace((unsigned char)*temp_str))
        temp_str++;

    /** If the string is empty after removing whitespaces **/
    if (*temp_str == NULL_BYTE)
        return split_result;

    /*while (str && *str != '\0')*/
    while (temp_str)
    {
        /** If string final char is enter **/
        if (*temp_str == '\n')
        {
            *temp_str = NULL_BYTE;
            break;
        }

        /** Check for the start or end of a quoted substring **/
        if (*temp_str == '\"')
            in_quotes = !in_quotes; /** Toggle in_quotes flag **/

        /** Store current string in the list **/
        if (*temp_str != NULL_BYTE)
        {
            split_result.string[strings_count++] = temp_str;
        }

        if (in_quotes)
        {
            /** Find the closing quote **/
            temp_str = strchr(temp_str + 1, '\"');
            if (temp_str)
            {
                temp_str++; /** Move past the closing quote **/
            }
            else
            {
                break; /** No closing quote found **/
            }
        }
        else
        {
            /** Move to the next string **/
            temp_str = strpbrk(temp_str, delimiter);

            if (temp_str)
            {
                *temp_str = NULL_BYTE; /** Null terminate **/
                temp_str++;

                /** Skip additional whitespaces **/
                while (isspace((unsigned char)*temp_str))
                    temp_str++;
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
