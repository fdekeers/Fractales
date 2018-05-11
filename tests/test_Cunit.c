#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include <CUnit/CUCurses.h>


int main(int argc, char *argv[])
{
    CU_pSuite pSuite = NULL;
    CU_initialize_registry(); // On initialise le registre
    
    if (CUE_SUCCESS != CU_initialize_registry()){ // On teste si l'initialisation s'est déroulee correctement
        CU_cleanup_registry(); // On libere le registre qui a ete cree
        return CU_get_error(); // Si il y a eu un problème on retourne le code d'erreur
    }
    
    pSuite = CU_add_suite("ma_suite", 0, 0);
    else if ((NULL == CU_add_test(pSuite, "Test assert string equal", test_fractale_get_name)) ||
             (NULL == CU_add_test(pSuite, "Test assert equal", test_fractale_get_value)) ||
             (NULL == CU_add_test(pSuite, "Test assert equal", test_fractale_get_width)) ||
             (NULL == CU_add_test(pSuite, "Test assert equal", test_fractale_get_heigth)) ||
             (NULL == CU_add_test(pSuite, "Test assert equal", test_fractale_get_a)) ||
             (NULL == CU_add_test(pSuite, "Test assert equal", test_fractale_get_b))
             (NULL == CU_add_test(pSuite, "Test assert equal", test_push))||
             (NULL == CU_add_test(pSuite, "Test assert equal", test_pop))||
             (NULL == CU_add_test(pSuite, "Test assert ptr null", test_remove_buffer))){
        CU_cleanup_registry(); // On libere le registre qui a ete cree
        return CU_get_error(); // Si il y a eu un problème on retourne le code d'erreur
    }
    
    CU_basic_run_tests(); // On fait tourner les tests que nous avons cree
    CU_basic_show_failures(CU_get_failure_list()); // On affiche les erreurs
    CU_cleanup_registry(); // On libere le registre qui a ete cree
    
    return CU_get_error();
}

// Test de la fonction fractal_get_name()
void test_fractale_get_name(void){
    struct fractal * frac_test = fractal_new("fractale243", 120,640, -0.8,0.8);
    CU_ASSERT_STRING_EQUAL("fractale243",fractal_get_name(frac_test));
    fractal_free(frac_test);
}

// Test de la fonction fractal_get_value()
void test_fractale_get_value(void){
    struct fractal * frac_test = fractal_new("fractale243", 800,800, -0.8,0.8);
    fractal_set_value(frac_test, 20, 60, 350);
    CU_ASSERT_EQUAL(350,fractal_get_value(frac_test,20,60));
    fractal_free(frac_test);
}

// Test de la fonction fractal_get_width()
void test_fractale_get_width(void){
    struct fractal * frac_test = fractal_new("fractale243", 800,1200, -0.8,0.8);
    CU_ASSERT_EQUAL(800,fractal_get_width(frac_test));
    fractal_free(frac_test);
}

// Test de la fonction fractal_get_heigth()
void test_fractale_get_heigth(void){
    struct fractal * frac_test = fractal_new("fractale243", 800,1200, -0.8,0.8);
    CU_ASSERT_EQUAL(1200,fractal_get_heigth(frac_test));
    fractal_free(frac_test);
}

// Test de la fonction fractal_get_a()
void test_fractale_get_a(void){
    struct fractal * frac_test = fractal_new("fractale243", 800,1200, -0.8,0.5);
    CU_ASSERT_EQUAL(-0.8,fractal_get_a(frac_test));
    fractal_free(frac_test);
}

// Test de la fonction fractal_get_b()
void test_fractale_get_b(void){
    struct fractal * frac_test = fractal_new("fractale243", 800,1200, -0.8,0.5);
    CU_ASSERT_EQUAL(0.5,fractal_get_b(frac_test));
    fractal_free(frac_test);
}

// Test de la fonction push()
void test_push(void){
    struct fractal * frac_test1 = fractal_new("fractale37", 600,1000, -0.7,0.6);
    struct fractal * frac_test2 = fractal_new("fractale38", 500,1100, -0.4,0.9);
    
    noeud * n = createNoeud(frac_test1);
    push(n,frac_test2);
    noeud * runner = n;
    CU_ASSERT_STRING_EQUAL("fractale38",(runner->fractal->name));
    runner = runnuer->next;
    freeNoeud(runner);
    CU_ASSERT_STRING_EQUAL("fractale37",(runner->fractal->name));
    fractal_free(frac_test1);
    fractal_free(frac_test2);
    freeNoeud(runner);
}

// Test de la fonction pop()
void test_pop(void){
    struct fractal * frac_test1 = fractal_new("fractale37", 600,1000, -0.7,0.6);
    struct fractal * frac_test2 = fractal_new("fractale38", 500,1100, -0.4,0.9);
    
    noeud * n = createNoeud(frac_test1);
    push(n,frac_test2);
    pop(frac_test2);
    noeud * runner = n;
    CU_ASSERT_STRING_EQUAL("fractale37",(runner->fractal->name));
    freeNoeud(runner);
    fractal_free(frac_test1);
}

// Test de la fonction buffer_empty()
void test_remove_buffer(void){
    struct fractal * buffer;
        if(NULL!=buffer){
            remove_buffer();
            CU_ASSERT_PTR_NULL(buffer);
    }
}

// Test de la fonction add_buffer()
void test_add_buffer(void){
    struct fractal * buffer[10] = {NULL};
    struct fractal * frac_test;
    add_buffer(frac_test);
    CU_ASSERT_PTR_NOT_NULL(buffer);
}

// Test de la fonction buffer_empty() (On teste pour un buffer vide)
void test_buffer_empty(void){
    struct fractal * buffer[10] = {NULL};
    int value = buffer_empty();
    CU_ASSERT_EQUAL(value,1);
}

// Test de la fonction buffer_empty() (On teste pour un buffer non vide)
void test_buffer_empty(void){
    struct fractal * buffer[10] = {NULL};
    struct fractal * frac_test;
    add_buffer(frac_test);
    int value = buffer_empty();
    CU_ASSERT_EQUAL(value,0);
}

