#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "chunk.h"
#include "vm.h"
#include "debug.h"

int main(int argc, const char * argv[]) {
    printf("Zhenzhu VM is in progress... \n");

    VM vm;
    initVM(&vm);

    Chunk chunk;
    initChunk(&chunk);
    writeChunk(&chunk, OP_NOP, 123);
    writeChunk(&chunk, OP_NOP, 123);

    disassembleChunk(&chunk, "test chunk");
    freeChunk(&chunk);
    freeVM(&vm);

    return 0;
}