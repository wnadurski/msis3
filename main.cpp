#include <stdbool.h>
#include "stdio.h"
#include "string.h"
#include "gmp.h"

struct _range {
    mpz_t from;
    mpz_t to;
} typedef range;

//struct _element {
//    mpz_t value;
//    _element* next;
//    _element* prev;
//} typedef element;

typedef void (*generator)(mpz_t);

range getRange(char* parameter)
{
    range r;
    char buffers[3][256];
    char *pch;

    mpz_inits(r.from, r.to, NULL);

    strcpy(buffers[0], parameter);
    pch = strtok(buffers[0], "-");
    for(int i =0 ; i < 2 ; ++i)
    {
        mpz_t *number;
        i == 0 ? number = &(r.from) : number=&(r.to);
        mpz_set_str((*number), pch, 10);

        pch = strtok(NULL, "-");
    }

    return r;
}

void print_random_numbers(generator getNext, range r)
{
    mpz_t i, num;
    mpz_inits(i, num, NULL);


    while(mpz_cmp(i, r.to) < 0)
    {
        getNext(num);

        if(mpz_cmp(i, r.from) >= 0)
        {
            gmp_printf("%Zd ", num);
        }

        mpz_add_ui(i, i, 1);
    }
}

struct _generatorData
{
    bool initialized;
    int elementsNumber;
    int last;
    mpz_t* elements;
} typedef generatorData;

generatorData initGenerator(int elementsNumber, mpz_t *elements)
{
    generatorData data;
    data.initialized = true;
    data.elementsNumber = elementsNumber;
    data.elements = elements;
    data.last = elementsNumber-1;

    return data;
}

void push(struct _generatorData* data, mpz_t val)
{
    data->last = (data->last + 1) % data->elementsNumber;
    mpz_set(data->elements[data->last], val);
}

void getLastN(mpz_t result, int n, struct _generatorData* data)
{
    int index = (data->last - n);
    while(index <0) {
        index = data->elementsNumber + index;
    }

    index = index % data->elementsNumber;

    mpz_set(result, data->elements[index]);
}

generatorData myData;
void firstGenerator(mpz_t result)
{
    mpz_t x_1, x_i, modder;
    mpz_inits(x_i, x_1, modder, NULL);
    getLastN(x_1, 0, &myData);

    mpz_set(x_i, x_1);
    mpz_mul(x_i, x_i, x_i); //^2
    mpz_mul_ui(x_i, x_i, 71738);
    mpz_add_ui(x_i, x_i, 27118);

    mpz_ui_pow_ui(modder, 2, 32);
    mpz_mod(x_i, x_i, modder);

    push(&myData, x_i);

    mpz_set(result, x_i);
}

int main(int argc, char* argv[]) {

    range r;
    mpz_t initials[1];
    mpz_t random1, random2;

    r = getRange(argv[1]);
    mpz_inits(initials[0], random1, random2, NULL);
    mpz_set_str(initials[0], argv[2], 10);

    myData = initGenerator(1, initials);

    print_random_numbers(&firstGenerator, r);

    return 0;
}