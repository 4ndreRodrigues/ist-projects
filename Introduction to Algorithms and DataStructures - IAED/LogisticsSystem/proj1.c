/*
 * File:  proj1.c
 * Author:  André Jacinto de Sousa Rodrigues  92421
 * Description: Logistics system program for first project of IAED.
*/

/* Libraries */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Constants defenition */
#define MAX_ORDERS 500
#define MAX_ORDER_PRODUCTS 200
#define MAX_PRODUCTS 10000
#define MAX_DESCRIPTON 63

/*  structs defenition */
typedef struct {
    int idp;  /* product id*/
    char description[MAX_DESCRIPTON];
    int price;
    int weight;
    int stock;
}Product;

typedef struct {
    Product order_products[MAX_ORDER_PRODUCTS];
    int ido;  /* Order id*/
    int weight;
    int orderProductsNumber;  /* Number of pruducts in the order */
}Order;


Product products[MAX_PRODUCTS]; /* Array of products */
Order orders[MAX_ORDERS]; /* Array of orders */

/* Function declarations */
void newProduct();
void addStock(int productsNumber);
void newOrder();
void addProduct(int ordersNumber, int productsNumber);
void removeStock(int productsNumber);
void removeProduct(int ordersNumber, int productsNumber);
void getOrderPrice(int ordersNumber);
void changePrice(int ordersNumber, int productsNumber);
void searchProduct(int ordersNumber, int productsNumber);
void getMaxOrderProduct(int ordersNumber, int productsNumber);
void listProducts(int productsNumber);
void listOrder(int ordersNumber);
void lista(int productsNumber);

/* Read the commands from stdin and execute them */
int main() {

    char c;
    int productsNumber = 0;  /* Number of products in the system*/
    int ordersNumber = 0;  /* Number of orders in the system */

    while ((c = getchar()) != 'x' && c != EOF) {
        switch (c) {
            case 'a':
                getchar();
                productsNumber++;
                newProduct();
                break;
            case 'q':
                addStock(productsNumber);
                break;
            case 'N':
                ordersNumber++;
                newOrder();
                break;
            case 'A':
                addProduct(ordersNumber, productsNumber);
                break;
            case 'r':
                removeStock(productsNumber);
                break;
            case 'R':
                removeProduct(ordersNumber, productsNumber);
                break;
            case 'C':
                getOrderPrice(ordersNumber);
                break;
            case 'p':
                changePrice(ordersNumber, productsNumber);
                break;
            case 'E':
                searchProduct(ordersNumber, productsNumber);
                break;
            case 'm':
                getMaxOrderProduct(ordersNumber, productsNumber);
                break;
            case 'l':
                listProducts(productsNumber);
                break;
            case 'L':
                listOrder(ordersNumber);
                break;
        }
    }
    return 0;
}

/*----------------------------------------------------------------------------*/
/* Auxiliar Functions */

/* Generates a new idp */
int idpGenerator() {
    static int idp = -1;
    idp++;
    return idp;
}

/* Generates a new ido */
int idoGenerator() {
    static int ido = -1;
    ido++;
    return ido;
}

/* Read the description from stdin */
void readDescription(char str[MAX_DESCRIPTON]) {
    int i;
    char c;

    for (i = 0; (c = getchar()) != ':' && c != '\n' && c != '\0'; i++) {
        str[i] = c;
    }
    str[i] = '\0';
}


void mergeProducts(Product array[], int left, int m, int right) {
    int i,j,k;
    Product aux[MAX_PRODUCTS];

    for (i = m + 1; i > left; i--) {
        aux[i - 1] = array[i - 1];
    }
    for (j = m; j < right; j++) {
        aux[right + m - j] = array[j + 1];
    }
    for (k = left; k <= right; k++) {
        if (aux[j].price < aux[i].price || i > m) {
            array[k] = aux[j--];
        }
        else {
            array[k] = aux[i++];
        }
    }
}

