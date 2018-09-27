//
// Created by bertrand on 27/09/18.
//

#include "core.h"
#include "main.h"

uint8_t core_ram_read(core *c, uint16_t address) {
    assert(address < RAM_SIZE - 1);
#ifdef DEBUG
    printf("RAM READ ram[%d]=%d\n", address, c->ram[address]);
#endif
    return c->ram[address];
}

void core_ram_write(core *c, uint16_t address, uint8_t value) {
    assert(address < RAM_SIZE - 1);
#ifdef DEBUG
    printf("RAM WRITE ram[%d]=%d\n", address, value);
#endif
    c->ram[address] = value;
}

void core_init(core *c) {
    memset(c, 0, sizeof(core)); // zero out memory and registers

    // Init boot rom
    uint8_t boot_buffer[BOOTSTRAP_ROM_SIZE];
    FILE *boot = fopen("resources/boot.bin", "r");
    fread(boot_buffer, sizeof(uint8_t), BOOTSTRAP_ROM_SIZE, boot);
    memcpy(c->ram, boot_buffer, BOOTSTRAP_ROM_SIZE);
    c->registers.pc = 0;
    core_setup_operations(c);
}

void core_loop(core *c) {
    while(1) {
        uint8_t code = core_ram_read(c, c->registers.pc++);
        if(code == 0xCB) { // extended operation
            code = core_ram_read(c, c->registers.pc++);
        }
        operation op = c->ops[code];

        if(op.handler == NULL) {
            printf("UNDEFINED OPERATION %0x\n", code);
            exit(EXIT_FAILURE);
        }

        uint8_t *args = malloc(op.operand_number * sizeof(uint8_t));
        for(int i = 0; i < op.operand_number; i++)
            args[i] = core_ram_read(c, c->registers.pc++);
        op.handler(c, args);
        sleep(1);
    }
}

void core_register_op(core *c, uint8_t code, operation op) {
    assert(c->ops[code].handler == NULL);
    c->ops[code] = op;
}

void core_register_ext_op(core *c, uint8_t code, operation op) {
    assert(c->ext_ops[code].handler == NULL);
    c->ext_ops[code] = op;
}

void core_setup_operations(core *c) {
    core_register_op(c, 0x00, (operation) {
        "NOP", 0, noop
    });
}