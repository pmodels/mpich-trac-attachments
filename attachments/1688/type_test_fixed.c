#define MY_INT    1
#define MY_FLOAT  2
#define MY_DOUBLE 3

#define HAVE_DOUBLE

#define TYPES                                   \
    TYPE_MACRO(int, MY_INT, MY_T_INT)           \
    TYPE_MACRO(float, MY_FLOAT, MY_T_FLOAT)     \
    CHECK_DOUBLE(double, MY_DOUBLE, MY_T_DOUBLE)

#ifdef HAVE_DOUBLE
#define CHECK_DOUBLE(c_type_, my_type_, safe_name_) TYPE_MACRO(c_type_, my_type_, safe_name_)
#else
#define CHECK_DOUBLE(c_type_, my_type_, safe_name_)
#endif


/* Generate a struct from the type macros */
struct {
#define TYPE_MACRO(c_type_, my_type_, safe_name_) c_type_ foo_##safe_name_;
    TYPES
#undef TYPE_MACRO
} foo_s;


int main(int argc, char **argv) {
    int x = 1, y;

    /* Generate a switch statement from the type macros */
    switch(x) {
#define TYPE_MACRO(c_type_, my_type_, safe_name_) my_type_:
            TYPES
            y = x;
            break;
        default:
            y = -1;
            break;
#undef TYPE_MACRO
    }

    return 0;
}
