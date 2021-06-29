# Suspected hardware bug:

Happens when clearing the USART.CTRLA.RXC bit, waiting for synchronization, then setting the bit and waiting for synchronization again.

# Expected behavior:

FERR, PERR, BUFOVF status flags cleared. RX buffer cleared.

# Actual behaviour:

FERR, PERR, BUFOVF status flags remain in their previous state. RX buffer is not cleared.
