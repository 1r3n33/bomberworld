#ifndef IRQ_H
#define IRQ_H

typedef void (*irq_handler_t)();

void set_irq(u8 flag, irq_handler_t handler);

#endif // IRQ_H
