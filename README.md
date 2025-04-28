# C Assembler

A two-pass assembler written in C, developed as part of a university computer architecture course.  
The assembler reads custom assembly language files (.as) and translates them into machine code output files.

---

## 📂 Features

- **First Pass**: Parse assembly code, collect labels, and calculate memory addresses.
- **Second Pass**: Resolve symbols, generate final machine code, and handle external references.
- **Symbol Table Management**: Add, search, and manage symbols efficiently.
- **Output File Generation**: Produce `.ob` (object), `.ent` (entries), and `.ext` (externals) files.
- **Memory Management**: Handle machine code and data storage.
- **Error Detection and Reporting**: Catch redefinitions, undeclared symbols, and memory overflows.
- **Makefile Included**: Easy project compilation with one command.
- **Multiple File Handling**: Assemble multiple `.as` files at once.

---

## 🚀 How to Compile

Use the Makefile provided:
```bash
make
```
or manually compile:
```bash
gcc *.c -o assembler
```
This will generate an executable named assembler.

___

🛠 How to Run

Run the assembler with one or more .as files:
```bash
./assembler file1.as file2.as ...
```
For each input file, the assembler generates corresponding output files if no errors are found.

📄 Example

Input file: example.as
Generated output:
- example.ob — Machine code
- example.ent — Entry symbols (if any)
- example.ext — External symbols (if any)

___

🙋‍♂️ My Contribution

This project was developed collaboratively.
My primary contributions include:
- Implementing the symbol table management (addSymbol.c, symbolTable.h, symbolSearch.c).
- Building the first pass logic (firstPass.c) — parsing labels, instructions, and directives.
- Developing the second pass (secondPass.c) — resolving addresses and external symbols.
- Creating output file generation (output.c) for .ob, .ent, and .ext files.
- Designing memory management functions (translate.c).
- Creating octal output formatting (printFunction.c).
- Writing or adapting the Makefile for easy compilation.
- Overall, I focused on building the core logic for symbol management, machine code generation, and output formatting.
  
 This project received a grade of 90 at the university.

---

📈 Project Status

- Fully working and tested with multiple assembly input files.
- Demonstrates strong understanding of assembler logic, file handling, memory management, and C programming.

---

📜 License

Open for educational and personal use.