/* Sort products by price */
void mergesortProducts(Product array[], int left, int right) {
    int m = (right + left) / 2;

    if (right <= left) return;
    mergesortProducts(array, left, m);
    mergesortProducts(array, m + 1, right);
    mergeProducts(array, left, m, right);
}

void mergeOrders(Product array[], int left, int m, int right) {
    int i,j,k;
    Product aux[MAX_PRODUCTS];

    for (i = m + 1; i > left; i--) {
        aux[i - 1] = array[i - 1];
    }
    for (j = m; j < right; j++) {
        aux[right + m - j] = array[j + 1];
    }
    for (k = left; k <= right; k++) {
        if (strcmp(aux[j].description, aux[i].description) < 0 || i > m) {
            array[k] = aux[j--];
        }
        else {
            array[k] = aux[i++];
        }
    }
}

/* Sort orders alphabetically */
void mergesortOrders(Product array[], int left, int right) {
    int m = (right + left) / 2;

    if (right <= left) return;
    mergesortOrders(array, left, m);
    mergesortOrders(array, m + 1, right);
    mergeOrders(array, left, m, right);
}

/* Verify if a product is in the system and return the position */
int hasProduct(int productsNumber, int idp) {
    int i;

    for (i = 0; i < productsNumber; i++) {
        if (products[i].idp == idp) {
            return i;
        }
    }
    return -1;
}

/* Verify if a order is in the system an retirn the position */
int hasOrder(int ordersNumber, int ido) {
    int i;

    for (i = 0; i < ordersNumber; i++) {
        if (orders[i].ido == ido) {
            return i;
        }
    }
    return -1;
}

/* Verify if a product is in an order and return the position */
int hasProductOrder(int ido, int idp) {
    int i;

    for (i = 0; i < orders[ido].orderProductsNumber; i++) {
        if (orders[ido].order_products[i].idp == idp) {
            return i;
        }
    }
    return -1;
}

/* Add a product in an order */
void addProductAux(int o, int p) {

    orders[o].order_products[orders[o].orderProductsNumber].idp = products[p].idp;
    strcpy(orders[o].order_products[orders[o].orderProductsNumber].description, products[p].description);
    orders[o].order_products[orders[o].orderProductsNumber].price = products[p].price;
    orders[o].order_products[orders[o].orderProductsNumber].weight = products[p].weight;
    orders[o].order_products[orders[o].orderProductsNumber].stock = 0;
    orders[o].orderProductsNumber += 1;
}

/* Add Stock to a product of an order */
void addStockOrder(int p, int stockAdd, int o, int h) {

    products[p].stock -= stockAdd;
    orders[o].order_products[h].stock += stockAdd;
    orders[o].weight += (products[p].weight * stockAdd);
}

/* Remove a product from an order */
void removeProductAux(int o, int p, int h) {
    int i;

    orders[o].weight -= (orders[o].order_products[h].stock * products[p].weight);
    for (i = h; i < orders[o].orderProductsNumber - 1; i++) {
        orders[o].order_products[i].idp = orders[o].order_products[i + 1].idp;
        strcpy(orders[o].order_products[i].description, orders[o].order_products[i + 1].description);
        orders[o].order_products[i].price = orders[o].order_products[i + 1].price;
        orders[o].order_products[i].weight = orders[o].order_products[i + 1].weight;
        orders[o].order_products[i].stock = orders[o].order_products[i + 1].stock;
    }
    orders[o].orderProductsNumber -= 1;
}

/*------------------------------------------------------------------------------------------------*/
/* Main Functions */

/* Command a: Add a new product to the system */
void newProduct() {
    char description[MAX_DESCRIPTON];
    int price, weight, stock, idp;

    idp = idpGenerator();
    readDescription(description);
    scanf("%d:%d:%d", &price, &weight, &stock);
    products[idp].idp = idp;
    strcpy(products[idp].description, description);
    products[idp].price = price;
    products[idp].weight = weight;
    products[idp].stock = stock;
    printf("Novo produto %d.\n", idp);
}

