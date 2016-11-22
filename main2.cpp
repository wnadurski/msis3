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
void secondGenerator(mpz_t result)
{
    mpz_t y_5, y_6, y_i, modder;
    mpz_inits(y_i, y_5, y_6, modder, NULL);
    getLastN(y_5, 4, &myData);
    getLastN(y_6, 5, &myData);

    mpz_set(y_i, y_5);
    mpz_mul(y_i, y_i, y_6);

    mpz_ui_pow_ui(modder, 2, 32);
    mpz_mod(y_i, y_i, modder);

    push(&myData, y_i);

    mpz_set(result, y_i);
}

int main(int argc, char* argv[]) {

    range r;
    mpz_t initials[6];
    mpz_t random1, random2;

    r = getRange(argv[1]);
    mpz_inits(random1, random2, NULL);

    for(int i =0; i <6; ++i) {
        mpz_init(initials[i]);
        mpz_set_str(initials[i], argv[2 + i], 10);
    }

    myData = initGenerator(6, initials);

    print_random_numbers(&secondGenerator, r);

    return 0;
}