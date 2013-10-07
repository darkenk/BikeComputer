#ifndef UTILS_H
#define UTILS_H

#define SET_BIT(reg, bit) (reg |= (1 << bit))
#define SET_BITS(reg, bit_mask) ( reg = bit_mask)
#define CLEAR_BIT(reg, bit) (reg &= ~(1 << bit))
#define IS_BIT_SET(reg, bit) (reg & (1 << bit))

#endif // UTILS_H
