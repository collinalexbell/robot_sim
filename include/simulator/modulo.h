#ifndef MODULO_H
#define MODULO_H


inline int modulo(int a, int b) {
    const int result = a % b;
    return result >= 0 ? result : result + b;
}


#endif
