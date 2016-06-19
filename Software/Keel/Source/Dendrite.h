#ifndef DENDRITE_H_
#define DENDRITE_H_

// Call this once after all initialization is done
// It will set up wireless module
void Dendrite_Init();

// Called when wireless module triggers interrupt
// (IRQ low level)
void Dendrite_Interrupt();

#endif //DENDRITE_H_