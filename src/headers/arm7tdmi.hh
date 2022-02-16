#ifndef ARM7TDMI_H
#define ARM7TDMI_H
#include <array>
class arm7tdmi
{
private:

    enum _mode
    {
        ARM_MODE,
        THUMB_MODE
    } mode;

    enum _registers
    {
        R0, R1, R2, R3, R4, R5, R6, R7, R8, R8_fiq, R9,   // General Purpose Registers
        R9_fiq, R10, R10_fiq, R11, R11_fiq, R12, R12_fiq, // General Purpose Registers

        R13, R13_fiq, R13_svc, R13_abt, R13_irq, R13_und, // Stack Pointer Register

        R14, R14_fiq, R14_svc, R14_abt, R14_irq, R14_und, // Link Register

        R15,     // Program Counter

        CPSR, SPSR_fiq, SPSR_svc, SPSR_abt, SPSR_irq, SPSR_und, // Program Status Registers
        
        NUM       // Number of Registers
    };

    std::array<uint32_t, NUM> registers;

public:
    arm7tdmi();
    ~arm7tdmi();

    /*Setting the CPU MODE
    ARM_MODE: 32-bit mode
    THUMB_MODE: 16-bit mode
    If given a non-existent mode, switching to default mode(THUMB_MODE)
    */
    void set_mode(enum _mode mode)
    {
        if (mode != ARM_MODE && mode != THUMB_MODE)
        {
            this->mode = THUMB_MODE;
        }
        this->mode = mode;
    }

    enum _mode get_mode()
    {
        return this->mode;
    }
};

#endif /* !ARM7TDMI_H */