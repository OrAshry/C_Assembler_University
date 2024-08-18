#ifndef MACROCONTEXT_H
#define MACROCONTEXT_H

struct MacroContext {
    struct Macro **macro_table;
    int macro_counter;
};

#endif