/* Command q: Add stock to a product */
void addStock(int productsNumber) {
    int idp, p, stock;

    scanf(" %d:%d", &idp, &stock);
    if ((p = hasProduct(productsNumber, idp)) >= 0) {
        products[p].stock += stock;
    }
    else {
        printf("Impossivel adicionar produto %d ao stock. Produto inexistente.\n", idp);
    }
}

/* Command N: Creat a new order */
void newOrder() {
    int ido;

    ido = idoGenerator();
    orders[ido].ido = ido;
    orders[ido].orderProductsNumber = 0;
    orders[ido].weight = 0;
    printf("Nova encomenda %d.\n", ido);
}

/* Command A: Add a product to an order */
void addProduct(int ordersNumber, int productsNumber) {
    int idp, ido, o, p, h, stock;

    scanf(" %d:%d:%d", &ido, &idp, &stock);
    if ((o = hasOrder(ordersNumber, ido)) >= 0) {
        if ((p = hasProduct(productsNumber, idp)) >= 0) {
            if (products[p].stock >= stock) {
                if ((orders[o].weight + (stock * products[p].weight)) <= 200) {
                    if ((h = hasProductOrder(o, idp)) >= 0) {
                        addStockOrder(p, stock, o, h);
                    }
                    else {
                        addProductAux(o, p);
                        addStockOrder(p, stock, o, hasProductOrder(o, idp));
                    }
                }
                else {
                    printf("Impossivel adicionar produto %d a encomenda %d. Peso da encomenda excede o maximo de 200.\n", idp, ido);
                }
            }
            else {
                printf("Impossivel adicionar produto %d a encomenda %d. Quantidade em stock insuficiente.\n", idp, ido);
            }
        }
        else {
            printf("Impossivel adicionar produto %d a encomenda %d. Produto inexistente.\n", idp, ido);
        }
    }
    else {
        printf("Impossivel adicionar produto %d a encomenda %d. Encomenda inexistente.\n", idp, ido);
    }
}

/* Command r: Removes stock from a product */
void removeStock(int productsNumber) {
    int idp, p, stock;

    scanf(" %d:%d", &idp, &stock);
    if ((p = hasProduct(productsNumber, idp)) >= 0) {
        if (products[p].stock >= stock) {
            products[p].stock -= stock;
        }
        else {
            printf("Impossivel remover %d unidades do produto %d do stock. Quantidade insuficiente.\n", stock, idp);
        }
    }
    else {
        printf("Impossivel remover stock do produto %d. Produto inexistente.\n", idp);
    }
}

/* Command R: Removes a product from an order */
void removeProduct(int ordersNumber, int productsNumber) {
    int idp, ido, o, p, h;

    scanf(" %d:%d", &ido, &idp);
    if ((o = hasOrder(ordersNumber, ido)) >= 0) {
        if ((p = hasProduct(productsNumber, idp)) >= 0) {
            if ((h = hasProductOrder(o, idp)) >= 0) {
                products[p].stock += orders[o].order_products[h].stock;
                removeProductAux(o, p, h);
            }
        }
        else {
            printf("Impossivel remover produto %d a encomenda %d. Produto inexistente.\n", idp, ido);
        }
    }
    else {
        printf("Impossivel remover produto %d a encomenda %d. Encomenda inexistente.\n", idp, ido);
    }
}

/* Command C: calculates the cost of an order */
void getOrderPrice(int ordersNumber) {
    int ido, o, h, orderPrice = 0;

    scanf(" %d", &ido);
    if ((o = hasOrder(ordersNumber, ido)) >= 0) {
        for (h = 0; h < orders[o].orderProductsNumber; h++) {
            orderPrice += (orders[o].order_products[h].price * orders[o].order_products[h].stock);
        }
        printf("Custo da encomenda %d %d.\n", ido, orderPrice);
    }
    else {
        printf("Impossivel calcular custo da encomenda %d. Encomenda inexistente.\n", ido);
    }
}

