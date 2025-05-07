#include <digisim/utility/print.h>

#include <stdio.h>
#include <stdlib.h>

char di_bit_to_char(DiBit bit) {
    switch (bit) {
    case DI_BIT_LOW: return '0';
    case DI_BIT_HIGH: return '1';
    case DI_BIT_ERROR: return 'E';
    case DI_BIT_UNKNOWN: return 'U';
    default: return 'X';
    }
}

void di_signal_format(DiSignal *signal, char *output) {
    for (size_t a = 0; a < signal->bits; a++) {
        output[signal->bits - 1 - a] = di_bit_to_char(di_signal_get(signal, a));
    }
}

void di_signal_print(DiSignal *signal, const char *terminator) {
    char *buffer = malloc(signal->bits + 1);

    // Could probably print by hand, but using format here for consistency.
    di_signal_format(signal, buffer);

    buffer[signal->bits] = '\0';

    if (!terminator) {
        terminator = "\n";
    }

    printf("%s%s", buffer, terminator);

    free(buffer);
}
