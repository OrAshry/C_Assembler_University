#include "symbolTable.h"

/**
 * @brief Converts an integer to its octal string representation.
 *
 * This function takes an integer `n` and converts it into its octal representation,
 * storing the result in the provided `octalStr` buffer. The `size` parameter indicates
 * the number of octal digits to generate (including leading zeros).
 *
 * @param n The integer to convert to an octal string.
 * @param octalStr A character buffer to store the resulting octal string.
 * @param size The number of octal digits to produce (not including the null terminator).
 */
void intToOctalString(int n, char *octalStr, int size) {
    int i;
    octalStr[size] = '\0';  
    for (i = size - 1; i >= 0; i--) {
        octalStr[i] = (n & 7) + '0';  /* Convert the last 3 bits to an octal digit */
        n >>= 3;  /* Shift right by 3 bits to process the next digit */
    }
}

/**
 * @brief Prints the code image to the specified file.
 *
 * This function writes the code image stored in the translation structure to the specified file.
 * The code image is converted to a 5-digit octal format, and each line contains the address and
 * the corresponding octal code.
 *
 * @param p A pointer to the translation structure containing the code image.
 * @param file A pointer to the file where the code image will be printed.
 */
void fprint_code_image(const translation_ptr p, FILE *file) {
    int i;
    char octalStr[6];  /* Adjusted to 6 to accommodate 5 octal digits plus the null terminator */

    for (i = 0; i < machine_code_ptr->IC; i++) {
        if (p->code_image[i] != '\0') {
            intToOctalString(p->code_image[i], octalStr, 5);  /* Using 5 for 5-digit octal */
            fprintf(file, "%04d %s\n", i, octalStr);
        }
    }
}

/**
 * @brief Prints the data image to the specified file.
 *
 * This function writes the data image stored in the translation structure to the specified file.
 * The data image is converted to a 5-digit octal format, and each line contains the address and
 * the corresponding octal data. The address is printed with a 4-digit format.
 *
 * @param p A pointer to the translation structure containing the data image.
 * @param file A pointer to the file where the data image will be printed.
 */
void fprint_data_image(const translation_ptr p, FILE *file) {
    int i;
    char octalStr[6];  /* 5 digits plus the null terminator */

    for (i = 0; i < machine_code_ptr->DC; i++) {
        if ((p->data_image[i] != '\0') || (p->data_image[i] == 0)) {
            intToOctalString(p->data_image[i], octalStr, 5);  /* Convert to 5-digit octal */
            if(machine_code_ptr->IC == 0) 
            {
                fprintf(file, "%04d %s", i + 100, octalStr);  /* Print the address with 4 digits and octal with 5 digits */
                if(i < (machine_code_ptr->DC + 100) - 1) 
                {
                    fprintf(file, "\n");
                }
            }
            else 
            {
                fprintf(file, "%04d %s", i + machine_code_ptr->IC, octalStr);  /* Print the address with 4 digits and octal with 5 digits */
                if(i < machine_code_ptr->DC - 1) 
                {
                    fprintf(file, "\n");
                }
            }
        }
    }
}