/* Command p: Change the price from a product */
void changePrice(int ordersNumber, int productsNumber) {
    int idp, o, p, h, price;

    scanf(" %d:%d", &idp, &price);
    if ((p = hasProduct(productsNumber, idp)) >= 0) {
        products[p].price = price;
        for (o = 0; o < ordersNumber; o++) {
            if ((h = hasProductOrder(o, idp)) >= 0) {
                orders[o].order_products[h].price = price;
            }
        }
    }
    else {
        printf("Impossivel alterar preco do produto %d. Produto inexistente.\n", idp);
    }
}

/* Command E: Lists the description and quantity of a product in an order */
void searchProduct(int ordersNumber, int productsNumber) {
    int ido, idp, o, p, h;

    scanf(" %d:%d", &ido, &idp);
    if ((o = hasOrder(ordersNumber, ido)) >= 0) {
        if ((p = hasProduct(productsNumber, idp)) >= 0) {
            if ((h = hasProductOrder(o, idp)) >= 0) {
                printf("%s %d.\n", orders[o].order_products[h].description, orders[o].order_products[h].stock);
            }
            else {
                printf("%s 0.\n", products[p].description);
            }
        }
        else {
            printf("Impossivel listar produto %d. Produto inexistente.\n", idp);
        }
    }
    else {
        printf("Impossivel listar encomenda %d. Encomenda inexistente.\n", ido);
    }
}

/* Command m: lists the order that a product has more stock */
void getMaxOrderProduct(int ordersNumber, int productsNumber) {
    int idp, o, p, h, maxStock = 0, idoMaxStock;

    scanf(" %d", &idp);
    if ((p = hasProduct(productsNumber, idp)) >= 0) {
        for (o = 0; o < ordersNumber; o++) {
            if ((h = hasProductOrder(o, idp)) >= 0) {
                if (orders[o].order_products[h].stock > maxStock) {
                    maxStock = orders[o].order_products[h].stock;
                    idoMaxStock = o;
                }
            }
        }
        if (maxStock <= 0) return;
        printf("Maximo produto %d %d %d.\n", idp, idoMaxStock, maxStock);
    }
    else {
        printf("Impossivel listar maximo do produto %d. Produto inexistente.\n", idp);
    }
}

/* Command l: list all products in the system by price order */
void listProducts(int productsNumber) {
    int i;
    Product array[MAX_PRODUCTS];

    for (i = 0; i < productsNumber; i++) {
        array[i].idp = products[i].idp;
        strcpy(array[i].description, products[i].description);
        array[i].price = products[i].price;
        array[i].weight = products[i].weight;
        array[i].stock = products[i].stock;
    }
    mergesortProducts(array, 0, productsNumber - 1);
    printf("Produtos\n");
    for (i = 0; i < productsNumber; i++) {
        printf("* %s %d %d\n", array[i].description, array[i].price, array[i].stock);
    }
}

/* Command L: list all products in an order in alphabetical order from the description */
void listOrder(int ordersNumber) {
    int ido, o, h, i;
    Product array[MAX_ORDER_PRODUCTS];

    scanf(" %d", &ido);
    if ((o = hasOrder(ordersNumber, ido)) >= 0) {
        for (h = 0; h < orders[o].orderProductsNumber; h++) {
            array[h].idp = orders[o].order_products[h].idp;
            strcpy(array[h].description, orders[o].order_products[h].description);
            array[h].price = orders[o].order_products[h].price;
            array[h].weight = orders[o].order_products[h].weight;
            array[h].stock = orders[o].order_products[h].stock;
        }
        mergesortOrders(array, 0, orders[o].orderProductsNumber - 1);
        printf("Encomenda %d\n", ido);
        for (i = 0; i < orders[o].orderProductsNumber; i++) {
            printf("* %s %d %d\n", array[i].description, array[i].price, array[i].stock);
        }
    }
    else {
        printf("Impossivel listar encomenda %d. Encomenda inexistente.\n", ido);
    }
}
