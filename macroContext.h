#ifndef MACROCONTEXT_H
#define MACROCONTEXT_H

/* MacroContext structure holding macro_table and macro counter */
struct MacroContext
{
    struct Macro **macro_table;
    int macro_counter;
};

#endif
