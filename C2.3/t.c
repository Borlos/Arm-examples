/*
Test de llamada a funcion en ensamblador desde C:
Función que declara 6 int con valor 1 y llama a 
la funcion en ensamblador sum que los suma
*/

int g; //global g

int main()
{
    int a, b, c, d, e, f;       // variables locales
    a = b = c = d = e = f = 1;    // valores 
    g = sum(a,b,c,d,e,f);       // llama a sum(), pasando a,b,c,d,e,f
}

